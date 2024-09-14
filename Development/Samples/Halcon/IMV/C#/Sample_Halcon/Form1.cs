using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MVSDK_Net;
using System.Runtime.InteropServices;
using HalconDotNet;
using System.Threading;
namespace Sample_Halcon
{
    public partial class Form1 : Form
    {
        private Thread m_hReceiveThread = null;
        private HWindow m_Window = new HWindow();
        private bool m_bGrabbing = false;
        private IMVDefine.IMV_DeviceList deviceList;
        private MyCamera cam = new MyCamera();
        private IMVDefine.IMV_EPixelType type = IMVDefine.IMV_EPixelType.gvspPixelMono8;
        private IMVDefine.IMV_PixelConvertParam stPixelConvertParam = new IMVDefine.IMV_PixelConvertParam();
        public Form1()
        {
            InitializeComponent();
            DisplayWindowsInitial();
        }

        private void btnEnum_Click(object sender, EventArgs e)
        {
            deviceList = new IMVDefine.IMV_DeviceList();
            IMVDefine.IMV_EInterfaceType interfaceType = IMVDefine.IMV_EInterfaceType.interfaceTypeAll;
            cbDeviceList.Items.Clear();

            // 枚举设备
            // enum device
            int res = MyCamera.IMV_EnumDevices(ref deviceList, (uint)interfaceType);
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
                            deviceList.pDevInfo + Marshal.SizeOf(typeof(IMVDefine.IMV_DeviceInfo)) * i,
                            typeof(IMVDefine.IMV_DeviceInfo));
                if (deviceInfo.nCameraType == IMVDefine.IMV_ECameraType.typeGigeCamera)
                {
                    if (deviceInfo.cameraName != "")
                    {
                        cbDeviceList.Items.Add("GigE: " + deviceInfo.cameraName + " (" + deviceInfo.serialNumber + ")");
                    }
                    else
                    {
                        cbDeviceList.Items.Add("GigE: " + deviceInfo.manufactureInfo + " " + deviceInfo.modelName + " (" + deviceInfo.serialNumber + ")");
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
                        cbDeviceList.Items.Add("USB: " + deviceInfo.manufactureInfo + " " + deviceInfo.modelName + " (" + deviceInfo.serialNumber + ")");
                    }
                }
            }

