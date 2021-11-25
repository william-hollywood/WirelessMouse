using System.Diagnostics;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;

namespace DesktopApp
{
    internal class MouseControl
    {
        [DllImport("user32")]
        public static extern int SetCursorPos(int x, int y);

        [DllImport("user32.dll")]
        private static extern bool GetCursorPos(out Point point);

        public static void MouseExample()
        {
            // nw in
            _ = GetCursorPos(out var p);
            _ = SetCursorPos(p.X - 50, p.Y - 50);
            // end

            Console.WriteLine($"{p.X} {p.Y}");
        }

        public static void HandleUDP(UdpReceiveResult udp)
        {
            Debug.WriteLine($"{System.Text.Encoding.ASCII.GetString(udp.Buffer)}");
        }
    }
}
