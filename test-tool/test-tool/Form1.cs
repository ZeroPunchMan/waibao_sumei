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
        readonly int[] allBaudrates = { 2400, 4800, 9600, 38400, 57600, 74880, 115200, 115200 * 2, 115200 * 5 };  //波特率
        readonly int[] allDataBits = { 5, 6, 7, 8 };    //数据位数
        readonly float[] allStopBits = { 1, 1.5f, 2 };    //停止位数
        readonly string strCfgFileName = "uart-tool.cfg";

        PortCfg config = null;
        SerialPort serialPort = null;

        Receiver receiver;

        public Form1()
        {

            InitializeComponent();

            //串口列表 
            comboBoxPort.Items.AddRange(SerialPort.GetPortNames());

            //波特率列表 
            comboBoxBaudrate.BeginUpdate();
            foreach (int br in allBaudrates)
            {
                comboBoxBaudrate.Items.Add(br.ToString());
            }
            comboBoxBaudrate.EndUpdate();

            //数据位列表 
            comboBoxDataBits.BeginUpdate();
            foreach (int db in allDataBits)
            {
                comboBoxDataBits.Items.Add(db.ToString());
            }
            comboBoxDataBits.EndUpdate();

            //停止位列表 
            comboBoxStopBits.BeginUpdate();
            foreach (float sb in allStopBits)
            {
                comboBoxStopBits.Items.Add(sb.ToString());
            }
            comboBoxStopBits.EndUpdate();


            try
            {
                using (FileStream stream = File.OpenRead(strCfgFileName))
                {
                    BinaryFormatter deserializer = new BinaryFormatter();   //二进制序列化器
                    config = deserializer.Deserialize(stream) as PortCfg;    //反序列化

                    int i = 0;
                    foreach (string name in SerialPort.GetPortNames())
                    {
                        if (name == config.portName)
                            break;
                        i++;
                    }

                    comboBoxPort.SelectedIndex = Clamp<int>(i, 0, comboBoxPort.Items.Count - 1);
                    comboBoxBaudrate.SelectedIndex = Clamp<int>(config.baudrateIdx, 0, comboBoxBaudrate.Items.Count - 1);
                    comboBoxDataBits.SelectedIndex = Clamp<int>(config.dataBitsIdx, 0, comboBoxDataBits.Items.Count - 1);
                    comboBoxStopBits.SelectedIndex = Clamp<int>(config.stopBitsIdx, 0, comboBoxStopBits.Items.Count - 1);


                    stream.Close();
                }
            }
            catch
            {
                //配置文件不存在,使用默认设置 
                if (comboBoxPort.Items.Count > 0)
                    comboBoxPort.SelectedIndex = 0;

                comboBoxBaudrate.SelectedIndex = 2;
                comboBoxDataBits.SelectedIndex = 3;
                comboBoxStopBits.SelectedIndex = 0;

                config = new PortCfg();
            }

            Type type = textBoxReceive.GetType();
            MethodInfo setStyle = type.GetMethod("SetStyle", BindingFlags.Instance | BindingFlags.NonPublic);
            MethodInfo updateStyle = type.GetMethod("UpdateStyles", BindingFlags.Instance | BindingFlags.NonPublic);
            try
            {
                object[] param = new object[2];
                param[0] = ControlStyles.OptimizedDoubleBuffer;
                param[1] = true;
                setStyle.Invoke(textBoxReceive, param);
                updateStyle.Invoke(textBoxReceive, null);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }

            receiver = new SogProtocol.Receiver(null);

        }

        bool portOpened
        {
            get
            {
                bool res = serialPort != null && serialPort.IsOpen;
                return res;
            }
        }

        //打开端口按钮
        private void buttonOpenPort_Click(object sender, EventArgs e)
        {
            if (portOpened)
            {   //关闭串口
                ClosePort();

                buttonOpenPort.Text = "打开串口";
            }
            else
            {   //打开串口
                if (TryOpenPort())
                {
                    buttonOpenPort.Text = "关闭串口";
                }
                else
                {
                    MessageBox.Show("打开串口失败");
                    buttonOpenPort.Text = "打开串口";
                }
            }

        }

        //尝试打开端口
        bool TryOpenPort()
        {
            serialPort = new SerialPort();
            //设置端口
            if (comboBoxPort.Text != null && comboBoxPort.Text != "")
                serialPort.PortName = comboBoxPort.Text;
            else
                return false;
            //设置波特率
            serialPort.BaudRate = allBaudrates[comboBoxBaudrate.SelectedIndex];
            //设置校验
            serialPort.Parity = Parity.None;
            //设置数据位数
            serialPort.DataBits = allDataBits[comboBoxDataBits.SelectedIndex];

            //设置停止位
            if (comboBoxStopBits.Text == "1")
            {
                serialPort.StopBits = StopBits.One;
            }
            else if (comboBoxStopBits.Text == "1.5")
            {
                serialPort.StopBits = StopBits.OnePointFive;
            }
            else if (comboBoxStopBits.Text == "2")
            {
                serialPort.StopBits = StopBits.Two;
            }

            //接收数据回调
            serialPort.DataReceived += this.OnReceivedData;
            //回调阈值设置为1
            serialPort.ReceivedBytesThreshold = 1;

            try
            {
                serialPort.Open();
            }
            catch
            {
                serialPort = null;
                return false;
            }

            config.portName = serialPort.PortName;
            config.baudrateIdx = comboBoxBaudrate.SelectedIndex;
            config.dataBitsIdx = comboBoxDataBits.SelectedIndex;
            config.stopBitsIdx = comboBoxStopBits.SelectedIndex;

            SaveConfiguration();
            return true;
        }

        void SaveConfiguration()
        {
            using (FileStream stream = File.Create(strCfgFileName))
            {
                BinaryFormatter serializer = new BinaryFormatter();　　//二进制格式序列化器
                serializer.Serialize(stream, config);　　//序列化对象到文件中
                stream.Close();
            }
        }

        //关闭端口
        void ClosePort()
        {
            if (serialPort != null)
            {
                serialPort.Close();
                serialPort = null;
            }

        }

        delegate void AppendNewReceivedData(byte[] data);
        //接收数据处理
        void AppendRecvData(byte[] data)
        {
            if (!this.checkBoxDisHex.Checked)
            {   //字符串显示
                string s = System.Text.Encoding.ASCII.GetString(data);
                this.textBoxReceive.AppendText(s);
            }
            else
            {   //hex显示
                StringBuilder sb = new StringBuilder();
                foreach (byte b in data)
                {
                    sb.AppendFormat("{0:x2} ", b);
                }
                this.textBoxReceive.AppendText(sb.ToString());
            }

            if (checkBoxNewLine.Checked)
                this.textBoxReceive.AppendText(Environment.NewLine);


            WbPacket[] packets = receiver.ParseBytes(data, 0, data.Length);
            foreach (WbPacket packet in packets)
            {
                if (packet != null)
                    ProcessPacket(packet);
            }
        }

        //接收回调
        void OnReceivedData(object sender, SerialDataReceivedEventArgs e)
        {
            byte[] data = new byte[serialPort.BytesToRead];
            serialPort.Read(data, 0, data.Length);//读取数据

            this.BeginInvoke(new AppendNewReceivedData(AppendRecvData), data);
        }

        //端口列表下拉
        private void comboBoxPort_DropDown(object sender, EventArgs e)
        {
            int tmp = comboBoxPort.SelectedIndex;
            comboBoxPort.Items.Clear();
            comboBoxPort.Items.AddRange(SerialPort.GetPortNames());

            if (comboBoxPort.Items.Count <= tmp)
                tmp = -1;

            comboBoxPort.SelectedIndex = tmp;
        }

        private void buttonClearDisplay_Click(object sender, EventArgs e)
        {
            //清除窗口
            textBoxReceive.Text = null;
        }



        //form关闭时
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            ClosePort(); //关闭串口
        }

        //clamp
        public static T Clamp<T>(T value, T min, T max)
            where T : System.IComparable<T>
        {
            T result = value;
            if (value.CompareTo(max) > 0)
                result = max;
            if (value.CompareTo(min) < 0)
                result = min;
            return result;
        }

        void PortSend(byte[] data, int offset, int len)
        {
            serialPort.Write(data, offset, len);
        }

        void DebugLog(string s)
        {
            textBoxLog.AppendText(s);
            textBoxLog.AppendText(Environment.NewLine);
        }


        private void button1_Click(object sender, EventArgs e)
        {
            textBoxLog.Clear();
        }

        string cmdBleDiscovery = "AT+DISC?\r\n";
        private void button2_Click(object sender, EventArgs e)
        {
            if (portOpened)
            {
                byte[] data = Encoding.ASCII.GetBytes(cmdBleDiscovery);
                serialPort.Write(data, 0, data.Length);
            }
        }

        string cmdBleConnTemplate = "AT+CONN{0}";
        private void button3_Click(object sender, EventArgs e)
        {
            if (!portOpened)
                return;

            int bleIdx = int.Parse(textBoxBleIdx.Text);
            string cmdBleConn = string.Format(cmdBleConnTemplate, bleIdx);

            byte[] data = Encoding.ASCII.GetBytes(cmdBleConn);
            serialPort.Write(data, 0, data.Length);
        }

        string cmdBleDisconnect = "AT\r\n";
        private void button4_Click(object sender, EventArgs e)
        {
            if (!portOpened)
                return;

            byte[] data = Encoding.ASCII.GetBytes(cmdBleDisconnect);
            serialPort.Write(data, 0, data.Length);
        }

        string cmdBleReconnect = "AT+CONNL\r\n";
        private void button5_Click(object sender, EventArgs e)
        {
            if (!portOpened)
                return;

            byte[] data = Encoding.ASCII.GetBytes(cmdBleReconnect);
            serialPort.Write(data, 0, data.Length);
        }


        string cmdBleNotify = "AT+NOTIFY_ON000D\r\n";
        private void button6_Click(object sender, EventArgs e)
        {
            if (!portOpened)
                return;

            byte[] data = Encoding.ASCII.GetBytes(cmdBleNotify);
            serialPort.Write(data, 0, data.Length);
        }

        string cmdBleWrite = "AT+SET_WAYWN0010\r\n";
        private void button7_Click(object sender, EventArgs e)
        {
            if (!portOpened)
                return;

            byte[] data = Encoding.ASCII.GetBytes(cmdBleWrite);
            serialPort.Write(data, 0, data.Length);
        }


        void SendPacket(WbPacket packet) //此方法会重新计算CRC
        {
            if (portOpened)
            {
                byte[] all = new byte[packet.len + 5];
                int count = 0;
                all[count++] = 0xae;
                all[count++] = 0x01;
                all[count++] = 0xf0;
                all[count++] = packet.len;
                all[count++] = packet.type;

                for (int i = 0; i < packet.len - 1; i++)
                    all[count++] = packet.data[i];

                all[count++] = ProtocolHelper.CalcVerify(packet.type, packet.data, (byte)(packet.len - 1));


                serialPort.Write(all, 0, all.Length);

                if (checkBoxPrintSend.Checked)
                {
                    StringBuilder sb = new StringBuilder();
                    sb.Append("发送: ");
                    foreach (byte b in all)
                        sb.Append(string.Format("0x{0:x2} ", b));

                    DebugLog(sb.ToString());
                }
            }
        }

        int CopyStrToByteArray(string s, byte[] des, int offset)
        {
            byte[] data = Encoding.ASCII.GetBytes(s);
            if (data.Length > des.Length - offset)
            {
                return -1;
            }

            foreach (byte b in data)
            {
                des[offset++] = b;
            }

            return data.Length;
        }
        private void buttonGetVersion_Click(object sender, EventArgs e)
        {
            SendVersionReq();
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            SendRunPauseReq(true);
        }

        private void buttonPause_Click(object sender, EventArgs e)
        {
            SendRunPauseReq(false);
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            SendStopReq();
        }

        private void buttonTestMode1_Click(object sender, EventArgs e)
        {
            SendTestMode1();
        }

        private void buttonTestMode2_Click(object sender, EventArgs e)
        {
            SendTestMode2();
        }
    }



    [Serializable]
    class PortCfg
    {
        public string portName = null; //串口名字
        public int baudrateIdx = 2; //这些配置的可选项是硬编码的内容,用索引存
        public int dataBitsIdx = 3;
        public int stopBitsIdx = 0;
        public string upgradeFile = null;
    }
}
