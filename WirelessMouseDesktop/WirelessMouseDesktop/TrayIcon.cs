using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Reactive.Linq;

namespace WirelessMouseDesktop
{
    /// <summary>
    /// TrayIcon to allow for application exiting
    /// </summary>
    public class TrayIcon : ApplicationContext
    {
        /// <summary>
        /// Main entry point for the application
        /// </summary>
        public static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new TrayIcon());
        }

        private readonly NotifyIcon trayIcon;
        private readonly IDisposable stream;

        /// <summary>
        /// Constructor to setup the TrayIcon
        /// </summary>
        public TrayIcon()
        {
            trayIcon = new();
            trayIcon.Icon = SystemIcons.Application;

            ContextMenuStrip menu = new();

            ToolStripMenuItem passwordItem = new();
            passwordItem.Text = "Change Password";
            passwordItem.Click += new(ShowPasswordEntry);
            _ = menu.Items.Add(passwordItem);

            ToolStripMenuItem exitItem = new();
            exitItem.Text = "Exit";
            exitItem.Click += new(Exit);
            _ = menu.Items.Add(exitItem);

            trayIcon.ContextMenuStrip = menu;
            trayIcon.Visible = true;
            stream = UdpStream().Subscribe(udp => MouseControl.HandleUDP(udp));
        }

        /// <summary>
        /// Create a UDPStream as an Observable
        /// </summary>
        /// <returns>IObservable of UdpReceiveResults</returns>
        private static IObservable<UdpReceiveResult> UdpStream()
        {
            return Observable.Using(() =>
            {
                UdpClient receiverUDP = new();
                receiverUDP.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
                receiverUDP.EnableBroadcast = true;
                receiverUDP.Client.ExclusiveAddressUse = false;
                receiverUDP.Client.Bind(new IPEndPoint(IPAddress.Any, 8192));
                return receiverUDP;
            }, udpClient => Observable.Defer(() => Observable.FromAsync(() => udpClient.ReceiveAsync())).Repeat());
        }

        /// <summary>
        /// Exit the application
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">eventArgs</param>
        private void Exit(object? sender, EventArgs e)
        {
            stream?.Dispose();
            trayIcon.Visible = false;
            Application.Exit();
        }

        /// <summary>
        /// Show the password entry prompt
        /// </summary>
        /// <param name="sender">sender of event</param>
        /// <param name="e">event args</param>
        private void ShowPasswordEntry(object? sender, EventArgs e)
        {
            string? newPass = ShowTextInput("Enter Password", "Password");
            if (newPass == null)
                return;
            if (string.IsNullOrWhiteSpace(newPass))
            {
                ShowAlert("Bad Password", "Invalid Password Entered.");
                return;
            }
            Properties.Settings.Default.Password = newPass;
            Properties.Settings.Default.Save();
            Properties.Settings.Default.Reload();
        }

        #region Popup helpers

        /// <summary>
        /// Show a popup with a text input
        /// </summary>
        /// <param name="windowTitle">Title of the window</param>
        /// <param name="message">Message to be displayed</param>
        /// <returns>string of result, null if canceled</returns>
        private static string? ShowTextInput(string windowTitle, string message)
        {
            int border = 10;
            int wid = 250;
            int hei = 150;
            int cx = (wid - (border * 2)) / 2;
            Form prompt = new()
            {
                Width = wid,
                Height = hei,
                FormBorderStyle = FormBorderStyle.FixedDialog,
                Text = windowTitle,
                StartPosition = FormStartPosition.CenterScreen
            };
            Label textLabel = new() { Left = 10, Top = border, Width = wid - 40, Text = message, TextAlign = ContentAlignment.TopCenter, Font = new Font(prompt.Font, FontStyle.Bold) };
            TextBox textBox = new() { Left = border, Top = border + 20, Width = wid - (border * 4), PlaceholderText = Properties.Settings.Default.Password };
            Button confirmation = new() { Text = "Ok", Left = cx - 25, Width = 50, Top = border + 70, DialogResult = DialogResult.OK };
            confirmation.Click += (sender, e) => { prompt.Close(); };
            prompt.Controls.Add(textBox);
            prompt.Controls.Add(confirmation);
            prompt.Controls.Add(textLabel);
            prompt.AcceptButton = confirmation;
            prompt.Select();
            prompt.KeyDown += (sender, args) => { textBox.Select(); };
            return prompt.ShowDialog() == DialogResult.OK ? textBox.Text : null;
        }

        /// <summary>
        /// Show an alert popup with a message
        /// </summary>
        /// <param name="windowTitle">Title of the window</param>
        /// <param name="message">Message to be displayed</param>
        private static void ShowAlert(string windowTitle, string message)
        {
            int border = 10;
            int wid = 250;
            int hei = 150;
            int cx = (wid - (border * 2)) / 2;
            Form prompt = new()
            {
                Width = wid,
                Height = hei,
                FormBorderStyle = FormBorderStyle.FixedDialog,
                Text = windowTitle,
                StartPosition = FormStartPosition.CenterScreen
            };
            Label textLabel = new() { Left = 10, Top = border + 30, Width = wid - 40, Text = message, TextAlign = ContentAlignment.TopCenter, Font = new Font(prompt.Font, FontStyle.Bold) };
            Button confirmation = new() { Text = "Ok", Left = cx - 25, Width = 50, Top = border + 70, DialogResult = DialogResult.OK };
            confirmation.Click += (sender, e) => { prompt.Close(); };
            prompt.Controls.Add(confirmation);
            prompt.Controls.Add(textLabel);
            prompt.AcceptButton = confirmation;
            prompt.Select();
            prompt.ShowDialog();
        }

        #endregion Popup helpers
    }
}
