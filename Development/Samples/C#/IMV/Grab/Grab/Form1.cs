//**********************************************************************
// 本Demo为简单演示主动取图的使用方法，主要是可直接拿到驱动中的缓存数据，减少帧数据的内存操作，但使用完帧数据后必须得调用IMV_ReleaseFrame()。
// 主动取图应用的注意事项：
// 适用于低帧率和高分辨率的情况。如果帧率高，可能会发生帧覆盖的现象。
//（如已获取到blockID为2的帧，但blockID为1的帧还未调用IMV_ReleaseFrame()，类似场景主动取图并不适合）。
//**********************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using MVSDK_Net;

namespace Grab
{
    public partial class Form1 : Form
    {
        private Thread m_hReceiveThread = null; // 取流线程 | image grab thread  
        private bool m_bStartGrab = false; // 取流控制变量 | grab flag     

        private IMVDefine.IMV_Frame m_frame;
        private MyCamera cam = new MyCamera();
        private int res = IMVDefine.IMV_OK;
        private Render m_Render;
        private IntPtr m_pDstData = IntPtr.Zero;
        private int m_iDstDataSize = 0;


        public Form1()
        {
            InitializeComponent();
        }

        //转码函数
        //transcoding function
        private bool ConvertToBGR24(IMVDefine.IMV_Frame frame)
        {
            IMVDefine.IMV_PixelConvertParam stPixelConvertParam = new IMVDefine.IMV_PixelConvertParam();

            //当内存申请失败，返回false
            try
            {
                if (m_pDstData == IntPtr.Zero || (int) frame.frameInfo.size*3 > m_iDstDataSize)
                {
                    if (m_pDstData != IntPtr.Zero)
                    {
                        Marshal.FreeHGlobal(m_pDstData);
                    }
                    m_pDstData = Marshal.AllocHGlobal((int) frame.frameInfo.size*3);
                    m_iDstDataSize = (int) frame.frameInfo.size*3;
                }
            }
            catch
            {
                return false;
            }

            // 图像转换成BGR8
            // convert image to BGR8
            stPixelConvertParam.nWidth = frame.frameInfo.width;
            stPixelConvertParam.nHeight = frame.frameInfo.height;
            stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat;
            stPixelConvertParam.pSrcData = frame.pData;
            stPixelConvertParam.nSrcDataLen = frame.frameInfo.size;
            stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX;
            stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY;
            stPixelConvertParam.eBayerDemosaic = IMVDefine.IMV_EBayerDemosaic.demosaicBilinear;
            stPixelConvertParam.eDstPixelFormat = IMVDefine.IMV_EPixelType.gvspPixelBGR8;
            stPixelConvertParam.pDstBuf = m_pDstData;
            stPixelConvertParam.nDstBufSize = (uint) m_iDstDataSize;

            res = cam.IMV_PixelConvert(ref stPixelConvertParam);
            if (res == IMVDefine.IMV_OK) return true;
            Console.WriteLine("image convert to BGR8 failed! ErrorCode[{0}]", res);
            return false;
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            m_bStartGrab = false;
            if (m_hReceiveThread != null)
                m_hReceiveThread.Join();

            // 停止拉流 
            // Stop grabbing 
            res = cam.IMV_StopGrabbing();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Stop grabbing failed! ErrorCode[{0}]", res));
                return;
            }

            if (m_pDstData != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(m_pDstData);
                m_pDstData = IntPtr.Zero;
            }

            btnOpen.Enabled = true;
            btnClose.Enabled = false;

            //关闭相机
            //Close cam 
            res = cam.IMV_Close();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Close camera failed! ErrorCode[{0}]", res));
                return;
            }

