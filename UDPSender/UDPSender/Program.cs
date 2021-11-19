using System.Net;
using System.Net.Sockets;
using System.Text;

Console.WriteLine("Starting UDP sending");

UdpClient udp = new UdpClient();

for (int i = 0; i < 100000; i++)
{
    var data = Encoding.ASCII.GetBytes($"Hello! {i}");
    udp.SendAsync(data, data.Length, "192.168.1.6", 8192);
}
