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
        byte[] encryptKey = new byte[2];
        byte[] encryptRand = new byte[2];
        byte[] deviceId = new byte[2] { 0x00, 0x13 };
        void ProcessPacket(WbPacket packet)
        {
            switch (packet.type)
            {
                case (byte)ProtocolHelper.FunctionCode.ReadVersion:
                    OnRecvVersion(packet);
                    break;
            }
            //string s = string.Format("len: 0x{0:x}, id: 0x{1:x}{2}x}}, cmd: 0x{3:x}, cmdsta: 0x{4:x}", packet.length, packet.devId[0], packet.devId[1], packet.cmdType, packet.cmdStatus);
        }



        void OnRecvVersion(WbPacket packet)
        {
            if (packet.len != 3)
            {
                DebugLog("版本长度错误");
                return;
            }

            string s = string.Format("ver: {0}.{1}", packet.data[0], packet.data[1]);
            DebugLog(s);
        }

    }
}