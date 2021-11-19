using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Reactive.Linq;
using System.Text;

namespace DesktopApp
{
    public partial class Client : Form
    {
        private readonly IDisposable stream;

        public static IObservable<UdpReceiveResult> UdpStream()
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

        public Client()
        {
            InitializeComponent();
            this.stream = UdpStream().Subscribe(udp => HandleUDP(udp));
        }

        private static void HandleUDP(UdpReceiveResult udp)
        {
            Debug.WriteLine($"{Encoding.ASCII.GetString(udp.Buffer)}");
        }
    }
}