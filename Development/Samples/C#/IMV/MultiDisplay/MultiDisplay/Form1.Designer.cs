namespace MultiDisplay
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
            this.pbImage1 = new System.Windows.Forms.PictureBox();
            this.btnOpen = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.pbImage2 = new System.Windows.Forms.PictureBox();
            this.pbImage4 = new System.Windows.Forms.PictureBox();
            this.pbImage3 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pbImage1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbImage2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbImage4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbImage3)).BeginInit();
            this.SuspendLayout();
            // 
            // pbImage1
            // 
            this.pbImage1.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.pbImage1.Location = new System.Drawing.Point(12, 68);
            this.pbImage1.Name = "pbImage1";
            this.pbImage1.Size = new System.Drawing.Size(368, 245);
            this.pbImage1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbImage1.TabIndex = 0;
            this.pbImage1.TabStop = false;
            // 
            // btnOpen
            // 
            this.btnOpen.Location = new System.Drawing.Point(86, 13);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(75, 23);
            this.btnOpen.TabIndex = 1;
            this.btnOpen.Text = "Open";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(321, 13);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 2;
            this.btnClose.Text = "Close";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // pbImage2
            // 
            this.pbImage2.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.pbImage2.Location = new System.Drawing.Point(386, 68);
            this.pbImage2.Name = "pbImage2";
            this.pbImage2.Size = new System.Drawing.Size(368, 245);
            this.pbImage2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbImage2.TabIndex = 3;
            this.pbImage2.TabStop = false;
            // 
            // pbImage4
            // 
            this.pbImage4.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.pbImage4.Location = new System.Drawing.Point(385, 319);
            this.pbImage4.Name = "pbImage4";
            this.pbImage4.Size = new System.Drawing.Size(368, 245);
            this.pbImage4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbImage4.TabIndex = 5;
            this.pbImage4.TabStop = false;
            // 
            // pbImage3
            // 
            this.pbImage3.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.pbImage3.Location = new System.Drawing.Point(11, 319);
            this.pbImage3.Name = "pbImage3";
            this.pbImage3.Size = new System.Drawing.Size(368, 245);
            this.pbImage3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbImage3.TabIndex = 4;
            this.pbImage3.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(765, 572);
            this.Controls.Add(this.pbImage4);
            this.Controls.Add(this.pbImage3);
            this.Controls.Add(this.pbImage2);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.btnOpen);
            this.Controls.Add(this.pbImage1);
            this.Name = "Form1";
            this.Text = "MultiDisplay";
            this.Shown += new System.EventHandler(this.Form1_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.pbImage1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbImage2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbImage4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbImage3)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pbImage1;
        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.PictureBox pbImage2;
        private System.Windows.Forms.PictureBox pbImage4;
        private System.Windows.Forms.PictureBox pbImage3;
    }
}

