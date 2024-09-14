namespace Samples_VisionPro
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.cogDisplayImage = new Cognex.VisionPro.Display.CogDisplay();
            this.btnOpen = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.cbDeviceList = new System.Windows.Forms.ComboBox();
            this.bnEnum = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.cogDisplayImage)).BeginInit();
            this.SuspendLayout();
            // 
            // cogDisplayImage
            // 
            resources.ApplyResources(this.cogDisplayImage, "cogDisplayImage");
            this.cogDisplayImage.ColorMapLowerClipColor = System.Drawing.Color.Black;
            this.cogDisplayImage.ColorMapLowerRoiLimit = 0D;
            this.cogDisplayImage.ColorMapPredefined = Cognex.VisionPro.Display.CogDisplayColorMapPredefinedConstants.None;
            this.cogDisplayImage.ColorMapUpperClipColor = System.Drawing.Color.Black;
            this.cogDisplayImage.ColorMapUpperRoiLimit = 1D;
            this.cogDisplayImage.DoubleTapZoomCycleLength = 2;
            this.cogDisplayImage.DoubleTapZoomSensitivity = 2.5D;
            this.cogDisplayImage.MouseWheelMode = Cognex.VisionPro.Display.CogDisplayMouseWheelModeConstants.Zoom1;
            this.cogDisplayImage.MouseWheelSensitivity = 1D;
            this.cogDisplayImage.Name = "cogDisplayImage";
            this.cogDisplayImage.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("cogDisplayImage.OcxState")));
            // 
            // btnOpen
            // 
            resources.ApplyResources(this.btnOpen, "btnOpen");
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // btnClose
            // 
            resources.ApplyResources(this.btnClose, "btnClose");
            this.btnClose.Name = "btnClose";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // cbDeviceList
            // 
            this.cbDeviceList.FormattingEnabled = true;
            resources.ApplyResources(this.cbDeviceList, "cbDeviceList");
            this.cbDeviceList.Name = "cbDeviceList";
            // 
            // bnEnum
            // 
            resources.ApplyResources(this.bnEnum, "bnEnum");
            this.bnEnum.Name = "bnEnum";
            this.bnEnum.UseVisualStyleBackColor = true;
            this.bnEnum.Click += new System.EventHandler(this.bnEnum_Click);
            // 
            // Form1
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.btnOpen);
            this.Controls.Add(this.bnEnum);
            this.Controls.Add(this.cbDeviceList);
            this.Controls.Add(this.cogDisplayImage);
            this.Name = "Form1";
            this.Shown += new System.EventHandler(this.Form1_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.cogDisplayImage)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.Button btnClose;
        private Cognex.VisionPro.Display.CogDisplay cogDisplayImage;
        private System.Windows.Forms.ComboBox cbDeviceList;
        private System.Windows.Forms.Button bnEnum;
    }
}

