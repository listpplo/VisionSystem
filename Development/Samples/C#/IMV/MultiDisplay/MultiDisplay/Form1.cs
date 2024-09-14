using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Threading;
using System.Diagnostics;
using System.Runtime.InteropServices;
using MVSDK_Net;

namespace MultiDisplay
{
    public partial class Form1 : Form
    {
        private List<PictureBox> pictureboxList = new List<PictureBox>(); //PictureBox列表
        private Camera[] cameraList; // 相机数组
        private const int CameraNumMax = 4; // 最大支持相机个数
        private int findCameraNum = 0; // 发现的相机个数

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            btnClose.Enabled = false;

            pictureboxList.Add(pbImage1);
            pictureboxList.Add(pbImage2);
            pictureboxList.Add(pbImage3);
            pictureboxList.Add(pbImage4);

            cameraList = new Camera[CameraNumMax];
            for (int i = 0; i < CameraNumMax; i++)
            {
                Render render = new Render(pictureboxList[i].Handle);
                cameraList[i] = new Camera(render);
            }

        }


        private void btnOpen_Click(object sender, EventArgs e)
        {
            //枚举设备
            // Discover device
            IMVDefine.IMV_DeviceList deviceList = new IMVDefine.IMV_DeviceList();
            IMVDefine.IMV_EInterfaceType interfaceTp = IMVDefine.IMV_EInterfaceType.interfaceTypeAll;
            int res = MyCamera.IMV_EnumDevices(ref deviceList, (uint) interfaceTp);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Enumeration devices failed! ErrorCode:[{0}]", res));
                return;
            }
            if (deviceList.nDevNum == 0)
            {
                MessageBox.Show(string.Format("No device find. ErrorCode:[{0}]", res));
                return;
            }

            findCameraNum = (int) deviceList.nDevNum;
            for (int i = 0; i < CameraNumMax; i++)
            {
                if (i < findCameraNum)
                {
                    if (cameraList[i].Open(i))
                    {
                        btnOpen.Enabled = false;
                        btnClose.Enabled = true;
                    }
                }
            }
        }

        // 停止码流 
        // stop grabbing 
        private void btnClose_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < CameraNumMax; i++)
            {
                if (i < findCameraNum)
                {
                    if (cameraList[i].Close())
                    {
                        btnOpen.Enabled = true;
                        btnClose.Enabled = false;
                    }
                }
            }
        }

        // 窗口关闭 
        // Window Closed 
        protected override void OnClosed(EventArgs e)
        {
            for (int i = 0; i < CameraNumMax; i++)
            {
                cameraList[i].Dispose();
            }
            base.OnClosed(e);
        }

    }
}
