
//**********************************************************************
// 本Demo为简单演示主动取图的使用方法，主要是可直接拿到驱动中的缓存数据，减少帧数据的内存操作，但使用完帧数据后必须得调用IMV_FG_ReleaseFrame()。
// 主动取图应用的注意事项：
// 适用于低帧率和高分辨率的情况。如果帧率高，可能会发生帧覆盖的现象。
//（如已获取到blockID为2的帧，但blockID为1的帧还未调用IMV_FG_ReleaseFrame()，类似场景主动取图并不适合）。
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
using CaptureCard_Net;

namespace Grab
{
    public partial class Form1 : Form
    {
        private Thread m_hReceiveThread = null; // 取流线程 | image grab thread
        private bool m_bStartGrab = false; // 取流控制变量 | grab flag      
        private IMVFGDefine.IMV_FG_Frame m_frame;
        private Render m_Render;
        private IntPtr m_pDstData = IntPtr.Zero;
        private int m_iDstDataSize = 0;
        private CardDev card = new CardDev();
        private CamDev camera = new CamDev();
        private int res = IMVFGDefine.IMV_FG_OK;

        private IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST m_interfaceList = new IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST();
        private IMVFGDefine.IMV_FG_EInterfaceType interfaceTp = IMVFGDefine.IMV_FG_EInterfaceType.typeCLInterface|IMVFGDefine.IMV_FG_EInterfaceType.typeCXPInterface;
        private IMVFGDefine.IMV_FG_DEVICE_INFO_LIST m_deviceList = new IMVFGDefine.IMV_FG_DEVICE_INFO_LIST();

        public Form1()
        {
            InitializeComponent();
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

        //转码函数
        //transcoding function
        private bool ConvertToBGR24(IMVFGDefine.IMV_FG_Frame frame)
        {
            IMVFGDefine.IMV_FG_PixelConvertParam stPixelConvertParam = new IMVFGDefine.IMV_FG_PixelConvertParam();

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
            stPixelConvertParam.eBayerDemosaic = IMVFGDefine.IMV_FG_EBayerDemosaic.IMV_FG_DEMOSAIC_BILINEAR;
            stPixelConvertParam.eDstPixelFormat = IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8;
            stPixelConvertParam.pDstBuf = m_pDstData;
            stPixelConvertParam.nDstBufSize = (uint) m_iDstDataSize;

            res = card.IMV_FG_PixelConvert(ref stPixelConvertParam);
            if (res == IMVFGDefine.IMV_FG_OK) return true;
            Console.WriteLine("image convert to BGR8 failed! ErrorCode[{0}]", res);
            return false;
        }


        private void btnClose_Click(object sender, EventArgs e)
        {
            m_bStartGrab = false;

            if (m_hReceiveThread != null)
                m_hReceiveThread.Join();
            if (m_pDstData != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(m_pDstData);
                m_pDstData = IntPtr.Zero;
            }

            // 停止拉流 
            // Stop grabbing 
            res = card.IMV_FG_StopGrabbing();
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Stop grabbing failed!");
                return;
            }

            //关闭相机
            //Close camera 
            res = camera.IMV_FG_CloseDevice();
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Close camera failed!");
                return;
            }

            // 关闭采集卡
            // Close capture device 
            res = card.IMV_FG_CloseInterface();
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Close cameralink board failed!");
                return;
            }

            btnOpen.Enabled = true;
            btnClose.Enabled = false;
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {

            // 打开采集卡设备
            // Open capture device 
            res = card.IMV_FG_OpenInterface((uint) cmbInterfaceList.SelectedIndex);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Open cameralink capture board device failed!");
                return;
            }

