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
            this.btnOpen = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.cbDeviceInfos = new System.Windows.Forms.ComboBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnEnumDevices = new System.Windows.Forms.Button();
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
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.btnGetParam = new System.Windows.Forms.Button();
            this.btnSetParam = new System.Windows.Forms.Button();
            this.txtGainRaw = new System.Windows.Forms.TextBox();
            this.txtExposuretime = new System.Windows.Forms.TextBox();
            this.cbPixelType = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pbImage)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // pbImage
            // 
            this.pbImage.Location = new System.Drawing.Point(12, 63);
            this.pbImage.Name = "pbImage";
            this.pbImage.Size = new System.Drawing.Size(547, 497);
            this.pbImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbImage.TabIndex = 0;
            this.pbImage.TabStop = false;
            // 
            // btnOpen
            // 
            this.btnOpen.Location = new System.Drawing.Point(6, 59);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(75, 23);
            this.btnOpen.TabIndex = 1;
            this.btnOpen.Text = "打开设备";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(119, 59);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 2;
            this.btnClose.Text = "关闭设备";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // cbDeviceInfos
            // 
            this.cbDeviceInfos.FormattingEnabled = true;
            this.cbDeviceInfos.Location = new System.Drawing.Point(13, 24);
            this.cbDeviceInfos.Name = "cbDeviceInfos";
            this.cbDeviceInfos.Size = new System.Drawing.Size(546, 20);
            this.cbDeviceInfos.TabIndex = 3;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnEnumDevices);
            this.groupBox1.Controls.Add(this.btnOpen);
            this.groupBox1.Controls.Add(this.btnClose);
            this.groupBox1.Location = new System.Drawing.Point(584, 24);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(200, 100);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "初始化";
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
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnSoftware);
            this.groupBox2.Controls.Add(this.chbSoftTrigger);
            this.groupBox2.Controls.Add(this.btnStop);
            this.groupBox2.Controls.Add(this.btnStart);
            this.groupBox2.Controls.Add(this.radioTrigger);
            this.groupBox2.Controls.Add(this.radioContinue);
            this.groupBox2.Location = new System.Drawing.Point(584, 139);
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
            this.groupBox3.Location = new System.Drawing.Point(584, 299);
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
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.btnGetParam);
            this.groupBox4.Controls.Add(this.btnSetParam);
            this.groupBox4.Controls.Add(this.txtGainRaw);
            this.groupBox4.Controls.Add(this.txtExposuretime);
            this.groupBox4.Controls.Add(this.cbPixelType);
            this.groupBox4.Controls.Add(this.label3);
            this.groupBox4.Controls.Add(this.label2);
            this.groupBox4.Controls.Add(this.label1);
            this.groupBox4.Location = new System.Drawing.Point(584, 390);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(200, 170);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "参数";
            // 
            // btnGetParam
            // 
            this.btnGetParam.Location = new System.Drawing.Point(10, 132);
            this.btnGetParam.Name = "btnGetParam";
            this.btnGetParam.Size = new System.Drawing.Size(75, 23);
            this.btnGetParam.TabIndex = 7;
            this.btnGetParam.Text = "获取参数";
            this.btnGetParam.UseVisualStyleBackColor = true;
            this.btnGetParam.Click += new System.EventHandler(this.btnGetParam_Click);
            // 
            // btnSetParam
            // 
            this.btnSetParam.Location = new System.Drawing.Point(115, 132);
            this.btnSetParam.Name = "btnSetParam";
            this.btnSetParam.Size = new System.Drawing.Size(75, 23);
            this.btnSetParam.TabIndex = 6;
            this.btnSetParam.Text = "设置参数";
            this.btnSetParam.UseVisualStyleBackColor = true;
            this.btnSetParam.Click += new System.EventHandler(this.btnSetParam_Click);
            // 
            // txtGainRaw
            // 
            this.txtGainRaw.Location = new System.Drawing.Point(69, 96);
            this.txtGainRaw.Name = "txtGainRaw";
            this.txtGainRaw.Size = new System.Drawing.Size(121, 21);
            this.txtGainRaw.TabIndex = 5;
            // 
            // txtExposuretime
            // 
            this.txtExposuretime.Location = new System.Drawing.Point(69, 65);
            this.txtExposuretime.Name = "txtExposuretime";
            this.txtExposuretime.Size = new System.Drawing.Size(121, 21);
            this.txtExposuretime.TabIndex = 4;
            // 
            // cbPixelType
            // 
            this.cbPixelType.FormattingEnabled = true;
            this.cbPixelType.Location = new System.Drawing.Point(69, 26);
            this.cbPixelType.Name = "cbPixelType";
            this.cbPixelType.Size = new System.Drawing.Size(121, 20);
            this.cbPixelType.TabIndex = 3;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(32, 99);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 2;
            this.label3.Text = "增益：";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(32, 68);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "曝光：";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "图像格式：";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(795, 572);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.cbDeviceInfos);
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
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pbImage;
        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.ComboBox cbDeviceInfos;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnEnumDevices;
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
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtGainRaw;
        private System.Windows.Forms.TextBox txtExposuretime;
        private System.Windows.Forms.ComboBox cbPixelType;
        private System.Windows.Forms.Button btnSetParam;
        private System.Windows.Forms.Button btnGetParam;
    }
}

