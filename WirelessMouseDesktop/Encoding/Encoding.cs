using System;
using System.Collections;

namespace Encoding
{
    public static class Encoding
    {
        /// <summary>
        /// representation of `0101` repeated 8 times
        /// Int value: 1431655765
        /// </summary>v
        public const int CheckNum = 0x55555555;

        public static byte[] Encode(short dx, short dy, string password)
        {
            byte[] data = new byte[8];
            int encoding = GetEncoding(ref password);
            int values = (dx << 16) + dy;
            values ^= encoding;
            int checksum = CheckNum ^ encoding;
            BitConverter.GetBytes(values).CopyTo(data, 0);
            BitConverter.GetBytes(checksum).CopyTo(data, 4);
            return data;
        }

        private static int GetEncoding(ref string password)
        {
            int encoding = CheckNum;
            // pad the password to be an even number of characters
            if (password.Length % 2 != 0)
            {
                password += ' ';
            }
            // just some bit shifting to act as a password endoding
            for (int i = 0; i < password.Length; i += 2)
            {
                int ca = password[i];
                int cb = password[i + 1];
                int c1 = (cb >> 8) + (cb >> 4) + (ca << 4) + (ca << 8) + cb;
                int c2 = (ca >> 8) + (cb >> 4) + (ca << 4) + (cb << 8) + ca;

                int cc = (c1 << 16) + c2;
                encoding ^= cc;
            }

            return encoding;
        }

        public static (short dx, short dy) Decode(byte[] bytes, string password)
        {
            short dx = 0, dy = 0;
            int encoding = GetEncoding(ref password);
            int checksum = CheckNum ^ encoding;

            int inChecksum = BitConverter.ToInt32(bytes.AsSpan()[4..]);
            if (checksum == inChecksum)
            {
                int values = BitConverter.ToInt32(bytes.AsSpan()[..4]);
                values ^= encoding;
                byte[] valBytes = BitConverter.GetBytes(values);
                dx = BitConverter.ToInt16(valBytes.AsSpan()[..2]);
                dy = BitConverter.ToInt16(valBytes.AsSpan()[2..4]);
                return (dx, dy);
            }
            return (0, 0);
        }
    }
}