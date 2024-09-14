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
using Cognex.VisionPro;
using MVSDK_Net;

namespace Samples_VisionPro
{
    public partial class Form1 : Form
    {
        private Thread m_hReceiveThread = null;         
        private MyCamera cam = new MyCamera();
        private long m_picSize = 0;
        private bool m_bGrabbing = false;           
        private IMVDefine.IMV_PixelConvertParam stPixelConvertParam = new IMVDefine.IMV_PixelConvertParam();
        private IMVDefine.IMV_EPixelType type = IMVDefine.IMV_EPixelType.gvspPixelMono8;
        private IMVDefine.IMV_DeviceList deviceList;

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

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            btnClose.Enabled = false;
        }

        // 转码显示线程 
        // display thread routine 
        private void ReceiveThreadProcess()
        {
            IMVDefine.IMV_Frame frame = new IMVDefine.IMV_Frame();
            Byte[] byteArrImageData = new Byte[m_picSize * 3];
            IntPtr pTemp = IntPtr.Zero;
            IntPtr pConvertDstBuffer = IntPtr.Zero;
            int nConvertBufSize = 0;
            int res = IMVDefine.IMV_OK;

            while (m_bGrabbing)
            {
                res = cam.IMV_GetFrame(ref frame, 1000);
                if (res == IMVDefine.IMV_OK)
                {
                    try
                    {
                        // Mono图像处理
                        if (IsMonoPixelFormat(frame.frameInfo.pixelFormat))
                        {
                            type = IMVDefine.IMV_EPixelType.gvspPixelMono8;
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
                        }
                        // 彩色图像处理
                        else if (IsColorPixelFormat(frame.frameInfo.pixelFormat))
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

                            unsafe
                            {
                                byte* pBufForSaveImage = (byte*)pTemp;

                                UInt32 nSupWidth = (uint)((frame.frameInfo.width + (UInt32)3) & 0xfffffffc);

                                for (int nRow = 0; nRow < frame.frameInfo.height; nRow++)
                                {
                                    for (int col = 0; col < frame.frameInfo.width; col++)
                                    {
                                        byteArrImageData[nRow * nSupWidth + col] = pBufForSaveImage[nRow * frame.frameInfo.width * 3 + (3 * col)];
                                        byteArrImageData[frame.frameInfo.width * frame.frameInfo.height + nRow * nSupWidth + col] = pBufForSaveImage[nRow * frame.frameInfo.width * 3 + (3 * col + 1)];
                                        byteArrImageData[frame.frameInfo.width * frame.frameInfo.height * 2 + nRow * nSupWidth + col] = pBufForSaveImage[nRow * frame.frameInfo.width * 3 + (3 * col + 2)];
                                    }
                                }
                                pTemp = Marshal.UnsafeAddrOfPinnedArrayElement(byteArrImageData, 0);
                            }
                        }
                        else
                        {
                            continue;
                        }
                        VisionProDisplay(frame.frameInfo.width, frame.frameInfo.height, pTemp, type);
                    }
                    catch (Exception)
                    {
                        throw;
                    }
                    finally
                    {
                        // 释放帧数据
                        // release frame
                        cam.IMV_ReleaseFrame(ref frame);
                    }

                }
                else
                {
                    continue;
                }
            }
            if (pConvertDstBuffer != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pConvertDstBuffer);
                pConvertDstBuffer = IntPtr.Zero;
            }
        }
    

        /// <summary>
        /// 显示图片
        /// </summary>
        /// <param name="nHeight">高</param>
        /// <param name="nWidth">宽</param>
        /// <param name="pImageBuf">图片数据</param>
        /// <param name="enPixelType">像素格式</param>
        public void VisionProDisplay(UInt32 nWidth, UInt32 nHeight, IntPtr pImageBuf, IMVDefine.IMV_EPixelType enPixelType)
        {
            try
            {
                if (enPixelType == IMVDefine.IMV_EPixelType.gvspPixelMono8)
                {
                    CogImage8Root cogImage8Root = new CogImage8Root();
                    cogImage8Root.Initialize((Int32)nWidth, (Int32)nHeight, pImageBuf, (Int32)nWidth, null);

                    CogImage8Grey cogImage8Grey = new CogImage8Grey();
                    cogImage8Grey.SetRoot(cogImage8Root);
                    this.cogDisplayImage.Image = cogImage8Grey.ScaleImage((int)nWidth, (int)nHeight);
                    System.GC.Collect();
                }
                else
                {
                    CogImage8Root image0 = new CogImage8Root();
                    IntPtr ptr0 = new IntPtr(pImageBuf.ToInt64());
                    image0.Initialize((int)nWidth, (int)nHeight, ptr0, (int)nWidth, null);

                    CogImage8Root image1 = new CogImage8Root();
                    IntPtr ptr1 = new IntPtr(pImageBuf.ToInt64() + m_picSize);
                    image1.Initialize((int)nWidth, (int)nHeight, ptr1, (int)nWidth, null);

                    CogImage8Root image2 = new CogImage8Root();
                    IntPtr ptr2 = new IntPtr(pImageBuf.ToInt64() + m_picSize * 2);
                    image2.Initialize((int)nWidth, (int)nHeight, ptr2, (int)nWidth, null);

                    CogImage24PlanarColor colorImage = new CogImage24PlanarColor();
                    colorImage.SetRoots(image0, image1, image2);

                    this.cogDisplayImage.Image = colorImage.ScaleImage((int)nWidth, (int)nHeight);
                    System.GC.Collect();
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return;
            }
            return;
        }


        private void btnOpen_Click(object sender, EventArgs e)
        {
            if (deviceList.nDevNum <= 0 || cbDeviceList.SelectedIndex < 0)
            {
                MessageBox.Show("No device,please select");
                return;
            }

            int res = cam.IMV_CreateHandle(IMVDefine.IMV_ECreateHandleMode.modeByIndex, cbDeviceList.SelectedIndex);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Create devHandle failed!");
                return;
            }

            // 打开设备 
            // open device 
            res = cam.IMV_Open();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Open camera failed");
                return;
            }

            this.Invoke(new Action(() =>
            {
                btnOpen.Enabled = false;
                btnClose.Enabled = true;
            }));

            long width = 0;
            long height = 0;

            // 获取宽
            // Get Width
            res = cam.IMV_GetIntFeatureValue("Width", ref width);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Get Width failed");
                return;
            }

            // 获取高
            // Get Height
            res = cam.IMV_GetIntFeatureValue("Height", ref height);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Get Height failed");
                return;
            }

            m_picSize = width * height;

            res = cam.IMV_SetEnumFeatureSymbol("AcquisitionMode", "Continuous");
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Set AcquisitionMode failed");
                return;
            }

            // 关闭触发模式
            // set trigger mode as off     
            res = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "Off");
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Set TriggerMode failed");
                return;
            }

            // 开启码流 
            // start grabbing 
            res = cam.IMV_StartGrabbing();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Start grabbing failed!");
                return;
            }

            m_bGrabbing = true;
            m_hReceiveThread = new Thread(new ThreadStart(ReceiveThreadProcess));
            m_hReceiveThread.Start();

        }
    
        // 停止码流 
        // stop grabbing 
        private void btnClose_Click(object sender, EventArgs e)
        {
            int res = IMVDefine.IMV_OK;

            // 停止拉流 
            // Stop grabbing 
            res = cam.IMV_StopGrabbing();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Stop grabbing failed!");
                return;
            }

            m_bGrabbing = false;
            m_hReceiveThread.Join();

            //关闭相机
            //Close camera 
            res = cam.IMV_Close();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Close camera failed!");
                return;
            }

            // 销毁设备句柄
            // Destroy Device Handle
            res = cam.IMV_DestroyHandle();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Destroy camera failed!");
                return;
            }                                             

            this.Invoke(new Action(() =>
            {
                btnOpen.Enabled = true;
                btnClose.Enabled = false;
            }));


        }

        // 窗口关闭 
        // Window Closed 
        protected override void OnClosed(EventArgs e)
        {
            int res = IMVDefine.IMV_OK;
            if (cam.IMV_IsGrabbing())
            {
                res = cam.IMV_StopGrabbing();
                if (res != IMVDefine.IMV_OK)
                {
                    MessageBox.Show("Stop grabbing failed!");
                    return;
                }
            }

            m_bGrabbing = false;
            if (m_hReceiveThread != null)
            {
                m_hReceiveThread.Join();
            }

            if (cam.IMV_IsOpen())
            {
                res = cam.IMV_Close();
                if (res != IMVDefine.IMV_OK)
                {
                    MessageBox.Show("Close camera failed!");
                    return;
                }

                res = cam.IMV_DestroyHandle();
                if (res != IMVDefine.IMV_OK)
                {
                    MessageBox.Show("Destroy camera failed!");
                    return;
                } 
            }
            base.OnClosed(e);
        }

        private void bnEnum_Click(object sender, EventArgs e)
        {
            cbDeviceList.Items.Clear();

            // 设备搜索 
            // device search 
            int res = IMVDefine.IMV_OK;
            deviceList = new IMVDefine.IMV_DeviceList();
            IMVDefine.IMV_EInterfaceType interfaceTp = IMVDefine.IMV_EInterfaceType.interfaceTypeAll;
            res = MyCamera.IMV_EnumDevices(ref deviceList, (uint)interfaceTp);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show("Enum Devices Fail");
                return;
            }
            if (deviceList.nDevNum == 0)
            {
                return;
            }

            // 在窗体列表中显示设备名
            // Display the device'name on window's list
            for (int i = 0; i < deviceList.nDevNum; i++)
            {
                IMVDefine.IMV_DeviceInfo deviceInfo = (IMVDefine.IMV_DeviceInfo)Marshal.PtrToStructure(deviceList.pDevInfo + i * Marshal.SizeOf(typeof(IMVDefine.IMV_DeviceInfo)), typeof(IMVDefine.IMV_DeviceInfo));
                if (deviceInfo.nCameraType == IMVDefine.IMV_ECameraType.typeGigeCamera)
                {
                    IMVDefine.IMV_GigEDeviceInfo gigEDeviceInfo =
                        (IMVDefine.IMV_GigEDeviceInfo)
                            ByteToStruct(deviceInfo.deviceSpecificInfo.gigeDeviceInfo,
                                typeof(IMVDefine.IMV_GigEDeviceInfo));
                    if (deviceInfo.cameraName != "")
                    {
                        cbDeviceList.Items.Add("GigE: " + deviceInfo.cameraName + " (" + deviceInfo.serialNumber + ")");
                    }
                    else
                    {
                        cbDeviceList.Items.Add("GigE: " + deviceInfo.manufactureInfo + " " + deviceInfo.modelName + " (" + deviceInfo.serialNumber + ")");
                    }
                }
                else if(deviceInfo.nCameraType==IMVDefine.IMV_ECameraType.typeU3vCamera)
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

            cbDeviceList.SelectedIndex= 0;
        }

        public static object ByteToStruct(Byte[] bytes, Type type)
        {
            int size = Marshal.SizeOf(type);
            if (size > bytes.Length)
            {
                return null;
            }

            // 分配结构体内存空间
            IntPtr structPtr = Marshal.AllocHGlobal(size);

            // 将byte数组拷贝到分配好的内存空间
            Marshal.Copy(bytes, 0, structPtr, size);

            // 将内存空间转换为目标结构体
            object obj = Marshal.PtrToStructure(structPtr, type);

            // 释放内存空间
            Marshal.FreeHGlobal(structPtr);

            return obj;
        }
    }
}
