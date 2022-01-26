using System;
using System.Collections;
using NUnit.Framework.Internal;

namespace Encoding
{
    public static class Encoding
    {
        /// <summary>
        /// representation of `0101` repeated 8 times
        /// Int value: 1431655765
        /// </summary>v
        public const uint CheckNum = 0x55555555;

        public static byte[] Encode(this (short dx, short dy) encodeValues, string password)
        {
            byte[] data = new byte[12];
            uint encoding = GetEncoding(ref password);

            uint type = 0 ^ encoding;

            uint values = (uint)(((ushort)encodeValues.dx << 16) + (ushort)encodeValues.dy);
            values ^= encoding;

            uint checksum = CheckNum ^ encoding;
            BitConverter.GetBytes(type).CopyTo(data, 0);
            BitConverter.GetBytes(values).CopyTo(data, 4);
            BitConverter.GetBytes(checksum).CopyTo(data, 8);
            return data;
        }

        public static byte[] Encode(this int encodeKey, string password)
        {
            byte[] data = new byte[12];
            uint encoding = GetEncoding(ref password);

            uint type = 1 ^ encoding;

            uint value = (uint)encodeKey;
            value ^= encoding;

            uint checksum = CheckNum ^ encoding;
            BitConverter.GetBytes(type).CopyTo(data, 0);
            BitConverter.GetBytes(value).CopyTo(data, 4);
            BitConverter.GetBytes(checksum).CopyTo(data, 8);
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

        public static void Decode(this byte[] bytes, string password, out uint type, out short dx, out short dy, out uint key)
        {
            uint encoding = GetEncoding(ref password);
            uint checksum = CheckNum ^ encoding;

            uint inChecksum = BitConverter.ToUInt32(bytes.AsSpan()[8..]);
            if (checksum == inChecksum)
            {
                type = BitConverter.ToUInt32(bytes.AsSpan()[0..4]) ^ encoding;
                switch (type)
                {
                    case 0:
                        bytes.AsSpan()[4..8].ToArray().DecodeMouse(encoding, out dx, out dy);
                        key = 0;
                        return;
                    case 1:
                        bytes.AsSpan()[4..8].ToArray().DecodeKey(encoding, out key);
                        dx = 0; 
                        dy = 0;
                        return;
                }
            }
            type = 0;
            key = 0;
            dx = 0;
            dy = 0;
            return;
        }

        public static void DecodeMouse(this byte[] bytes, uint encoding, out short dx, out short dy)
        {
            uint values = BitConverter.ToUInt32(bytes.AsSpan()[..4]);
            values ^= encoding;

            byte[] valBytes = BitConverter.GetBytes(values);
            dx = BitConverter.ToInt16(valBytes.AsSpan()[..2]);
            dy = BitConverter.ToInt16(valBytes.AsSpan()[2..4]);
        }

        public static void DecodeKey(this byte[] bytes, uint encoding, out uint key)
        {
            uint values = BitConverter.ToUInt32(bytes.AsSpan()[..4]);
            values ^= encoding;

            byte[] valBytes = BitConverter.GetBytes(values);
            key = BitConverter.ToUInt32(valBytes);
        }
    }
}