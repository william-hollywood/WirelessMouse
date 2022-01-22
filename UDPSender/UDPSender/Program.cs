using Encoding;
using System.Net;
using System.Net.Sockets;

Console.WriteLine("Starting UDP sending");

UdpClient udp = new UdpClient();

for (int i = 0; i < 100000; i++)
{
    var data = ((short)5, (short)5).Encode("default");
    udp.SendAsync(data, data.Length, "192.168.1.6", 8192);
}
