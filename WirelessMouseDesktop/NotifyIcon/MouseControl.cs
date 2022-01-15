using System.Diagnostics;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using Encoding;

namespace DesktopApp
{
    internal class MouseControl
    {
        [DllImport("user32")]
        public static extern int SetCursorPos(int x, int y);

        [DllImport("user32.dll")]
        private static extern bool GetCursorPos(out Point point);

        public static string password = "default";

        public static void HandleUDP(UdpReceiveResult udp)
        {
            var (dx, dy) = udp.Buffer.Decode(password);

            /*_ = GetCursorPos(out var p);
            _ = SetCursorPos(p.X + dx, p.Y + dy);*/

            Debug.WriteLine($"{dx} {dy}");
        }
    }
}
