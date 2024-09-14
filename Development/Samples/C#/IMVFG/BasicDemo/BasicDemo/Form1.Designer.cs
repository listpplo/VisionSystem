namespace BasicDemo
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.pbImage = new System.Windows.Forms.PictureBox();
            this.btnOpenInterface = new System.Windows.Forms.Button();
            this.btnCloseInterface = new System.Windows.Forms.Button();
            this.cmbInterfaceList = new System.Windows.Forms.ComboBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnEnumInterfaces = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnSoftware = new System.Windows.Forms.Button();
            this.chbSoftTrigger = new System.Windows.Forms.CheckBox();
            this.btnStop = new System.Windows.Forms.Button();
            this.btnStart = new System.Windows.Forms.Button();
            this.radioTrigger = new System.Windows.Forms.RadioButton();
            this.radioContinue = new System.Windows.Forms.RadioButton();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnSaveBIN = new System.Windows.Forms.Button();
            this.btnSaveBMP = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cmbDeviceList = new System.Windows.Forms.ComboBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.btnEnumDevices = new System.Windows.Forms.Button();
            this.btnOpenDevice = new System.Windows.Forms.Button();
            this.btnCloseDevice = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.btnGetInterfaceParam = new System.Windows.Forms.Button();
            this.btnSetInterfaceParam = new System.Windows.Forms.Button();
            this.txtInterfaceHeight = new System.Windows.Forms.TextBox();
            this.txtInterfaceWidth = new System.Windows.Forms.TextBox();
            this.cmbInterfacePixelType = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.btnGetDeviceParam = new System.Windows.Forms.Button();
            this.btnSetDeviceParam = new System.Windows.Forms.Button();
            this.txtDeviceGainRaw = new System.Windows.Forms.TextBox();
            this.txtDeviceExposuretime = new System.Windows.Forms.TextBox();
            this.cmbDevicePixelType = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pbImage)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.SuspendLayout();
            // 
            // pbImage
            // 
            this.pbImage.Location = new System.Drawing.Point(12, 101);
            this.pbImage.Name = "pbImage";
            this.pbImage.Size = new System.Drawing.Size(723, 631);
            this.pbImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbImage.TabIndex = 0;
            this.pbImage.TabStop = false;
            // 
            // btnOpenInterface
            // 
            this.btnOpenInterface.Location = new System.Drawing.Point(6, 59);
            this.btnOpenInterface.Name = "btnOpenInterface";
            this.btnOpenInterface.Size = new System.Drawing.Size(75, 23);
            this.btnOpenInterface.TabIndex = 1;
            this.btnOpenInterface.Text = "打开采集卡";
            this.btnOpenInterface.UseVisualStyleBackColor = true;
            this.btnOpenInterface.Click += new System.EventHandler(this.btnOpenInterface_Click);
            // 
            // btnCloseInterface
            // 
            this.btnCloseInterface.Location = new System.Drawing.Point(119, 59);
            this.btnCloseInterface.Name = "btnCloseInterface";
            this.btnCloseInterface.Size = new System.Drawing.Size(75, 23);
            this.btnCloseInterface.TabIndex = 2;
            this.btnCloseInterface.Text = "关闭采集卡";
            this.btnCloseInterface.UseVisualStyleBackColor = true;
            this.btnCloseInterface.Click += new System.EventHandler(this.btnCloseInterface_Click);
            // 
            // cmbInterfaceList
            // 
            this.cmbInterfaceList.FormattingEnabled = true;
            this.cmbInterfaceList.Location = new System.Drawing.Point(65, 24);
            this.cmbInterfaceList.Name = "cmbInterfaceList";
            this.cmbInterfaceList.Size = new System.Drawing.Size(670, 20);
            this.cmbInterfaceList.TabIndex = 3;
            this.cmbInterfaceList.SelectedIndexChanged += new System.EventHandler(this.cmbInterfaceList_SelectedIndexChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnEnumInterfaces);
            this.groupBox1.Controls.Add(this.btnOpenInterface);
            this.groupBox1.Controls.Add(this.btnCloseInterface);
            this.groupBox1.Location = new System.Drawing.Point(741, 24);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(200, 100);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "采集卡";
            // 
            // btnEnumInterfaces
            // 
            this.btnEnumInterfaces.Location = new System.Drawing.Point(31, 21);
            this.btnEnumInterfaces.Name = "btnEnumInterfaces";
            this.btnEnumInterfaces.Size = new System.Drawing.Size(135, 23);
            this.btnEnumInterfaces.TabIndex = 0;
            this.btnEnumInterfaces.Text = "查找采集卡";
            this.btnEnumInterfaces.UseVisualStyleBackColor = true;
            this.btnEnumInterfaces.Click += new System.EventHandler(this.btnEnumInterfaces_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnSoftware);
            this.groupBox2.Controls.Add(this.chbSoftTrigger);
            this.groupBox2.Controls.Add(this.btnStop);
            this.groupBox2.Controls.Add(this.btnStart);
            this.groupBox2.Controls.Add(this.radioTrigger);
            this.groupBox2.Controls.Add(this.radioContinue);
            this.groupBox2.Location = new System.Drawing.Point(741, 236);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(200, 136);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "采集图像";
            // 
            // btnSoftware
            // 
            this.btnSoftware.Location = new System.Drawing.Point(119, 105);
            this.btnSoftware.Name = "btnSoftware";
            this.btnSoftware.Size = new System.Drawing.Size(75, 23);
            this.btnSoftware.TabIndex = 5;
            this.btnSoftware.Text = "软触发";
            this.btnSoftware.UseVisualStyleBackColor = true;
            this.btnSoftware.Click += new System.EventHandler(this.btnSoftware_Click);
            // 
            // chbSoftTrigger
            // 
            this.chbSoftTrigger.AutoSize = true;
            this.chbSoftTrigger.Location = new System.Drawing.Point(6, 109);
            this.chbSoftTrigger.Name = "chbSoftTrigger";
            this.chbSoftTrigger.Size = new System.Drawing.Size(60, 16);
            this.chbSoftTrigger.TabIndex = 4;
            this.chbSoftTrigger.Text = "软触发";
            this.chbSoftTrigger.UseVisualStyleBackColor = true;
            this.chbSoftTrigger.CheckedChanged += new System.EventHandler(this.chbSoftTrigger_CheckedChanged);
            // 
            // btnStop
            // 
            this.btnStop.Location = new System.Drawing.Point(119, 65);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(75, 23);
            this.btnStop.TabIndex = 3;
            this.btnStop.Text = "停止采集";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnStart
            // 
            this.btnStart.Location = new System.Drawing.Point(6, 65);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(75, 23);
            this.btnStart.TabIndex = 2;
            this.btnStart.Text = "开始采集";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // radioTrigger
            // 
            this.radioTrigger.AutoSize = true;
            this.radioTrigger.Location = new System.Drawing.Point(119, 32);
            this.radioTrigger.Name = "radioTrigger";
            this.radioTrigger.Size = new System.Drawing.Size(71, 16);
            this.radioTrigger.TabIndex = 1;
            this.radioTrigger.TabStop = true;
            this.radioTrigger.Text = "触发模式";
            this.radioTrigger.UseVisualStyleBackColor = true;
            this.radioTrigger.CheckedChanged += new System.EventHandler(this.radioTrigger_CheckedChanged);
            // 
            // radioContinue
            // 
            this.radioContinue.AutoSize = true;
            this.radioContinue.Location = new System.Drawing.Point(10, 32);
            this.radioContinue.Name = "radioContinue";
            this.radioContinue.Size = new System.Drawing.Size(71, 16);
            this.radioContinue.TabIndex = 0;
            this.radioContinue.TabStop = true;
            this.radioContinue.Text = "连续模式";
            this.radioContinue.UseVisualStyleBackColor = true;
            this.radioContinue.CheckedChanged += new System.EventHandler(this.radioContinue_CheckedChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btnSaveBIN);
            this.groupBox3.Controls.Add(this.btnSaveBMP);
            this.groupBox3.Location = new System.Drawing.Point(741, 378);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(200, 68);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "保存图片";
            // 
            // btnSaveBIN
            // 
            this.btnSaveBIN.Location = new System.Drawing.Point(119, 28);
            this.btnSaveBIN.Name = "btnSaveBIN";
            this.btnSaveBIN.Size = new System.Drawing.Size(75, 23);
            this.btnSaveBIN.TabIndex = 1;
            this.btnSaveBIN.Text = "保存BIN";
            this.btnSaveBIN.UseVisualStyleBackColor = true;
            this.btnSaveBIN.Click += new System.EventHandler(this.btnSaveBIN_Click);
            // 
            // btnSaveBMP
            // 
            this.btnSaveBMP.Location = new System.Drawing.Point(10, 29);
            this.btnSaveBMP.Name = "btnSaveBMP";
            this.btnSaveBMP.Size = new System.Drawing.Size(75, 23);
            this.btnSaveBMP.TabIndex = 0;
            this.btnSaveBMP.Text = "保存BMP";
            this.btnSaveBMP.UseVisualStyleBackColor = true;
            this.btnSaveBMP.Click += new System.EventHandler(this.btnSaveBMP_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 27);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "采集卡";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 62);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "设备";
            // 
            // cmbDeviceList
            // 
            this.cmbDeviceList.FormattingEnabled = true;
            this.cmbDeviceList.Location = new System.Drawing.Point(65, 59);
            this.cmbDeviceList.Name = "cmbDeviceList";
            this.cmbDeviceList.Size = new System.Drawing.Size(670, 20);
            this.cmbDeviceList.TabIndex = 9;
            this.cmbDeviceList.SelectedIndexChanged += new System.EventHandler(this.cmbDeviceList_SelectedIndexChanged);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.btnEnumDevices);
            this.groupBox4.Controls.Add(this.btnOpenDevice);
            this.groupBox4.Controls.Add(this.btnCloseDevice);
            this.groupBox4.Location = new System.Drawing.Point(741, 130);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(200, 100);
            this.groupBox4.TabIndex = 10;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "相机设备";
            // 
            // btnEnumDevices
            // 
            this.btnEnumDevices.Location = new System.Drawing.Point(31, 21);
            this.btnEnumDevices.Name = "btnEnumDevices";
            this.btnEnumDevices.Size = new System.Drawing.Size(135, 23);
            this.btnEnumDevices.TabIndex = 0;
            this.btnEnumDevices.Text = "查找设备";
            this.btnEnumDevices.UseVisualStyleBackColor = true;
            this.btnEnumDevices.Click += new System.EventHandler(this.btnEnumDevices_Click);
            // 
            // btnOpenDevice
            // 
            this.btnOpenDevice.Location = new System.Drawing.Point(6, 59);
            this.btnOpenDevice.Name = "btnOpenDevice";
            this.btnOpenDevice.Size = new System.Drawing.Size(75, 23);
            this.btnOpenDevice.TabIndex = 1;
            this.btnOpenDevice.Text = "打开设备";
            this.btnOpenDevice.UseVisualStyleBackColor = true;
            this.btnOpenDevice.Click += new System.EventHandler(this.btnOpenDevice_Click);
            // 
            // btnCloseDevice
            // 
            this.btnCloseDevice.Location = new System.Drawing.Point(119, 59);
            this.btnCloseDevice.Name = "btnCloseDevice";
            this.btnCloseDevice.Size = new System.Drawing.Size(75, 23);
            this.btnCloseDevice.TabIndex = 2;
            this.btnCloseDevice.Text = "关闭设备";
            this.btnCloseDevice.UseVisualStyleBackColor = true;
            this.btnCloseDevice.Click += new System.EventHandler(this.btnCloseDevice_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.btnGetInterfaceParam);
            this.groupBox5.Controls.Add(this.btnSetInterfaceParam);
            this.groupBox5.Controls.Add(this.txtInterfaceHeight);
            this.groupBox5.Controls.Add(this.txtInterfaceWidth);
            this.groupBox5.Controls.Add(this.cmbInterfacePixelType);
            this.groupBox5.Controls.Add(this.label3);
            this.groupBox5.Controls.Add(this.label4);
            this.groupBox5.Controls.Add(this.label5);
            this.groupBox5.Location = new System.Drawing.Point(741, 452);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(200, 137);
            this.groupBox5.TabIndex = 11;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "采集卡参数";
            // 
            // btnGetInterfaceParam
            // 
            this.btnGetInterfaceParam.Location = new System.Drawing.Point(6, 106);
            this.btnGetInterfaceParam.Name = "btnGetInterfaceParam";
            this.btnGetInterfaceParam.Size = new System.Drawing.Size(75, 23);
            this.btnGetInterfaceParam.TabIndex = 7;
            this.btnGetInterfaceParam.Text = "获取参数";
            this.btnGetInterfaceParam.UseVisualStyleBackColor = true;
            this.btnGetInterfaceParam.Click += new System.EventHandler(this.btnGetInterfaceParam_Click);
            // 
            // btnSetInterfaceParam
            // 
            this.btnSetInterfaceParam.Location = new System.Drawing.Point(111, 106);
            this.btnSetInterfaceParam.Name = "btnSetInterfaceParam";
            this.btnSetInterfaceParam.Size = new System.Drawing.Size(75, 23);
            this.btnSetInterfaceParam.TabIndex = 6;
            this.btnSetInterfaceParam.Text = "设置参数";
            this.btnSetInterfaceParam.UseVisualStyleBackColor = true;
            this.btnSetInterfaceParam.Click += new System.EventHandler(this.btnSetInterfaceParam_Click);
            // 
            // txtInterfaceHeight
            // 
            this.txtInterfaceHeight.Location = new System.Drawing.Point(69, 79);
            this.txtInterfaceHeight.Name = "txtInterfaceHeight";
            this.txtInterfaceHeight.Size = new System.Drawing.Size(121, 21);
            this.txtInterfaceHeight.TabIndex = 5;
            // 
            // txtInterfaceWidth
            // 
            this.txtInterfaceWidth.Location = new System.Drawing.Point(69, 52);
            this.txtInterfaceWidth.Name = "txtInterfaceWidth";
            this.txtInterfaceWidth.Size = new System.Drawing.Size(121, 21);
            this.txtInterfaceWidth.TabIndex = 4;
            // 
            // cmbInterfacePixelType
            // 
            this.cmbInterfacePixelType.FormattingEnabled = true;
            this.cmbInterfacePixelType.Location = new System.Drawing.Point(69, 26);
            this.cmbInterfacePixelType.Name = "cmbInterfacePixelType";
            this.cmbInterfacePixelType.Size = new System.Drawing.Size(121, 20);
            this.cmbInterfacePixelType.TabIndex = 3;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 82);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 12);
            this.label3.TabIndex = 2;
            this.label3.Text = "图像高度：";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(8, 55);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 12);
            this.label4.TabIndex = 1;
            this.label4.Text = "图像宽度：";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(8, 29);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(65, 12);
            this.label5.TabIndex = 0;
            this.label5.Text = "图像格式：";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.btnGetDeviceParam);
            this.groupBox6.Controls.Add(this.btnSetDeviceParam);
            this.groupBox6.Controls.Add(this.txtDeviceGainRaw);
            this.groupBox6.Controls.Add(this.txtDeviceExposuretime);
            this.groupBox6.Controls.Add(this.cmbDevicePixelType);
            this.groupBox6.Controls.Add(this.label6);
            this.groupBox6.Controls.Add(this.label7);
            this.groupBox6.Controls.Add(this.label8);
            this.groupBox6.Location = new System.Drawing.Point(741, 595);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(200, 137);
            this.groupBox6.TabIndex = 12;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "设备参数";
            // 
            // btnGetDeviceParam
            // 
            this.btnGetDeviceParam.Location = new System.Drawing.Point(6, 106);
            this.btnGetDeviceParam.Name = "btnGetDeviceParam";
            this.btnGetDeviceParam.Size = new System.Drawing.Size(75, 23);
            this.btnGetDeviceParam.TabIndex = 7;
            this.btnGetDeviceParam.Text = "获取参数";
            this.btnGetDeviceParam.UseVisualStyleBackColor = true;
            this.btnGetDeviceParam.Click += new System.EventHandler(this.btnGetDeviceParam_Click);
            // 
            // btnSetDeviceParam
            // 
            this.btnSetDeviceParam.Location = new System.Drawing.Point(111, 106);
            this.btnSetDeviceParam.Name = "btnSetDeviceParam";
            this.btnSetDeviceParam.Size = new System.Drawing.Size(75, 23);
            this.btnSetDeviceParam.TabIndex = 6;
            this.btnSetDeviceParam.Text = "设置参数";
            this.btnSetDeviceParam.UseVisualStyleBackColor = true;
            this.btnSetDeviceParam.Click += new System.EventHandler(this.btnSetDeviceParam_Click);
            // 
            // txtDeviceGainRaw
            // 
            this.txtDeviceGainRaw.Location = new System.Drawing.Point(69, 79);
            this.txtDeviceGainRaw.Name = "txtDeviceGainRaw";
            this.txtDeviceGainRaw.Size = new System.Drawing.Size(121, 21);
            this.txtDeviceGainRaw.TabIndex = 5;
            // 
            // txtDeviceExposuretime
            // 
            this.txtDeviceExposuretime.Location = new System.Drawing.Point(69, 52);
            this.txtDeviceExposuretime.Name = "txtDeviceExposuretime";
            this.txtDeviceExposuretime.Size = new System.Drawing.Size(121, 21);
            this.txtDeviceExposuretime.TabIndex = 4;
            // 
            // cmbDevicePixelType
            // 
            this.cmbDevicePixelType.FormattingEnabled = true;
            this.cmbDevicePixelType.Location = new System.Drawing.Point(69, 26);
            this.cmbDevicePixelType.Name = "cmbDevicePixelType";
            this.cmbDevicePixelType.Size = new System.Drawing.Size(121, 20);
            this.cmbDevicePixelType.TabIndex = 3;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(32, 82);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(41, 12);
            this.label6.TabIndex = 2;
            this.label6.Text = "增益：";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(32, 55);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(41, 12);
            this.label7.TabIndex = 1;
            this.label7.Text = "曝光：";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(8, 29);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(65, 12);
            this.label8.TabIndex = 0;
            this.label8.Text = "图像格式：";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(953, 741);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.cmbDeviceList);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.cmbInterfaceList);
            this.Controls.Add(this.pbImage);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "C# SDK演示程序";
            this.Shown += new System.EventHandler(this.Form1_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.pbImage)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pbImage;
        private System.Windows.Forms.Button btnOpenInterface;
        private System.Windows.Forms.Button btnCloseInterface;
        private System.Windows.Forms.ComboBox cmbInterfaceList;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnEnumInterfaces;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RadioButton radioTrigger;
        private System.Windows.Forms.RadioButton radioContinue;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.CheckBox chbSoftTrigger;
        private System.Windows.Forms.Button btnSoftware;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnSaveBIN;
        private System.Windows.Forms.Button btnSaveBMP;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cmbDeviceList;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button btnEnumDevices;
        private System.Windows.Forms.Button btnOpenDevice;
        private System.Windows.Forms.Button btnCloseDevice;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button btnGetInterfaceParam;
        private System.Windows.Forms.Button btnSetInterfaceParam;
        private System.Windows.Forms.TextBox txtInterfaceHeight;
        private System.Windows.Forms.TextBox txtInterfaceWidth;
        private System.Windows.Forms.ComboBox cmbInterfacePixelType;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button btnGetDeviceParam;
        private System.Windows.Forms.Button btnSetDeviceParam;
        private System.Windows.Forms.TextBox txtDeviceGainRaw;
        private System.Windows.Forms.TextBox txtDeviceExposuretime;
        private System.Windows.Forms.ComboBox cmbDevicePixelType;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
    }
}

