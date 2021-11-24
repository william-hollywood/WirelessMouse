using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Encoding
{
    [TestFixture]
    internal class Tests
    {
        [Test]
        public void Encode_Obfuscates_Bytes()
        {
            byte[] data = Encoding.Encode(0, 0, "SomeText");
            short dx = BitConverter.ToInt16(data, 0);
            short dy = BitConverter.ToInt16(data, 2);
            Assert.AreNotEqual(dx, 0);
            Assert.AreNotEqual(dy, 0);
        }

        [Test]
        public void Decode_Correctly_Decodes_Bytes()
        {
            string password = "SomeText";
            short inX = 16000;
            short inY = 16000;
            byte[] data = Encoding.Encode(inX, inY, password);
            (short dx, short dy) = Encoding.Decode(data, password);
            Assert.AreEqual(inX, dx);
            Assert.AreEqual(inY, dy);
        }

        [Test]
        public void EncodeDecode_All_Combinations()
        {
            string password = "SomeText";
            for (short i = short.MinValue; i <= short.MaxValue; i++)
            {
                byte[] data = Encoding.Encode(i, i, password);
                (short dx, short dy) = Encoding.Decode(data, password);
                Assert.AreEqual(i, dx);
                Assert.AreEqual(i, dy);
            }
        }
    }
}
