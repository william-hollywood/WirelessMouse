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
        public const uint CheckNum = 0x55555555;

        public static byte[] Encode(short dx, short dy, string password)
        {
            byte[] data = new byte[8];
            uint encoding = GetEncoding(ref password);

            uint values = (uint)(((ushort)dx << 16) + (ushort)dy);
            values ^= encoding;

            uint checksum = CheckNum ^ encoding;
            BitConverter.GetBytes(values).CopyTo(data, 0);
            BitConverter.GetBytes(checksum).CopyTo(data, 4);
            return data;
        }

        private static uint GetEncoding(ref string password)
        {
            uint encoding = CheckNum;
            // pad the password to be an even number of characters
            if (password.Length % 2 != 0)
            {
                password += ' ';
            }
            // just some bit shifting to act as a password endoding
            for (int i = 0; i < password.Length; i += 2)
            {
                uint ca = password[i];
                uint cb = password[i + 1];
                uint c1 = (cb >> 8) + (cb >> 4) + (ca << 4) + (ca << 8) + cb;
                uint c2 = (ca >> 8) + (cb >> 4) + (ca << 4) + (cb << 8) + ca;

                uint cc = (c1 << 16) + c2;
                encoding ^= cc;
            }

            return encoding;
        }

        public static (short dx, short dy) Decode(byte[] bytes, string password)
        {
            uint encoding = GetEncoding(ref password);
            uint checksum = CheckNum ^ encoding;

            uint inChecksum = BitConverter.ToUInt32(bytes.AsSpan()[4..]);
            if (checksum == inChecksum)
            {
                uint values = BitConverter.ToUInt32(bytes.AsSpan()[..4]);
                values ^= encoding;

                byte[] valBytes = BitConverter.GetBytes(values);
                short dx = BitConverter.ToInt16(valBytes.AsSpan()[..2]);
                short dy = BitConverter.ToInt16(valBytes.AsSpan()[2..4]);
                return (dx, dy);
            }
            return (0, 0);
        }
    }
}