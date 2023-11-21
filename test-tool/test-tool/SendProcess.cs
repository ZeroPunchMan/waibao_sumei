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

        private void SendStartReq()
        {
            WbPacket packet = new WbPacket();
            packet.dir = 1;
            packet.len = 1;
            packet.type = (byte)ProtocolHelper.FunctionCode.Start;

            SendPacket(packet);
        }
        private void SendPauseReq()
        {
            WbPacket packet = new WbPacket();
            packet.dir = 1;
            packet.len = 1;
            packet.type = (byte)ProtocolHelper.FunctionCode.Pause;

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
        { //todo
            WbPacket packet = new WbPacket();
            packet.dir = 1;
            packet.len = 1;
            packet.type = (byte)ProtocolHelper.FunctionCode.ReadVersion;

            SendPacket(packet);
        }

        private void SendTestMode2()
        { //todo
            WbPacket packet = new WbPacket();
            packet.dir = 1;
            packet.len = 1;
            packet.type = (byte)ProtocolHelper.FunctionCode.ReadVersion;

            SendPacket(packet);
        }
    }
}