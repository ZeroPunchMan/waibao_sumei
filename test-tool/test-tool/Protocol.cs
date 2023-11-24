using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SogProtocol
{
    internal static class ProtocolHelper
    {
        public static readonly byte[] head = { 0x53, 0x47 };

        public enum FunctionCode
        {
            RunPause = 0x01,
            Stop = 0x03,
            Motors = 0x04,
            Battery = 0x05,
            ReadVersion = 0x66,
        }
        public static byte CalcVerify(byte type, byte[] data, byte count)
        {
            byte sum = type;
            for (byte i = 0; i < count; i++)
                sum += data[i];
            return sum;
        }
    }

    class Sender
    {
        public delegate void SendMethod(byte[] data, int offset, int len);
        SendMethod sendMethod;
        public delegate void LogMethod(string s);
        LogMethod logMethod;

        private void Log(string s)
        {
            this.logMethod?.Invoke(s);
        }

        public Sender(SendMethod sm, LogMethod lm)
        {
            this.sendMethod = sm;
            this.logMethod = lm;

            if (sendMethod == null)
            {
                throw new Exception("Sender construct error");
            }
        }
    }
    public class WbPacket
    {
        public byte dir;
        public byte len;
        public byte type;
        public byte[] data;

        public WbPacket()
        {
            data = new byte[256];
        }
    }

    class Receiver
    {
        enum ParseStatus
        {
            Head,
            Dir,
            Token,
            Len,
            Type,
            Data,
            Verify,
        }

        public delegate void LogMethod(string s);
        LogMethod logMethod;
        public Receiver(LogMethod lm)
        {
            this.logMethod = lm;
        }
        private void Log(string s)
        {
            this.logMethod?.Invoke(s);
        }

        ParseStatus status = ParseStatus.Head;
        WbPacket packet = null;
        int parseCount = 0;
        DateTime lastRecvTime = DateTime.Now;
        public WbPacket ParseByte(byte b)
        {
            TimeSpan span = DateTime.Now - lastRecvTime;
            if (span.TotalMilliseconds > 500)
            {
                parseCount = 0;
                status = ParseStatus.Head;
            }

            lastRecvTime = DateTime.Now;

            switch (status)
            {
                case ParseStatus.Head:
                    if (b == 0xae)
                    {
                        Log("head ok");
                        status = ParseStatus.Dir;
                        packet = new WbPacket();
                    }
                    break;
                case ParseStatus.Dir:
                    if (b == 1 || b == 2)
                    {
                        packet.dir = b;
                        status = ParseStatus.Token;
                    }
                    else
                    {
                        status = ParseStatus.Head;
                    }
                    break;
                case ParseStatus.Token:
                    if (b == 0xf0)
                    {
                        status = ParseStatus.Len;
                    }
                    else
                    {
                        status = ParseStatus.Head;
                    }
                    break;
                case ParseStatus.Len:
                    packet.len = b;
                    if (packet.len >= 1)
                        status = ParseStatus.Type;
                    else
                        status = ParseStatus.Head;
                    break;
                case ParseStatus.Type:
                    packet.type = b;
                    if (packet.len > 1)
                        status = ParseStatus.Data;
                    else
                        status = ParseStatus.Verify;

                    parseCount = 0;
                    break;
                case ParseStatus.Data:
                    packet.data[parseCount++] = b;
                    //Log(string.Format("data: {0}, 0x{1:x}", parseCount, b));
                    if (parseCount >= packet.len - 1)
                    {
                        status = ParseStatus.Verify;
                    }
                    break;
                case ParseStatus.Verify:
                    status = ParseStatus.Head;

                    byte verify = ProtocolHelper.CalcVerify(packet.type, packet.data, (byte)(packet.len - 1));
                    if (b == verify)
                    {
                        return packet;
                    }

                    break;
            }

            return null;
        }

        public WbPacket[] ParseBytes(byte[] data, int offset, int len)
        {
            WbPacket[] res = new WbPacket[20];
            int count = 0;
            for (int i = 0; i < res.Length; i++)
                res[i] = null;

            for (int i = 0; i < len; i++)
            {
                WbPacket packet = ParseByte(data[offset + i]);
                if (packet != null)
                {
                    res[count++] = packet;
                }
            }

            return res;
        }
    }

}


