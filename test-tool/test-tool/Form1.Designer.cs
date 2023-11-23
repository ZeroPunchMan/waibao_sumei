namespace UART_demo
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBoxReceive = new System.Windows.Forms.TextBox();
            this.comboBoxPort = new System.Windows.Forms.ComboBox();
            this.comboBoxBaudrate = new System.Windows.Forms.ComboBox();
            this.comboBoxDataBits = new System.Windows.Forms.ComboBox();
            this.comboBoxStopBits = new System.Windows.Forms.ComboBox();
            this.buttonOpenPort = new System.Windows.Forms.Button();
            this.buttonClearDisplay = new System.Windows.Forms.Button();
            this.checkBoxDisHex = new System.Windows.Forms.CheckBox();
            this.textBoxLog = new System.Windows.Forms.TextBox();
            this.checkBoxNewLine = new System.Windows.Forms.CheckBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.textBoxBleIdx = new System.Windows.Forms.TextBox();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.button7 = new System.Windows.Forms.Button();
            this.checkBoxPrintSend = new System.Windows.Forms.CheckBox();
            this.buttonGetVersion = new System.Windows.Forms.Button();
            this.buttonStart = new System.Windows.Forms.Button();
            this.buttonPause = new System.Windows.Forms.Button();
            this.buttonStop = new System.Windows.Forms.Button();
            this.buttonTestMode1 = new System.Windows.Forms.Button();
            this.buttonTestMode2 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBoxReceive
            // 
            this.textBoxReceive.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBoxReceive.Location = new System.Drawing.Point(433, 72);
            this.textBoxReceive.MaxLength = 655355;
            this.textBoxReceive.Multiline = true;
            this.textBoxReceive.Name = "textBoxReceive";
            this.textBoxReceive.ReadOnly = true;
            this.textBoxReceive.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxReceive.Size = new System.Drawing.Size(376, 407);
            this.textBoxReceive.TabIndex = 0;
            // 
            // comboBoxPort
            // 
            this.comboBoxPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPort.FormattingEnabled = true;
            this.comboBoxPort.Location = new System.Drawing.Point(12, 504);
            this.comboBoxPort.Name = "comboBoxPort";
            this.comboBoxPort.Size = new System.Drawing.Size(84, 20);
            this.comboBoxPort.TabIndex = 4;
            this.comboBoxPort.DropDown += new System.EventHandler(this.comboBoxPort_DropDown);
            // 
            // comboBoxBaudrate
            // 
            this.comboBoxBaudrate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxBaudrate.FormattingEnabled = true;
            this.comboBoxBaudrate.Location = new System.Drawing.Point(108, 504);
            this.comboBoxBaudrate.Name = "comboBoxBaudrate";
            this.comboBoxBaudrate.Size = new System.Drawing.Size(83, 20);
            this.comboBoxBaudrate.TabIndex = 5;
            // 
            // comboBoxDataBits
            // 
            this.comboBoxDataBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxDataBits.FormattingEnabled = true;
            this.comboBoxDataBits.Location = new System.Drawing.Point(11, 534);
            this.comboBoxDataBits.Name = "comboBoxDataBits";
            this.comboBoxDataBits.Size = new System.Drawing.Size(85, 20);
            this.comboBoxDataBits.TabIndex = 6;
            // 
            // comboBoxStopBits
            // 
            this.comboBoxStopBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxStopBits.FormattingEnabled = true;
            this.comboBoxStopBits.Location = new System.Drawing.Point(108, 534);
            this.comboBoxStopBits.Name = "comboBoxStopBits";
            this.comboBoxStopBits.Size = new System.Drawing.Size(83, 20);
            this.comboBoxStopBits.TabIndex = 7;
            // 
            // buttonOpenPort
            // 
            this.buttonOpenPort.Location = new System.Drawing.Point(12, 567);
            this.buttonOpenPort.Name = "buttonOpenPort";
            this.buttonOpenPort.Size = new System.Drawing.Size(75, 23);
            this.buttonOpenPort.TabIndex = 12;
            this.buttonOpenPort.Text = "打开串口";
            this.buttonOpenPort.UseVisualStyleBackColor = true;
            this.buttonOpenPort.Click += new System.EventHandler(this.buttonOpenPort_Click);
            // 
            // buttonClearDisplay
            // 
            this.buttonClearDisplay.Location = new System.Drawing.Point(733, 489);
            this.buttonClearDisplay.Name = "buttonClearDisplay";
            this.buttonClearDisplay.Size = new System.Drawing.Size(75, 23);
            this.buttonClearDisplay.TabIndex = 13;
            this.buttonClearDisplay.Text = "清除接收";
            this.buttonClearDisplay.UseVisualStyleBackColor = true;
            this.buttonClearDisplay.Click += new System.EventHandler(this.buttonClearDisplay_Click);
            // 
            // checkBoxDisHex
            // 
            this.checkBoxDisHex.AutoSize = true;
            this.checkBoxDisHex.Location = new System.Drawing.Point(650, 489);
            this.checkBoxDisHex.Margin = new System.Windows.Forms.Padding(2);
            this.checkBoxDisHex.Name = "checkBoxDisHex";
            this.checkBoxDisHex.Size = new System.Drawing.Size(66, 16);
            this.checkBoxDisHex.TabIndex = 37;
            this.checkBoxDisHex.Text = "hex显示";
            this.checkBoxDisHex.UseVisualStyleBackColor = true;
            // 
            // textBoxLog
            // 
            this.textBoxLog.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBoxLog.Location = new System.Drawing.Point(12, 12);
            this.textBoxLog.MaxLength = 655355;
            this.textBoxLog.Multiline = true;
            this.textBoxLog.Name = "textBoxLog";
            this.textBoxLog.ReadOnly = true;
            this.textBoxLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxLog.Size = new System.Drawing.Size(392, 467);
            this.textBoxLog.TabIndex = 38;
            // 
            // checkBoxNewLine
            // 
            this.checkBoxNewLine.AutoSize = true;
            this.checkBoxNewLine.Location = new System.Drawing.Point(554, 489);
            this.checkBoxNewLine.Margin = new System.Windows.Forms.Padding(2);
            this.checkBoxNewLine.Name = "checkBoxNewLine";
            this.checkBoxNewLine.Size = new System.Drawing.Size(72, 16);
            this.checkBoxNewLine.TabIndex = 41;
            this.checkBoxNewLine.Text = "自动换行";
            this.checkBoxNewLine.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(329, 489);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 43;
            this.button1.Text = "清除";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(433, 12);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(57, 23);
            this.button2.TabIndex = 44;
            this.button2.Text = "扫描";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // textBoxBleIdx
            // 
            this.textBoxBleIdx.Location = new System.Drawing.Point(505, 14);
            this.textBoxBleIdx.Name = "textBoxBleIdx";
            this.textBoxBleIdx.Size = new System.Drawing.Size(45, 21);
            this.textBoxBleIdx.TabIndex = 45;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(571, 12);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(55, 23);
            this.button3.TabIndex = 46;
            this.button3.Text = "连接";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(641, 12);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(57, 23);
            this.button4.TabIndex = 47;
            this.button4.Text = "断开";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(437, 43);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(53, 23);
            this.button5.TabIndex = 48;
            this.button5.Text = "重连";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(505, 43);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(55, 23);
            this.button6.TabIndex = 50;
            this.button6.Text = "开通知";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(571, 43);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(55, 23);
            this.button7.TabIndex = 51;
            this.button7.Text = "开写";
            this.button7.UseVisualStyleBackColor = true;
            this.button7.Click += new System.EventHandler(this.button7_Click);
            // 
            // checkBoxPrintSend
            // 
            this.checkBoxPrintSend.AutoSize = true;
            this.checkBoxPrintSend.Location = new System.Drawing.Point(235, 493);
            this.checkBoxPrintSend.Margin = new System.Windows.Forms.Padding(2);
            this.checkBoxPrintSend.Name = "checkBoxPrintSend";
            this.checkBoxPrintSend.Size = new System.Drawing.Size(72, 16);
            this.checkBoxPrintSend.TabIndex = 69;
            this.checkBoxPrintSend.Text = "打印发送";
            this.checkBoxPrintSend.UseVisualStyleBackColor = true;
            // 
            // buttonGetVersion
            // 
            this.buttonGetVersion.Location = new System.Drawing.Point(235, 518);
            this.buttonGetVersion.Name = "buttonGetVersion";
            this.buttonGetVersion.Size = new System.Drawing.Size(55, 23);
            this.buttonGetVersion.TabIndex = 52;
            this.buttonGetVersion.Text = "版本号";
            this.buttonGetVersion.UseVisualStyleBackColor = true;
            this.buttonGetVersion.Click += new System.EventHandler(this.buttonGetVersion_Click);
            // 
            // buttonStart
            // 
            this.buttonStart.Location = new System.Drawing.Point(328, 518);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(55, 23);
            this.buttonStart.TabIndex = 70;
            this.buttonStart.Text = "开始";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
            // 
            // buttonPause
            // 
            this.buttonPause.Location = new System.Drawing.Point(404, 518);
            this.buttonPause.Name = "buttonPause";
            this.buttonPause.Size = new System.Drawing.Size(55, 23);
            this.buttonPause.TabIndex = 71;
            this.buttonPause.Text = "暂停";
            this.buttonPause.UseVisualStyleBackColor = true;
            this.buttonPause.Click += new System.EventHandler(this.buttonPause_Click);
            // 
            // buttonStop
            // 
            this.buttonStop.Location = new System.Drawing.Point(328, 547);
            this.buttonStop.Name = "buttonStop";
            this.buttonStop.Size = new System.Drawing.Size(55, 23);
            this.buttonStop.TabIndex = 72;
            this.buttonStop.Text = "停止";
            this.buttonStop.UseVisualStyleBackColor = true;
            this.buttonStop.Click += new System.EventHandler(this.buttonStop_Click);
            // 
            // buttonTestMode1
            // 
            this.buttonTestMode1.Location = new System.Drawing.Point(537, 518);
            this.buttonTestMode1.Name = "buttonTestMode1";
            this.buttonTestMode1.Size = new System.Drawing.Size(72, 23);
            this.buttonTestMode1.TabIndex = 73;
            this.buttonTestMode1.Text = "测试模式1";
            this.buttonTestMode1.UseVisualStyleBackColor = true;
            this.buttonTestMode1.Click += new System.EventHandler(this.buttonTestMode1_Click);
            // 
            // buttonTestMode2
            // 
            this.buttonTestMode2.Location = new System.Drawing.Point(627, 518);
            this.buttonTestMode2.Name = "buttonTestMode2";
            this.buttonTestMode2.Size = new System.Drawing.Size(71, 23);
            this.buttonTestMode2.TabIndex = 74;
            this.buttonTestMode2.Text = "测试模式2";
            this.buttonTestMode2.UseVisualStyleBackColor = true;
            this.buttonTestMode2.Click += new System.EventHandler(this.buttonTestMode2_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(832, 602);
            this.Controls.Add(this.buttonTestMode2);
            this.Controls.Add(this.buttonTestMode1);
            this.Controls.Add(this.buttonStop);
            this.Controls.Add(this.buttonPause);
            this.Controls.Add(this.buttonStart);
            this.Controls.Add(this.checkBoxPrintSend);
            this.Controls.Add(this.buttonGetVersion);
            this.Controls.Add(this.button7);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.textBoxBleIdx);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.checkBoxNewLine);
            this.Controls.Add(this.textBoxLog);
            this.Controls.Add(this.checkBoxDisHex);
            this.Controls.Add(this.buttonClearDisplay);
            this.Controls.Add(this.buttonOpenPort);
            this.Controls.Add(this.comboBoxStopBits);
            this.Controls.Add(this.comboBoxDataBits);
            this.Controls.Add(this.comboBoxBaudrate);
            this.Controls.Add(this.comboBoxPort);
            this.Controls.Add(this.textBoxReceive);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "sm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxReceive;
        private System.Windows.Forms.ComboBox comboBoxPort;
        private System.Windows.Forms.ComboBox comboBoxBaudrate;
        private System.Windows.Forms.ComboBox comboBoxDataBits;
        private System.Windows.Forms.ComboBox comboBoxStopBits;
        private System.Windows.Forms.Button buttonOpenPort;
        private System.Windows.Forms.Button buttonClearDisplay;
        private System.Windows.Forms.CheckBox checkBoxDisHex;
        private System.Windows.Forms.TextBox textBoxLog;
        private System.Windows.Forms.CheckBox checkBoxNewLine;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox textBoxBleIdx;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.CheckBox checkBoxPrintSend;
        private System.Windows.Forms.Button buttonGetVersion;
        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.Button buttonPause;
        private System.Windows.Forms.Button buttonStop;
        private System.Windows.Forms.Button buttonTestMode1;
        private System.Windows.Forms.Button buttonTestMode2;
    }
}

