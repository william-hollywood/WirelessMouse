using System.Net;
using System.Net.Sockets;
using System.Reactive.Linq;

namespace DesktopApp
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
            ToolStripMenuItem menuItem = new();

            menuItem.Text = "Exit";
            menuItem.Click += new(Exit);

            _ = menu.Items.Add(menuItem);
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
    }
}
