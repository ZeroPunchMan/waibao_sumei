using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO.Ports;
//using System.Threading.Tasks;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.InteropServices;
using System.Reflection;
using SogProtocol;

namespace UART_demo
{
    public partial class Form1 : Form
    {
        private void SendVersionReq()
        {
            WbPacket packet = new WbPacket();
            packet.dir = 1;
            packet.len = 1;
            packet.type = (byte)ProtocolHelper.FunctionCode.ReadVersion;

            SendPacket(packet);
        }

        private void SendRunPauseReq(bool run)
        {
            WbPacket packet = new WbPacket();
            packet.dir = 1;
            packet.len = 2;
            packet.type = (byte)ProtocolHelper.FunctionCode.RunPause;
            if (run)
                packet.data[0] = 1;
            else
                packet.data[0] = 0;

            SendPacket(packet);
        }

        private void SendStopReq()
        {
            WbPacket packet = new WbPacket();
            packet.dir = 1;
            packet.len = 1;
            packet.type = (byte)ProtocolHelper.FunctionCode.Stop;

            SendPacket(packet);
        }

        private void SendTestMode1()
        {
            WbPacket packet = new WbPacket();
            packet.dir = 1;
            packet.len = 9;
            packet.type = (byte)ProtocolHelper.FunctionCode.Motors;

            for (int i = 0; i < 4; i++)
            {
                packet.data[i * 2] = (byte)(i * 10 + 10);
                packet.data[i * 2 + 1] = (byte)(i + 6);
            }

            SendPacket(packet);
        }

        private void SendTestMode2()
        {
            WbPacket packet = new WbPacket();
            packet.dir = 1;
            packet.len = 9;
            packet.type = (byte)ProtocolHelper.FunctionCode.Motors;

            packet.data[0] = 100;
            packet.data[1] = 10;

            packet.data[2] = 100;
            packet.data[3] = 10;

            packet.data[4] = 100;
            packet.data[5] = 10;

            packet.data[6] = 100;
            packet.data[7] = 10;


            SendPacket(packet);
        }
    }
}