            // 销毁设备句柄
            // Destroy Device Handle
            res = cam.IMV_DestroyHandle();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Destroy camera failed! ErrorCode[{0}]", res));
            }
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {

            // 创建设备句柄
            // Create Device Handle
            res = cam.IMV_CreateHandle(IMVDefine.IMV_ECreateHandleMode.modeByIndex, cbDeviceList.SelectedIndex);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Create devHandle failed! ErrorCode[{0}]", res));
                return;
            }

            // 打开相机设备 
            // Connect to camera 
            res = cam.IMV_Open();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Open camera failed! ErrorCode:[{0}]", res));
                return;
            }

            //设置缓存个数为8
            //set buffer count to 8
            res = cam.IMV_SetBufferCount(8);

            // 开始拉流 
            // Start grabbing
            res = cam.IMV_StartGrabbing();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Start grabbing failed! ErrorCode[{0}]", res));
                return;
            }

            btnOpen.Enabled = false;
            btnClose.Enabled = true;
            m_bStartGrab = true;
            m_hReceiveThread = new Thread(GrabThread);
            m_hReceiveThread.Start();
        }

        // 主动取流线程 
        // grab thread 
        private void GrabThread()
        {
            while (m_bStartGrab)
            {
                // 获取一帧图像
                // Get a frame image 
                res = cam.IMV_GetFrame(ref m_frame, 1000);
                if (res == IMVDefine.IMV_OK)
                {
                    if (m_frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelMono8)
                    {
                        m_Render.Display(m_frame.pData, (int) m_frame.frameInfo.width, (int) m_frame.frameInfo.height,
                            Render.VR_PIXEL_TYPE_E.VR_PIXEL_FMT_MONO8);
                    }
                    else
                    {
                        if (ConvertToBGR24(m_frame))
                            m_Render.Display(m_pDstData, (int) m_frame.frameInfo.width, (int) m_frame.frameInfo.height,
                                Render.VR_PIXEL_TYPE_E.VR_PIXEL_FMT_RGB24);
                    }

                    // 释放一帧图像
                    // Release a frame
                    cam.IMV_ReleaseFrame(ref m_frame);
                }
                else
                    Console.WriteLine("IMV_GetFrame failed, res[{0}]", res);
            }

        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            m_Render = new Render(pbImage.Handle);
            m_Render.Open();

            btn_enum_Click(sender, e);
        }

        protected override void OnClosed(EventArgs e)
        {
            if (m_bStartGrab)
            {
                btnClose_Click(null, null);
            }

            m_Render.Close();
            m_Render = null;
            base.OnClosed(e);
        }

        private void btn_enum_Click(object sender, EventArgs e)
        {
            IMVDefine.IMV_DeviceList deviceList = new IMVDefine.IMV_DeviceList();
            IMVDefine.IMV_EInterfaceType interfaceType = IMVDefine.IMV_EInterfaceType.interfaceTypeAll;
            cbDeviceList.Items.Clear();

            // 枚举设备
            // enum device
            int res = MyCamera.IMV_EnumDevices(ref deviceList, (uint) interfaceType);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Enum Devices Fail");
                return;
            }

            // 显示设备信息
            // Display device infomation
            for (int i = 0; i < deviceList.nDevNum; i++)
            {
                IMVDefine.IMV_DeviceInfo deviceInfo =
                    (IMVDefine.IMV_DeviceInfo)
                        Marshal.PtrToStructure(
                            deviceList.pDevInfo + Marshal.SizeOf(typeof (IMVDefine.IMV_DeviceInfo))*i,
                            typeof (IMVDefine.IMV_DeviceInfo));
                if (deviceInfo.nCameraType == IMVDefine.IMV_ECameraType.typeGigeCamera)
                {
                    if (deviceInfo.cameraName != "")
                    {
                        cbDeviceList.Items.Add("GigE: " + deviceInfo.cameraName + " (" + deviceInfo.serialNumber + ")");
                    }
                    else
                    {
                        cbDeviceList.Items.Add("GigE: " + deviceInfo.manufactureInfo + " " + deviceInfo.modelName + " (" +
                                               deviceInfo.serialNumber + ")");
                    }
                }
                else if (deviceInfo.nCameraType == IMVDefine.IMV_ECameraType.typeU3vCamera)
                {
                    if (deviceInfo.cameraName != "")
                    {
                        cbDeviceList.Items.Add("USB: " + deviceInfo.cameraName + " (" + deviceInfo.serialNumber + ")");
                    }
                    else
                    {
                        cbDeviceList.Items.Add("USB: " + deviceInfo.manufactureInfo + " " + deviceInfo.modelName + " (" +
                                               deviceInfo.serialNumber + ")");
                    }
                }
            }

            if (deviceList.nDevNum > 0)
            {
                cbDeviceList.SelectedIndex = 0;
            }
        }
    }
}
