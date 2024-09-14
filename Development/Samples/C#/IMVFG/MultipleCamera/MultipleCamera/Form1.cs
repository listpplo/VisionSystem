//**********************************************************************
// 本Demo为简单演示多相机取图的使用方法
// 只支持打开采集卡上具有相机的一对设备进行拉流 
// This demo is a simple demonstration of how to use multiple cameras to capture images. 
// Only supports opening a pair of devices with cameras on the acquisition card for streaming
//**********************************************************************


using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CaptureCard_Net;


namespace MultipleCamera
{
    public partial class Form1 : Form
    {
        private List<PictureBox> pictureboxList = new List<PictureBox>(); //PictureBox列表
        private Device[] cameraList; // 设备数组
        private const int CameraNumMax = 4; // 最大支持相机个数
        private int findCameraNum = 0; // 发现的相机个数
        private Dictionary<string, string> cameraKeyofCardAndCamera=new Dictionary<string, string>(); //相机和卡的CameraKey对应关系
        public Form1()
        {
            InitializeComponent();
        }

        // 打开卡和相机
        // Open card and camera
        private void btnOpen_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < CameraNumMax; i++)
            {
                if (i < findCameraNum)
                {
                    if (cameraList[i].Open(cameraKeyofCardAndCamera.ElementAt(i).Key,cameraKeyofCardAndCamera.ElementAt(i).Value))
                    {
                        btnOpen.Enabled = false;
                        btnClose.Enabled = true;
                        btn_enum.Enabled = false;
                    }
                }
            }
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            btnClose.Enabled = false;

            pictureboxList.Add(pbImage1);
            pictureboxList.Add(pbImage2);
            pictureboxList.Add(pbImage3);
            pictureboxList.Add(pbImage4);

            cameraList = new Device[CameraNumMax];
            for (int i = 0; i < CameraNumMax; i++)
            {
                Render render = new Render(pictureboxList[i].Handle);
                cameraList[i] = new Device(render);
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

        // 停止码流并且关闭相机和卡 
        // stop grabbing and close camera and card
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
                        btn_enum.Enabled = true;
                    }
                }
            }
        }

        // 枚举卡和相机
        // Enumerate card and camera
        private void btn_enum_Click(object sender, EventArgs e)
        {
            cameraKeyofCardAndCamera.Clear();
            btnOpen.Enabled = false;
            int res = IMVFGDefine.IMV_FG_OK;
            IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST interfaceList = new IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST();
            IMVFGDefine.IMV_FG_EInterfaceType interfaceTp = IMVFGDefine.IMV_FG_EInterfaceType.typeCLInterface;
            IMVFGDefine.IMV_FG_DEVICE_INFO_LIST camList = new IMVFGDefine.IMV_FG_DEVICE_INFO_LIST();
            IMVFGDefine.IMV_FG_INTERFACE_INFO[] interfaceInfo;
            IMVFGDefine.IMV_FG_DEVICE_INFO[] deviceInfo;

            // 枚举采集卡设备
            // Discover capture board device
            res = CardDev.IMV_FG_EnumInterface((uint)interfaceTp, ref interfaceList);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Enumeration devices failed!errorCode:[{0}]", res));
                return;
            }
            if (interfaceList.nInterfaceNum == 0)
            {
                MessageBox.Show(string.Format("No board device find.errorCode:[{0}]", res));
                return;
            }

            // 枚举相机设备
            // discover camera 
            res = CamDev.IMV_FG_EnumDevices((uint)interfaceTp, ref camList);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Enumeration camera devices failed!errorCode:[{0}]", res));
                return;
            }
            if (camList.nDevNum == 0)
            {
                MessageBox.Show(string.Format("No camera device find.errorCode:[{0}]", res));
                return;
            }

            // 获得采集卡的信息
            interfaceInfo =
               new IMVFGDefine.IMV_FG_INTERFACE_INFO[interfaceList.nInterfaceNum];
            for (int i = 0; i < interfaceList.nInterfaceNum; i++)
            {
                //capture interface info
                //采集卡接口信息
                interfaceInfo[i] =
                    (IMVFGDefine.IMV_FG_INTERFACE_INFO)
                        Marshal.PtrToStructure(
                            interfaceList.pInterfaceInfoList +
                            Marshal.SizeOf(typeof(IMVFGDefine.IMV_FG_INTERFACE_INFO)) * i,
                            typeof(IMVFGDefine.IMV_FG_INTERFACE_INFO));
            }

            // 获得相机的信息
            deviceInfo = new IMVFGDefine.IMV_FG_DEVICE_INFO[camList.nDevNum];
            for (int j = 0; j < camList.nDevNum; j++)
            {
                deviceInfo[j] =
                    (IMVFGDefine.IMV_FG_DEVICE_INFO)
                        Marshal.PtrToStructure(
                            camList.pDeviceInfoList +
                            Marshal.SizeOf(typeof(IMVFGDefine.IMV_FG_DEVICE_INFO)) * j,
                            typeof(IMVFGDefine.IMV_FG_DEVICE_INFO));
            }
            findCameraNum = (int)camList.nDevNum;

            for (int i = 0; i < deviceInfo.Length; i++)
            {
                ((Label)this.Controls.Find(string.Format("label{0}", 1 + i * 2), false)[0]).Text = "Device: " + deviceInfo[i].cameraName + " " +
                                            deviceInfo[i].modelName + " " +
                                            deviceInfo[i].serialNumber;

                // 根据相机信息寻找匹配的卡信息
                // Search for matching card information based on camera information
                for (int j = 0; j < interfaceInfo.Length; j++)
                {
                    if (deviceInfo[i].FGInterfaceInfo.interfaceKey == interfaceInfo[j].interfaceKey)
                    {
                        cameraKeyofCardAndCamera.Add(interfaceInfo[j].interfaceKey, deviceInfo[i].cameraKey);
                        ((Label)this.Controls.Find(string.Format("label{0}", 2 + i * 2), false)[0]).Text = "Interface: " + interfaceInfo[j].interfaceName + " " +
                                            interfaceInfo[j].interfaceKey;
                        break;
                    }

                }
            }
            btnOpen.Enabled = true;
            btnClose.Enabled = false;
        }
    }
}