            if (deviceList.nDevNum > 0)
            {
                cbDeviceList.SelectedIndex = 0;
            }
        }

        private void DisplayWindowsInitial()
        {           
            // 定义显示的起点和宽高 
            // Definition the width and height of the display window
            HTuple hWindowRow, hWindowColumn, hWindowWidth, hWindowHeight;

            // 设置显示窗口的起点和宽高
            // Set the width and height of the display window
            hWindowRow = 0;
            hWindowColumn = 0;
            hWindowWidth = pbImage.Width;
            hWindowHeight = pbImage.Height;

            try
            {
                HTuple hWindowID = (HTuple)pbImage.Handle;
                m_Window.OpenWindow(hWindowRow, hWindowColumn, hWindowWidth, hWindowHeight, hWindowID, "visible", "");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return;
            }

        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            if (deviceList.nDevNum <= 0)
            {
                MessageBox.Show("No device!");
                return;
            }

            // 创建设备句柄
            // Create Device Handle
            int res = cam.IMV_CreateHandle(IMVDefine.IMV_ECreateHandleMode.modeByIndex, cbDeviceList.SelectedIndex);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Create devHandle failed! ErrorCode[{0}]", res));
                return;
            }

            // 打开相机设备 
            // Connect to camera 
            res = cam.IMV_Open();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Open camera failed! ErrorCode:[{0}]", res));
                return;
            }

            // 设置连续模式
            // set AcquisitionMode Continuous
            res = cam.IMV_SetEnumFeatureSymbol("AcquisitionMode", "Continuous");
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Open camera failed! ErrorCode:[{0}]", res));
                return;
            }

            // 关闭触发模式
            // set triggermode off
            res = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "Off");
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Open camera failed! ErrorCode:[{0}]", res));
                return;
            }

            // 开始拉流 
            // Start grabbing 
            res = cam.IMV_StartGrabbing();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Start grabbing failed! ErrorCode:[{0}]", res));
                return;
            }

            m_bGrabbing = true;
            m_hReceiveThread = new Thread(GrabThread);
            m_hReceiveThread.Start();
            btnOpen.Enabled = false;
            btnClose.Enabled = true;
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            m_bGrabbing = false;

            // 停止拉流 
            // Stop grabbing 
            int res = cam.IMV_StopGrabbing();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Stop grabbing failed! ErrorCode:[{0}]", res));
                return;
            }

            //关闭相机
            //Close camera 
            res = cam.IMV_Close();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Close camera failed! ErrorCode:[{0}]", res));
                return;
            }

            // 销毁设备句柄
            // Destroy Device Handle
            res = cam.IMV_DestroyHandle();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Destroy camera failed! ErrorCode[{0}]", res));
                return;
            }

            btnOpen.Enabled = true;
            btnClose.Enabled = false;
        }
        public void HalconDisplay(HTuple hWindow, HObject Hobj, HTuple hHeight, HTuple hWidth)
        {
            try
            {
                HOperatorSet.SetPart(hWindow, 0, 0, hHeight - 1, hWidth - 1);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return;
            }
            if (hWindow == null)
            {
                return;
            }
            try
            {
                HOperatorSet.DispObj(Hobj, hWindow);
                Hobj.Dispose();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return;
            }
            return;
        }

        public void GrabThread()
        {
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_Frame frame = new IMVDefine.IMV_Frame();
            HObject Hobj = new HObject();
            IntPtr pTemp = IntPtr.Zero;
            IntPtr pConvertDstBuffer = IntPtr.Zero;
            int nConvertBufSize = 0;

            while (m_bGrabbing)
            {
                res = cam.IMV_GetFrame(ref frame, 1000);
                if (res == IMVDefine.IMV_OK)
                {
                    if (IsColorPixelFormat(frame.frameInfo.pixelFormat))
                    {
                        type = IMVDefine.IMV_EPixelType.gvspPixelRGB8;
                        if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelRGB8)
                        {
                            pTemp = frame.pData;
                        }
                        else
                        {
                            if (pConvertDstBuffer == IntPtr.Zero || nConvertBufSize < (frame.frameInfo.width * frame.frameInfo.height * 3))
                            {
                                if (pConvertDstBuffer != IntPtr.Zero)
                                {
                                    Marshal.FreeHGlobal(pConvertDstBuffer);
                                    pConvertDstBuffer = IntPtr.Zero;
                                }

                                pConvertDstBuffer = Marshal.AllocHGlobal((int)(frame.frameInfo.width * frame.frameInfo.height * 3));
                                if (pConvertDstBuffer == IntPtr.Zero)
                                {
                                    break;
                                }
                                nConvertBufSize = (int)(frame.frameInfo.width * frame.frameInfo.height * 3);
                            }

                            // 其他格式彩色图像转为RGB
                            stPixelConvertParam.nWidth = frame.frameInfo.width;
                            stPixelConvertParam.nHeight = frame.frameInfo.height;
                            stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat;
                            stPixelConvertParam.pSrcData = frame.pData;
                            stPixelConvertParam.nSrcDataLen = frame.frameInfo.size;
                            stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX;
                            stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY;
                            stPixelConvertParam.eBayerDemosaic = IMVDefine.IMV_EBayerDemosaic.demosaicNearestNeighbor;
                            stPixelConvertParam.eDstPixelFormat = IMVDefine.IMV_EPixelType.gvspPixelRGB8;
                            stPixelConvertParam.pDstBuf = pConvertDstBuffer;
                            stPixelConvertParam.nDstBufSize = frame.frameInfo.width * frame.frameInfo.height * 3;
                            res = cam.IMV_PixelConvert(ref stPixelConvertParam);
                            if (res != IMVDefine.IMV_OK)
                            {
                                break;
                            }
                            pTemp = pConvertDstBuffer;
                        }

                        try
                        {
                            HOperatorSet.GenImageInterleaved(out Hobj, (HTuple)pTemp, (HTuple)"rgb", (HTuple)frame.frameInfo.width, (HTuple)frame.frameInfo.height, -1, "byte", 0, 0, 0, 0, -1, 0);
                      
                        }
                        catch (Exception)
                        {

                            throw;
                        }
                    }
                    else if (IsMonoPixelFormat(frame.frameInfo.pixelFormat))
                    {
                        if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelMono8)
                        {
                            pTemp = frame.pData;
                        }
                        else
                        {
                            if (pConvertDstBuffer == IntPtr.Zero || nConvertBufSize < (frame.frameInfo.width * frame.frameInfo.height))
                            {
                                if (pConvertDstBuffer != IntPtr.Zero)
                                {
                                    Marshal.FreeHGlobal(pConvertDstBuffer);
                                    pConvertDstBuffer = IntPtr.Zero;
                                }

                                pConvertDstBuffer = Marshal.AllocHGlobal((int)(frame.frameInfo.width * frame.frameInfo.height));
                                if (pConvertDstBuffer == IntPtr.Zero)
                                {
                                    break;
                                }
                                nConvertBufSize = (int)(frame.frameInfo.width * frame.frameInfo.height);
                            }

                            // 其他格式Mono转为Mono8
                            stPixelConvertParam.nWidth = frame.frameInfo.width;
                            stPixelConvertParam.nHeight = frame.frameInfo.height;
                            stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat;
                            stPixelConvertParam.pSrcData = frame.pData;
                            stPixelConvertParam.nSrcDataLen = frame.frameInfo.size;
                            stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX;
                            stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY;
                            stPixelConvertParam.eBayerDemosaic = IMVDefine.IMV_EBayerDemosaic.demosaicNearestNeighbor;
                            stPixelConvertParam.eDstPixelFormat = IMVDefine.IMV_EPixelType.gvspPixelMono8;
                            stPixelConvertParam.pDstBuf = pConvertDstBuffer;
                            stPixelConvertParam.nDstBufSize = frame.frameInfo.width * frame.frameInfo.height;

                            res = cam.IMV_PixelConvert(ref stPixelConvertParam);
                            if (res != IMVDefine.IMV_OK)
                            {
                                break;
                            }
                            pTemp = pConvertDstBuffer;                          
                        }
                        try
                        {
                            HOperatorSet.GenImage1Extern(out Hobj, "byte", frame.frameInfo.width, frame.frameInfo.height, pTemp, IntPtr.Zero);
                        }
                        catch (Exception)
                        {

                            throw;
                        }
                    }
                    else
                    {
                        // 释放帧数据
                        // release frame
                        cam.IMV_ReleaseFrame(ref frame);
                        continue;
                    }

                    HalconDisplay(m_Window, Hobj, frame.frameInfo.height, frame.frameInfo.width);
                    // 释放帧数据
                    // release frame
                    cam.IMV_ReleaseFrame(ref frame);
                }
            }

            if (pConvertDstBuffer != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pConvertDstBuffer);
                pConvertDstBuffer = IntPtr.Zero;
            }
        }

        /// <summary>
        /// 图像是否为Mono格式
        /// </summary>
        /// <param name="enType"></param>
        /// <returns></returns>
        private bool IsMonoPixelFormat(IMVDefine.IMV_EPixelType enType)
        {
            switch (enType)
            {
                case IMVDefine.IMV_EPixelType.gvspPixelMono8:
                case IMVDefine.IMV_EPixelType.gvspPixelMono10:
                case IMVDefine.IMV_EPixelType.gvspPixelMono10Packed:
                case IMVDefine.IMV_EPixelType.gvspPixelMono12:
                case IMVDefine.IMV_EPixelType.gvspPixelMono12Packed:
                    return true;
                default:
                    return false;
            }
        }

        /// <summary>
        /// 图像是否为彩色
        /// </summary>
        /// <param name="enType"></param>
        /// <returns></returns>
        private bool IsColorPixelFormat(IMVDefine.IMV_EPixelType enType)
        {
            switch (enType)
            {
                case IMVDefine.IMV_EPixelType.gvspPixelRGB8:
                case IMVDefine.IMV_EPixelType.gvspPixelBGR8:
                case IMVDefine.IMV_EPixelType.gvspPixelRGBA8:
                case IMVDefine.IMV_EPixelType.gvspPixelBGRA8:
                case IMVDefine.IMV_EPixelType.gvspPixelYUV422_8:
                case IMVDefine.IMV_EPixelType.gvspPixelYUV422_8_UYVY:
                case IMVDefine.IMV_EPixelType.gvspPixelBayGR8:
                case IMVDefine.IMV_EPixelType.gvspPixelBayRG8:
                case IMVDefine.IMV_EPixelType.gvspPixelBayGB8:
                case IMVDefine.IMV_EPixelType.gvspPixelBayBG8:
                case IMVDefine.IMV_EPixelType.gvspPixelBayGB10:
                case IMVDefine.IMV_EPixelType.gvspPixelBayGB10Packed:
                case IMVDefine.IMV_EPixelType.gvspPixelBayBG10:
                case IMVDefine.IMV_EPixelType.gvspPixelBayBG10Packed:
                case IMVDefine.IMV_EPixelType.gvspPixelBayRG10:
                case IMVDefine.IMV_EPixelType.gvspPixelBayRG10Packed:
                case IMVDefine.IMV_EPixelType.gvspPixelBayGR10:
                case IMVDefine.IMV_EPixelType.gvspPixelBayGR10Packed:
                case IMVDefine.IMV_EPixelType.gvspPixelBayGB12:
                case IMVDefine.IMV_EPixelType.gvspPixelBayGB12Packed:
                case IMVDefine.IMV_EPixelType.gvspPixelBayBG12:
                case IMVDefine.IMV_EPixelType.gvspPixelBayBG12Packed:
                case IMVDefine.IMV_EPixelType.gvspPixelBayRG12:
                case IMVDefine.IMV_EPixelType.gvspPixelBayRG12Packed:
                case IMVDefine.IMV_EPixelType.gvspPixelBayGR12:
                case IMVDefine.IMV_EPixelType.gvspPixelBayGR12Packed:
                    return true;
                default:
                    return false;
            }
        }

     
    }
}