            // 打开采集卡相机设备 
            // Connect to camera 
            res = camera.IMV_FG_OpenDevice(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_INDEX,
                cmbDeviceList.SelectedIndex);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Open camera failed!");
                return;
            }
			
			res = camera.IMV_FG_SetEnumFeatureSymbol("TriggerMode", "Off");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Set TriggerMode failed!");
                return;
            }

            // 开始拉流 
            // Start grabbing
            res = card.IMV_FG_StartGrabbing();
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Start grabbing failed!");
                return;
            }
            m_bStartGrab = true;
            btnOpen.Enabled = false;
            btnClose.Enabled = true;
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
                res = card.IMV_FG_GetFrame(ref m_frame, 1000);
                if (res == IMVFGDefine.IMV_FG_OK)
                {
                    if (m_frame.frameInfo.pixelFormat == IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8)
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

                    // 释放图像
                    // Release frame
                    card.IMV_FG_ReleaseFrame(ref m_frame);
                }
                else
                    Console.WriteLine("IMV_FG_GetFrame failed, res[{0}]", res);
            }
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            btnOpen.Enabled = false;
            btnClose.Enabled = false;
            m_Render = new Render(pbImage.Handle);
            m_Render.Open();
            btn_enum_Click(sender, e);
        }

        private void btn_enum_Click(object sender, EventArgs e)
        {
            btn_enum.Enabled = false;
            cmbInterfaceList.Items.Clear();
            cmbDeviceList.Items.Clear();

            //枚举采集卡设备
            // Discover capture board device
            res = CardDev.IMV_FG_EnumInterface((uint) interfaceTp, ref m_interfaceList);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Enumeration devices failed!");
                return;
            }
            if (m_interfaceList.nInterfaceNum == 0)
            {
                MessageBox.Show("No board device find.");
                return;
            }

            // 添加采集卡列表
            // Add Interface List
            for (int i = 0; i < m_interfaceList.nInterfaceNum; i++)
            {
                IMVFGDefine.IMV_FG_INTERFACE_INFO interfaceinfo =
                    (IMVFGDefine.IMV_FG_INTERFACE_INFO)
                        Marshal.PtrToStructure(
                            m_interfaceList.pInterfaceInfoList +
                            Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_INTERFACE_INFO))*i,
                            typeof (IMVFGDefine.IMV_FG_INTERFACE_INFO));
                if (interfaceinfo.interfaceName != "")
                {
                    cmbInterfaceList.Items.Add(interfaceinfo.interfaceName + " [" + interfaceinfo.serialNumber + "]");
                }
                else
                {
                    cmbInterfaceList.Items.Add(interfaceinfo.modelName + " [" + interfaceinfo.serialNumber + "]");
                }
            }

            if (cmbInterfaceList.Items.Count > 0)
                cmbInterfaceList.SelectedIndex = 0;

            //枚举相机设备
            // discover camera 
            res = CamDev.IMV_FG_EnumDevices((uint) interfaceTp, ref m_deviceList);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Enumeration camera devices failed!");
                return;
            }
            if (m_deviceList.nDevNum == 0)
            {
                MessageBox.Show("No camera device find.");
                return;
            }

            // 添加设备列表
            // Add Device List
            for (int i = 0; i < m_deviceList.nDevNum; i++)
            {
                IMVFGDefine.IMV_FG_DEVICE_INFO deviceInfo =
                    (IMVFGDefine.IMV_FG_DEVICE_INFO)
                        Marshal.PtrToStructure(
                            m_deviceList.pDeviceInfoList +
                            Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_DEVICE_INFO))*i,
                            typeof (IMVFGDefine.IMV_FG_DEVICE_INFO));

                if (deviceInfo.cameraName != "")
                {
                    cmbDeviceList.Items.Add(deviceInfo.cameraName + " [" + deviceInfo.serialNumber + "]");
                }
                else
                {
                    cmbDeviceList.Items.Add(deviceInfo.modelName + " [" + deviceInfo.serialNumber + "]");
                }

            }

            //选择第一项
            //select the first item
            if (cmbDeviceList.Items.Count > 0)
            {
                cmbDeviceList.SelectedIndex = 0;
            }

            btnOpen.Enabled = true;
            btnClose.Enabled = false;
            btn_enum.Enabled = true;
        }

        private void cmbInterfaceList_SelectedIndexChanged(object sender, EventArgs e)
        {
            int boardSelectIndex = cmbInterfaceList.SelectedIndex;
            IMVFGDefine.IMV_FG_INTERFACE_INFO interfaceInfo =
                (IMVFGDefine.IMV_FG_INTERFACE_INFO)
                    Marshal.PtrToStructure(
                        m_interfaceList.pInterfaceInfoList +
                        Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_INTERFACE_INFO))*boardSelectIndex,
                        typeof (IMVFGDefine.IMV_FG_INTERFACE_INFO));

            for (int i = 0; i < m_deviceList.nDevNum; i++)
            {
                IMVFGDefine.IMV_FG_DEVICE_INFO deviceInfo =
                    (IMVFGDefine.IMV_FG_DEVICE_INFO)
                        Marshal.PtrToStructure(
                            m_deviceList.pDeviceInfoList +
                            Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_DEVICE_INFO))*i,
                            typeof (IMVFGDefine.IMV_FG_DEVICE_INFO));
                if (deviceInfo.FGInterfaceInfo.interfaceKey.Equals(interfaceInfo.interfaceKey))
                {
                    cmbDeviceList.SelectedIndexChanged -= cmbDeviceList_SelectedIndexChanged;
                    cmbDeviceList.SelectedIndex = i;
                    cmbDeviceList.SelectedIndexChanged += cmbDeviceList_SelectedIndexChanged;
                    break;
                }
            }
        }

        private void cmbDeviceList_SelectedIndexChanged(object sender, EventArgs e)
        {
            int cameraSelectIndex = cmbDeviceList.SelectedIndex;
            IMVFGDefine.IMV_FG_DEVICE_INFO deviceInfo =
                (IMVFGDefine.IMV_FG_DEVICE_INFO)
                    Marshal.PtrToStructure(
                        m_deviceList.pDeviceInfoList +
                        Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_DEVICE_INFO))*cameraSelectIndex,
                        typeof (IMVFGDefine.IMV_FG_DEVICE_INFO));

            for (int i = 0; i < m_interfaceList.nInterfaceNum; i++)
            {
                IMVFGDefine.IMV_FG_INTERFACE_INFO interfaceInfo =
                    (IMVFGDefine.IMV_FG_INTERFACE_INFO)
                        Marshal.PtrToStructure(
                            m_interfaceList.pInterfaceInfoList +
                            Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_INTERFACE_INFO))*i,
                            typeof (IMVFGDefine.IMV_FG_INTERFACE_INFO));
                if (interfaceInfo.interfaceKey.Equals(deviceInfo.FGInterfaceInfo.interfaceKey))
                {
                    cmbInterfaceList.SelectedValueChanged -= cmbInterfaceList_SelectedIndexChanged;
                    cmbInterfaceList.SelectedIndex = i;
                    cmbInterfaceList.SelectedValueChanged += cmbInterfaceList_SelectedIndexChanged;
                    break;
                }
            }
        }
    }
}
