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
        private static extern int SetCursorPos(int x, int y);

        [DllImport("user32.dll")]
        private static extern bool GetCursorPos(out Point point);

        //used for sending the mouse events like

        [DllImport("user32.dll")]
        private static extern void mouse_event(uint dwFlags, uint dx, uint dy, uint dwData, IntPtr dwExtraInfo);

        //constants for the mouse events

        //found at http://msdn.microsoft.com/en-us/library/ms646273(v=vs.85).aspx

        private const uint MOUSEEVENTF_LEFTDOWN = 0x0002;  //The left button was pressed

        private const uint MOUSEEVENTF_LEFTUP = 0x0004;  //The left button was released.

        private const uint MOUSEEVENTF_RIGHTDOWN = 0x08;   //The right button was pressed

        private const uint MOUSEEVENTF_RIGHTUP = 0x10;   //The left button was released.

        private static string password = "Default";

        private static int lastX;
        private static int lastY;

        private const int SCALE = 3;

        public static void HandleUDP(UdpReceiveResult udp)
        {
            var (dx, dy) = udp.Buffer.Decode(password);
            if (dx == short.MaxValue && dy == short.MaxValue)
            {
                _ = GetCursorPos(out var p);
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, new IntPtr());
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, new IntPtr());
            }
            else if(dx == short.MinValue && dy == short.MinValue)
            {
                _ = GetCursorPos(out var p);
                mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, new IntPtr());
                mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, new IntPtr());
            } else
            {
                if (dx == 0 && dy == 0)
                {
                    _ = GetCursorPos(out var p);
                    lastX = p.Y; // ew
                    lastY = p.X;
                }
                _ = SetCursorPos(lastY + (dy*SCALE), lastX + (dx * SCALE));
            }
        }
    }
}
