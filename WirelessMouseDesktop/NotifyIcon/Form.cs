using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Reactive.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace NotifyIcon
{
    public partial class Form : System.Windows.Forms.Form
    {
        [DllImport("user32")]
        public static extern int SetCursorPos(int x, int y);

        [DllImport("user32.dll")]
        static extern bool GetCursorPos(out Point point);

        public static void MouseExample()
        {

            Point p = new();
            // nw in
            _ = GetCursorPos(out p);
            _ = SetCursorPos(p.X - 50, p.Y - 50);
            // end

            Console.WriteLine($"{p.X} {p.Y}");
        }

        public static IObservable<UdpReceiveResult> UdpStream()
        {
            return Observable.Using(() =>
            {
                UdpClient receiverUDP = new UdpClient();
                receiverUDP.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
                receiverUDP.EnableBroadcast = true;
                receiverUDP.Client.ExclusiveAddressUse = false;
                receiverUDP.Client.Bind(new IPEndPoint(IPAddress.Any, 8192));
                return receiverUDP;
            },udpClient => Observable.Defer(() => Observable.FromAsync(() => udpClient.ReceiveAsync())).Repeat());
        }

        public Form()
        {
            InitializeComponent();
            UdpStream().Subscribe(udp => { Debug.WriteLine($"{Encoding.ASCII.GetString(udp.Buffer)}"); });
        }
    }
}