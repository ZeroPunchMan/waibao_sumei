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
                case (byte)ProtocolHelper.FunctionCode.RunPause:
                    OnRecvRunPause(packet);
                    break;
                case (byte)ProtocolHelper.FunctionCode.Stop:
                    OnRecvStop(packet);
                    break;
                case (byte)ProtocolHelper.FunctionCode.Motors:
                    OnRecvMotors(packet);
                    break;
                case (byte)ProtocolHelper.FunctionCode.Battery:
                    OnRecvBattery(packet);
                    break;
                case (byte)ProtocolHelper.FunctionCode.ReadVersion:
                    OnRecvVersion(packet);
                    break;
            }
            //string s = string.Format("len: 0x{0:x}, id: 0x{1:x}{2}x}}, cmd: 0x{3:x}, cmdsta: 0x{4:x}", packet.length, packet.devId[0], packet.devId[1], packet.cmdType, packet.cmdStatus);
        }

        void OnRecvRunPause(WbPacket packet)
        {
            if (packet.len != 2)
            {
                DebugLog("run pause 长度错误");
                return;
            }

            switch (packet.data[0])
            {
                case 0:
                    DebugLog("暂停 ok");
                    break;
                case 1:
                    DebugLog("运行 ok");
                    break;
                case 2:
                    DebugLog("运行失败：电量低");
                    break;
                case 3:
                    DebugLog("运行失败：充电中");
                    break;
            }
        }


        void OnRecvStop(WbPacket packet)
        {
            if (packet.len != 1)
            {
                DebugLog("stop 长度错误");
                return;
            }

            DebugLog("stop ok");
        }

        void OnRecvMotors(WbPacket packet)
        {
            //todo
            //if (packet.len != 3)
            //{
            //    DebugLog("motors长度错误");
            //    return;
            //}

            //string s = string.Format("ver: {0}.{1}", packet.data[0], packet.data[1]);
            //DebugLog(s);
        }

        void OnRecvBattery(WbPacket packet)
        {
            if (packet.len != 3)
            {
                DebugLog("battery长度错误");
                return;
            }

            string sta = "undefined";
            switch (packet.data[1])
            {
                case 1:
                    sta = "ok";
                    break;
                case 2:
                    sta = "电量低";
                    break;
                case 3:
                    sta = "充电中";
                    break;
            }
            string s = string.Format("电量: {0}, 状态: {1}", packet.data[0], sta);
            DebugLog(s);
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