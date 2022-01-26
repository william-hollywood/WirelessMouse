using NUnit.Framework;

namespace Encoding
{

    internal class Tests
    {
        [TestFixture]
        internal class MouseTests
        {
            [Test]
            public void Odd_Password_length()
            {
                byte[] data = ((short)0, (short)0).Encode("SomeText");
                short dx = BitConverter.ToInt16(data, 4);
                short dy = BitConverter.ToInt16(data, 6);
                Assert.AreNotEqual(dx, 0);
                Assert.AreNotEqual(dy, 0);
            }

            [Test]
            public void Encode_Obfuscates_Bytes()
            {
                byte[] data = ((short)0, (short)0).Encode("SomeText");
                short dx = BitConverter.ToInt16(data, 4);
                short dy = BitConverter.ToInt16(data, 6);
                Assert.AreNotEqual(0, dx);
                Assert.AreNotEqual(0, dy);
            }

            [Test]
            public void Decode_Correctly_Decodes_Bytes()
            {
                string password = "SomeText";
                short inX = 16000;
                short inY = 16000;
                byte[] data = (inX, inY).Encode(password);
                data.Decode(password, out uint type, out short dx, out short dy, out uint key);
                Assert.AreEqual(0, type);
                Assert.AreEqual(inX, dx);
                Assert.AreEqual(inY, dy);
            }

            [Test]
            public void EncodeDecode_All_Combinations()
            {
                string password = "SomeText";
                for (short i = short.MinValue; i < short.MaxValue; i++)
                {
                    byte[] data = (i, i).Encode(password);
                    data.Decode(password, out uint type, out short dx, out short dy, out uint key);
                    Assert.AreEqual(0, type);
                    Assert.AreEqual(i, dx);
                    Assert.AreEqual(i, dy);
                }
            }
        }

        [TestFixture]
        internal class KeyTests
        {

        }
    }
}
