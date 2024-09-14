//**********************************************************************
// 本Demo为简单演示SDK的使用，由于采集卡和相机皆支持触发设置，故本Demo中
// 对于两种触发方式都进行了说明，在使用中可根据具体情况寻找合适的方法                 
// This Demo shows how to use GenICam API(C) to write a simple program.
// Because of board and device all support  to setting trigger, this demo writes two tergger methods.
// You can use suitable method during use. 
//**********************************************************************

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
using System.IO;
using CaptureCard_Net;

namespace BasicDemo
{
    public partial class Form1 : Form
    {
        private CardDev card = new CardDev(); // 采集卡对象 | card object
        private CamDev cam = new CamDev(); // 设备对象 | device object
        private List<IMVFGDefine.IMV_FG_Frame> m_frameList = new List<IMVFGDefine.IMV_FG_Frame>(); // 图像缓存列表
        private Thread renderThread = null; // 显示线程 | image display thread 
        private bool m_bShowLoop = true; // 线程控制变量 | thread looping flag 
        private Mutex m_mutex = new Mutex(); // 锁，保证多线程安全 | mutex 
        private Render m_Render; // 显示类 | display
        private bool m_bGrabbing = false;
        private Thread m_hReceiveThread = null; //取流线程 | get frame thread
        private IMVFGDefine.IMV_FG_Frame m_frame;
        private IntPtr m_pDstData;
        private int m_nDataLenth = 0;
        private string m_binSavePath = Environment.CurrentDirectory + @"\Bins";
        private string m_bitMapSavePath = Environment.CurrentDirectory + @"\BitMaps";
        private IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST m_interfaceList;
        private IMVFGDefine.IMV_FG_EInterfaceType m_interfaceTp = IMVFGDefine.IMV_FG_EInterfaceType.typeCLInterface;
        private IntPtr m_BufForDriver = IntPtr.Zero;
        private UInt32 m_nBufSizeForDriver = 0;
        private IMVFGDefine.IMV_FG_FrameInfo frameInfo = new IMVFGDefine.IMV_FG_FrameInfo();
        private IMVFGDefine.IMV_FG_DEVICE_INFO_LIST m_deviceList;

        [DllImport("kernel32.dll", EntryPoint = "CopyMemory", SetLastError = false)]
        public static extern void CopyMemory(IntPtr dest, IntPtr src, int count);

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            //初始化界面上各个控件状态
            //Initialize the state of Control operation
            btnStart.Enabled = false;
            btnStop.Enabled = false;
            btnSoftware.Enabled = false;
            btnSaveBMP.Enabled = false;
            btnSaveBIN.Enabled = false;
            btnSetInterfaceParam.Enabled = false;
            btnGetInterfaceParam.Enabled = false;
            cmbInterfacePixelType.Enabled = false;
            txtInterfaceWidth.Enabled = false;
            txtInterfaceHeight.Enabled = false;
            chbSoftTrigger.Enabled = false;
            radioContinue.Enabled = false;
            radioTrigger.Enabled = false;
            btnGetDeviceParam.Enabled = false;
            btnSetDeviceParam.Enabled = false;
            cmbDevicePixelType.Enabled = false;
            txtDeviceExposuretime.Enabled = false;
            txtDeviceGainRaw.Enabled = false;
            btnCloseInterface.Enabled = false;
            btnCloseDevice.Enabled = false;

            m_Render = new Render(pbImage.Handle);
            if (null == renderThread)
            {
                renderThread = new Thread(new ThreadStart(ShowThread));
                renderThread.Start();
            }
            btnEnumInterfaces_Click(null, null);
            btnEnumDevices_Click(null, null);
            m_stopWatch.Start();
        }

        public void ReceiveThreadProcess()
        {
            while (m_bGrabbing)
            {
                if (cam != null && (IMVFGDefine.IMV_FG_OK == card.IMV_FG_GetFrame(ref m_frame, 1000)))
                {
                    if (m_BufForDriver == IntPtr.Zero || m_frame.frameInfo.size > m_nBufSizeForDriver)
                    {
                        if (m_BufForDriver != IntPtr.Zero)
                        {
                            Marshal.FreeHGlobal(m_BufForDriver);
                            m_BufForDriver = IntPtr.Zero;
                        }

                        m_BufForDriver = Marshal.AllocHGlobal((Int32) m_frame.frameInfo.size);
                        if (m_BufForDriver == IntPtr.Zero)
                        {
                            return;
                        }
                        m_nBufSizeForDriver = m_frame.frameInfo.size;
                    }
                    frameInfo = m_frame.frameInfo;
                    CopyMemory(m_BufForDriver, m_frame.pData, (int) m_frame.frameInfo.size);
                    //添加数据
                    m_mutex.WaitOne();
                    m_frameList.Add(m_frame);
                    m_mutex.ReleaseMutex();
                }
            }
        }

        // 转码显示线程 
        // display thread routine 
        private void ShowThread()
        {
            while (m_bShowLoop)
            {
                if (m_frameList.Count == 0)
                {
                    Thread.Sleep(10);
                    continue;
                }

                m_mutex.WaitOne();
                IMVFGDefine.IMV_FG_Frame frame = m_frameList.ElementAt(0);
                m_frameList.Remove(frame);
                m_mutex.ReleaseMutex();

                // 主动调用回收垃圾 
                // call garbage collection 
                GC.Collect();

                // 控制显示最高帧率为25FPS 
                // control frame display rate to be 25 FPS 
                if (false == isTimeToDisplay())
                {
                    card.IMV_FG_ReleaseFrame(ref frame);
                    continue;
                }

                try
                {
                    if (frame.frameInfo.pixelFormat == IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8)
                    {
                        m_Render.Display(frame.pData, (int) frame.frameInfo.width, (int) frame.frameInfo.height,
                            Render.VR_PIXEL_TYPE_E.VR_PIXEL_FMT_MONO8);
                    }
                    else if (frame.frameInfo.pixelFormat == IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8)
                    {
                        m_Render.Display(frame.pData, (int) frame.frameInfo.width, (int) frame.frameInfo.height,
                            Render.VR_PIXEL_TYPE_E.VR_PIXEL_FMT_RGB24);
                    }
                    else
                    {
                        // 图像转码为BGR格式 
                        // raw frame data converted to BGR 
                        if (true == ConvertToBGR24(frame)) //转码成功显示图像
                        {
                            m_Render.Display(m_pDstData, (int) frame.frameInfo.width, (int) frame.frameInfo.height,
                                Render.VR_PIXEL_TYPE_E.VR_PIXEL_FMT_RGB24);
                        }
                    }
                }
                catch (Exception exception)
                {
                    MessageBox.Show(exception.ToString());
                }
                finally
                {
                    // 必须主动释放驱动缓存
                    // Must actively release the driver cache
                    card.IMV_FG_ReleaseFrame(ref frame);
                }
            }
        }

        private const int DEFAULT_INTERVAL = 40;
        private Stopwatch m_stopWatch = new Stopwatch();

        // 判断是否应该做显示操作 
        // calculate interval to determine if it's show time now 
        private bool isTimeToDisplay()
        {
            m_stopWatch.Stop();
            long m_lDisplayInterval = m_stopWatch.ElapsedMilliseconds;
            if (m_lDisplayInterval <= DEFAULT_INTERVAL)
            {
                m_stopWatch.Start();
                return false;
            }
            else
            {
                m_stopWatch.Reset();
                m_stopWatch.Start();
                return true;
            }
        }

        // 窗口关闭 
        // Window Closed 
        protected override void OnClosed(EventArgs e)
        {
            btnCloseDevice_Click(null, null);
            btnCloseInterface_Click(null, null);

            m_bShowLoop = false;
            renderThread.Join();
            m_Render.Close();
            m_Render = null;

            if (m_pDstData != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(m_pDstData);
                m_pDstData = IntPtr.Zero;
            }

            if (m_BufForDriver != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(m_BufForDriver);
                m_BufForDriver = IntPtr.Zero;
            }

            base.OnClosed(e);
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            // 标志位置true 
            // set position bit true
            m_bGrabbing = true;

            m_hReceiveThread = new Thread(ReceiveThreadProcess);
            m_hReceiveThread.Start();

            // 开启码流 
            // start grabbing 
            if (IMVFGDefine.IMV_FG_OK != card.IMV_FG_StartGrabbing())
            {
                m_bGrabbing = false;
                m_hReceiveThread.Join();
                MessageBox.Show("Start grabbing failed");
                return;
            }

            SetCtrlWhenStartGrab();
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            // 标志位置为false
            // set position bit false
            m_bGrabbing = false;
            m_hReceiveThread.Join();

            // 停止码流 
            // stop grabbing
            if (IMVFGDefine.IMV_FG_OK != card.IMV_FG_StopGrabbing())
            {
                MessageBox.Show("Stop Grabbing Fail!");
                return;
            }

            SetCtrlWhenStopGrab();
        }

        //软触发一次
        //Software Trigger
        private void btnSoftware_Click(object sender, EventArgs e)
        {
            //发送一次触发命令
            //Send Trigger Command
            int res = IMVFGDefine.IMV_FG_OK;

#if TRIGGERBYBOARD //采集卡软触发
            res = card.IMV_FG_ExecuteCommandFeature("TriggerSoftware");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Execute TriggerSoftware failed! ErrorCode[{0}]", res));
                return;
            }
#else //相机软触发
            res = cam.IMV_FG_ExecuteCommandFeature("TriggerSoftware");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Execute TriggerSoftware failed! ErrorCode[{0}]", res));
                return;
            }
#endif
        }

        //转码函数
        //transcoding function
        private bool ConvertToBGR24(IMVFGDefine.IMV_FG_Frame frame)
        {
            IMVFGDefine.IMV_FG_PixelConvertParam stPixelConvertParam = new IMVFGDefine.IMV_FG_PixelConvertParam();

            //当内存申请失败，返回false
            try
            {
                if (m_pDstData == IntPtr.Zero || (int) (frame.frameInfo.width*frame.frameInfo.height*3) > m_nDataLenth)
                {
                    if (m_pDstData != IntPtr.Zero)
                    {
                        Marshal.FreeHGlobal(m_pDstData);
                    }
                    m_pDstData = Marshal.AllocHGlobal((int) (frame.frameInfo.width*frame.frameInfo.height*3));
                    m_nDataLenth = (int) (frame.frameInfo.width*frame.frameInfo.height*3);
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
            stPixelConvertParam.nDstBufSize = (uint) m_nDataLenth;

            int res = card.IMV_FG_PixelConvert(ref stPixelConvertParam);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                // 转码出错,返回false           
                return false;
            }

            return true;
        }

        private bool SaveToBitmap(IntPtr pSrcData)
        {
            Bitmap bitmap = null;
            if (!ConvertToBitmap(pSrcData, ref bitmap))
            {
                return false;
            }

            if (!Directory.Exists(m_bitMapSavePath))
            {
                Directory.CreateDirectory(m_bitMapSavePath);
            }
            try
            {
                string imageName = m_bitMapSavePath + @"\" + frameInfo.blockId + ".bmp";
                bitmap.Save(imageName, ImageFormat.Bmp);
                bitmap.Dispose();
                MessageBox.Show(string.Format("Save bitmap Successfully!Save path is [{0}]", imageName));
            }
            catch (Exception exception)
            {
                if (bitmap != null)
                {
                    bitmap.Dispose();
                }
                MessageBox.Show("Save bitmap Failed,because" + exception.ToString());
                return false;
            }
            return true;
        }

        private bool ConvertToBitmap(IntPtr pSrcData, ref Bitmap bitmap)
        {
            IntPtr pDstRGB;
            BitmapData bmpData;
            Rectangle bitmapRect = new Rectangle();
            int ImgSize;

            if (frameInfo.pixelFormat == IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8)
                //图像格式为Mono8时，无需转码，直接转成bitmap进行保存
            {
                // 用Mono8数据生成Bitmap
                bitmap = new Bitmap((int) frameInfo.width, (int) frameInfo.height, PixelFormat.Format8bppIndexed);
                ColorPalette colorPalette = bitmap.Palette;
                for (int i = 0; i != 256; ++i)
                {
                    colorPalette.Entries[i] = Color.FromArgb(i, i, i);
                }
                bitmap.Palette = colorPalette;

                bitmapRect.Height = bitmap.Height;
                bitmapRect.Width = bitmap.Width;
                bmpData = bitmap.LockBits(bitmapRect, ImageLockMode.ReadWrite, bitmap.PixelFormat);
                CopyMemory(bmpData.Scan0, pSrcData, bmpData.Stride*bitmap.Height);
                bitmap.UnlockBits(bmpData);
            }
            else if (frameInfo.pixelFormat == IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8)
                //图像格式为BGR8时，无需转码，直接转成bitmap进行保存
            {
                // 用BGR24数据生成Bitmap
                bitmap = new Bitmap((int) frameInfo.width, (int) frameInfo.height, PixelFormat.Format24bppRgb);

                bitmapRect.Height = bitmap.Height;
                bitmapRect.Width = bitmap.Width;
                bmpData = bitmap.LockBits(bitmapRect, ImageLockMode.ReadWrite, bitmap.PixelFormat);
                CopyMemory(bmpData.Scan0, pSrcData, bmpData.Stride*bitmap.Height);
                bitmap.UnlockBits(bmpData);
            }
            else //当图像格式为其它时，先转码为BGR24，然后转成bitmap进行保存
            {
                ImgSize = (int) frameInfo.width*(int) frameInfo.height*3;

                try
                {
                    pDstRGB = Marshal.AllocHGlobal(ImgSize);
                }
                catch
                {
                    return false;
                }
                if (pDstRGB == IntPtr.Zero)
                {
                    return false;
                }

                IMVFGDefine.IMV_FG_PixelConvertParam stPixelConvertParam =
                    new IMVFGDefine.IMV_FG_PixelConvertParam();
                int res = 0;
                // 转码参数
                stPixelConvertParam.nWidth = frameInfo.width;
                stPixelConvertParam.nHeight = frameInfo.height;
                stPixelConvertParam.ePixelFormat = frameInfo.pixelFormat;
                stPixelConvertParam.pSrcData = pSrcData;
                stPixelConvertParam.nSrcDataLen = frameInfo.size;
                stPixelConvertParam.nPaddingX = frameInfo.paddingX;
                stPixelConvertParam.nPaddingY = frameInfo.paddingY;
                stPixelConvertParam.eBayerDemosaic =
                    IMVFGDefine.IMV_FG_EBayerDemosaic.IMV_FG_DEMOSAIC_NEAREST_NEIGHBOR;
                stPixelConvertParam.eDstPixelFormat = IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8;
                stPixelConvertParam.pDstBuf = pDstRGB;
                stPixelConvertParam.nDstBufSize = (uint) ImgSize;

                res = card.IMV_FG_PixelConvert(ref stPixelConvertParam);
                if (IMVFGDefine.IMV_FG_OK != res)
                {
                    // 转码出错
                    MessageBox.Show("image convert to BGR failed!");
                    return false;
                }

                // 用BGR24数据生成Bitmap
                bitmap = new Bitmap((int) frameInfo.width, (int) frameInfo.height, PixelFormat.Format24bppRgb);

                bitmapRect.Height = bitmap.Height;
                bitmapRect.Width = bitmap.Width;
                bmpData = bitmap.LockBits(bitmapRect, ImageLockMode.ReadWrite, bitmap.PixelFormat);
                CopyMemory(bmpData.Scan0, pDstRGB, bmpData.Stride*bitmap.Height);
                bitmap.UnlockBits(bmpData);

                Marshal.FreeHGlobal(pDstRGB);
            }
            return true;
        }

        private bool SaveToBin(IntPtr pSrcData)
        {
            byte[] pBuffer = new byte[frameInfo.size];
            Marshal.Copy(pSrcData, pBuffer, 0, (int) frameInfo.size);
            if (!Directory.Exists(m_binSavePath))
            {
                Directory.CreateDirectory(m_binSavePath);
            }
            try
            {
                string binPath = m_binSavePath + @"\" + frameInfo.blockId + ".bin";
                using (Stream filePath = new FileStream(binPath, FileMode.Create))
                {
                    using (BinaryWriter sw = new BinaryWriter(filePath)) //建立二进制文件流
                    {
                        sw.Write(pBuffer);
                    }
                }
                MessageBox.Show(string.Format("Save bin Successfully!Save path is [{0}]", binPath));
            }
            catch (Exception exception)
            {
                MessageBox.Show("Save bin Failed,because" + exception.ToString());
                return false;
            }
            return true;
        }

        private void btnSaveBMP_Click(object sender, EventArgs e)
        {
            if (false == m_bGrabbing)
            {
                MessageBox.Show("Not Start Grabbing");
                return;
            }

            if (m_BufForDriver != IntPtr.Zero)
                SaveToBitmap(m_BufForDriver);
        }

        private void btnSaveBIN_Click(object sender, EventArgs e)
        {
            if (false == m_bGrabbing)
            {
                MessageBox.Show("Not Start Grabbing");
                return;
            }

            if (m_BufForDriver != IntPtr.Zero)
                SaveToBin(m_BufForDriver);
        }

        private void SetCtrlWhenInterfaceOpen()
        {
            this.Invoke(new Action(() =>
            {
                btnOpenInterface.Enabled = false;
                btnCloseInterface.Enabled = true;
                btnStart.Enabled = true;
                btnStop.Enabled = false;
                btnSoftware.Enabled = false;
                btnSaveBMP.Enabled = false;
                btnSaveBIN.Enabled = false;
                btnSetInterfaceParam.Enabled = true;
                btnGetInterfaceParam.Enabled = true;

                cmbInterfacePixelType.Enabled = true;
                txtInterfaceWidth.Enabled = true;
                txtInterfaceHeight.Enabled = true;

                chbSoftTrigger.Enabled = true;

                radioContinue.Enabled = true;
                radioTrigger.Enabled = true;
            }));
        }

        private void SetCtrlWhenInterfaceClose()
        {
            this.Invoke(new Action(() =>
            {
                btnOpenInterface.Enabled = true;
                btnCloseInterface.Enabled = false;
                btnStart.Enabled = false;
                btnStop.Enabled = false;
                btnSoftware.Enabled = false;
                btnSaveBMP.Enabled = false;
                btnSaveBIN.Enabled = false;
                btnSetInterfaceParam.Enabled = false;
                btnGetInterfaceParam.Enabled = false;

                cmbInterfacePixelType.Enabled = false;
                txtInterfaceWidth.Enabled = false;
                txtInterfaceHeight.Enabled = false;

                chbSoftTrigger.Enabled = false;

                radioContinue.Enabled = false;
                radioTrigger.Enabled = false;
            }));
        }

        private void SetCtrlWhenDeviceOpen()
        {
            this.Invoke(new Action(() =>
            {
                btnOpenDevice.Enabled = false;
                btnCloseDevice.Enabled = true;

                btnSetDeviceParam.Enabled = true;
                btnGetDeviceParam.Enabled = true;

                cmbDevicePixelType.Enabled = true;
                txtDeviceExposuretime.Enabled = true;
                txtDeviceGainRaw.Enabled = true;
            }));
        }

        private void SetCtrlWhenDeviceClose()
        {
            this.Invoke(new Action(() =>
            {
                btnOpenDevice.Enabled = true;
                btnCloseDevice.Enabled = false;

                btnSetDeviceParam.Enabled = false;
                btnGetDeviceParam.Enabled = false;

                cmbDevicePixelType.Enabled = false;
                txtDeviceExposuretime.Enabled = false;
                txtDeviceGainRaw.Enabled = false;
            }));
        }

        private void SetCtrlWhenStartGrab()
        {
            this.Invoke(new Action(() =>
            {
                btnStart.Enabled = false;
                btnStop.Enabled = true;
                btnSaveBMP.Enabled = true;
                btnSaveBIN.Enabled = true;
                cmbInterfacePixelType.Enabled = false;
                txtInterfaceHeight.Enabled = false;
                txtInterfaceWidth.Enabled = false;

                if (radioTrigger.Checked)
                {
                    chbSoftTrigger.Enabled = true;
                }
                else
                {
                    chbSoftTrigger.Enabled = false;
                }
                if (chbSoftTrigger.Checked && radioTrigger.Checked)
                {
                    btnSoftware.Enabled = true;
                }
                else
                {
                    btnSoftware.Enabled = false;
                }
            }));
        }

        private void SetCtrlWhenStopGrab()
        {
            //设置控件状态
            //set control operation state
            this.Invoke(new Action(() =>
            {
                btnStart.Enabled = true;
                btnStop.Enabled = false;
                btnSaveBMP.Enabled = false;
                btnSaveBIN.Enabled = false;
                btnSoftware.Enabled = false;
                cmbInterfacePixelType.Enabled = true;
                txtInterfaceHeight.Enabled = true;
                txtInterfaceWidth.Enabled = true;
            }));
        }

        private void radioContinue_CheckedChanged(object sender, EventArgs e)
        {
            if (radioContinue.Checked)
            {
                if (card.IMV_FG_IsOpenInterface() && cam.IMV_FG_IsDeviceOpen())
                {
                    // 设置为自由拉流模式
                    // Set TriggerMode to Off
                    SetContinuousConf();
                }

                this.Invoke(new Action(() =>
                {
                    btnSoftware.Enabled = false;
                    chbSoftTrigger.Enabled = false;
                }));
            }
        }

        private void radioTrigger_CheckedChanged(object sender, EventArgs e)
        {
            if (radioTrigger.Checked)
            {
                if (chbSoftTrigger.Checked)
                {
                    //设置触发源为软触发
                    //Set Trigger Source to Software
                    SetSoftTriggerConf();

                    this.Invoke(new Action(() =>
                    {
                        btnSoftware.Enabled = true;
                        chbSoftTrigger.Enabled = true;
                    }));
                }
                else
                {
                    //设置触发源为Line1
                    //Set Trigger Source to Line1
                    SetLineTriggerConf();

                    this.Invoke(new Action(() =>
                    {
                        btnSoftware.Enabled = false;
                        chbSoftTrigger.Enabled = true;
                    }));
                }
            }
        }

        private void chbSoftTrigger_CheckedChanged(object sender, EventArgs e)
        {
            if (chbSoftTrigger.Checked)
            {
                //设置触发源为软触发
                //Set Trigger Source to Software
                SetSoftTriggerConf();

                this.Invoke(new Action(() =>
                {
                    btnSoftware.Enabled = true;
                }));
            }
            else
            {
                //设置触发源为Line1
                //Set Trigger Source to Line1
                SetLineTriggerConf();

                this.Invoke(new Action(() =>
                {
                    btnSoftware.Enabled = false;
                }));
            }
        }

        private void btnEnumInterfaces_Click(object sender, EventArgs e)
        {
            cmbInterfaceList.Items.Clear();
            int res = IMVFGDefine.IMV_FG_OK;
            m_interfaceList = new IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST();

            res = CardDev.IMV_FG_EnumInterface((uint) m_interfaceTp, ref m_interfaceList);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Enumeration board failed!errorCode:[{0}]", res));
                return;
            }
            if (0 == m_interfaceList.nInterfaceNum)
            {
                MessageBox.Show("No board found!");
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
        }

        private void btnOpenInterface_Click(object sender, EventArgs e)
        {
            if (cmbInterfaceList.Items.Count > 0)
            {
                cmbInterfacePixelType.Items.Clear();
                cmbInterfacePixelType.Text = string.Empty;
                int res = IMVFGDefine.IMV_FG_OK;
                uint boardIndex = (uint) cmbInterfaceList.SelectedIndex;

                res = card.IMV_FG_OpenInterface(boardIndex);
                if (IMVFGDefine.IMV_FG_OK != res)
                {
                    MessageBox.Show(string.Format("Open cameralink capture board device failed!errorCode:[{0}]", res));
                    return;
                }

                // 获取所有可以设置的像素格式 
                // get all Image Pixel Format
                uint nEntryNum = 0;
                IMVFGDefine.IMV_FG_EnumEntryList pixelTypeList = new IMVFGDefine.IMV_FG_EnumEntryList();
                res = card.IMV_FG_GetEnumFeatureEntryNum("PixelFormat", ref nEntryNum);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    MessageBox.Show(string.Format("Get settable enumeration number failed! ErrorCode[{0}]", res));
                    return;
                }

                pixelTypeList.nEnumEntryBufferSize = (uint) Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_EnumEntryInfo))*
                                                     nEntryNum;
                pixelTypeList.pEnumEntryInfo = Marshal.AllocHGlobal((int) pixelTypeList.nEnumEntryBufferSize);
                if (pixelTypeList.pEnumEntryInfo == IntPtr.Zero)
                {
                    MessageBox.Show("pEnumEntryInfo is NULL");
                    return;
                }

                res = card.IMV_FG_GetEnumFeatureEntrys("PixelFormat", ref pixelTypeList);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    MessageBox.Show("get all Image Pixel Format failed");
                    return;
                }
                for (int i = 0; i < nEntryNum; i++)
                {
                    IMVFGDefine.IMV_FG_EnumEntryInfo entryInfo =
                        (IMVFGDefine.IMV_FG_EnumEntryInfo)
                            Marshal.PtrToStructure(
                                pixelTypeList.pEnumEntryInfo +
                                Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_EnumEntryInfo))*i,
                                typeof (IMVFGDefine.IMV_FG_EnumEntryInfo));
                    cmbInterfacePixelType.Items.Add(entryInfo.name);
                }

                if (pixelTypeList.pEnumEntryInfo != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(pixelTypeList.pEnumEntryInfo);
                    pixelTypeList.pEnumEntryInfo = IntPtr.Zero;
                }

                // 设置为自由拉流模式
                // Set TriggerMode to Off
                radioContinue.Checked = true;

                btnGetInterfaceParam_Click(null, null);
                SetCtrlWhenInterfaceOpen();
            }
        }

        private void btnCloseInterface_Click(object sender, EventArgs e)
        {
            if (card.getHandle() == IntPtr.Zero)
            {
                return;
            }
            int res = IMVFGDefine.IMV_FG_OK;
            if (card.IMV_FG_IsGrabbing())
            {
                btnStop_Click(null, null);
            }
            res = card.IMV_FG_CloseInterface();
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("Close board failed!");
                return;
            }

            SetCtrlWhenInterfaceClose();
        }

        private void btnEnumDevices_Click(object sender, EventArgs e)
        {
            if (m_interfaceList.nInterfaceNum == 0)
            {
                MessageBox.Show("No board found!");
                return;
            }

            cmbDeviceList.Items.Clear();
            cmbDeviceList.Text = string.Empty;

            // 设备搜索 
            // device search 
            m_deviceList = new IMVFGDefine.IMV_FG_DEVICE_INFO_LIST();
            int res = CamDev.IMV_FG_EnumDevices((uint) m_interfaceTp, ref m_deviceList);
            if (res == IMVFGDefine.IMV_FG_OK && m_deviceList.nDevNum > 0)
            {

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
            }
            else
            {
                MessageBox.Show("Enumerate devices fail!Please check!");
            }
        }

        private void btnOpenDevice_Click(object sender, EventArgs e)
        {
            if (cmbDeviceList.Items.Count > 0)
            {
                if (cam.IMV_FG_IsDeviceOpen())
                {
                    MessageBox.Show("Device is opend!");
                    return;
                }

                int deviceIndex = cmbDeviceList.SelectedIndex;
                int res = IMVFGDefine.IMV_FG_OK;
                cmbDevicePixelType.Items.Clear();
                cmbDevicePixelType.Text = string.Empty;

                res = cam.IMV_FG_OpenDevice(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_INDEX, deviceIndex);
                if (IMVFGDefine.IMV_FG_OK != res)
                {
                    MessageBox.Show(string.Format("Open cameralink capture board device failed!errorCode:[{0}]", res));
                    return;
                }

                // 获取所有可以设置的像素格式 
                // get all Image Pixel Format
                uint nEntryNum = 0;
                IMVFGDefine.IMV_FG_EnumEntryList pixelTypeList = new IMVFGDefine.IMV_FG_EnumEntryList();
                res = cam.IMV_FG_GetEnumFeatureEntryNum("PixelFormat", ref nEntryNum);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    MessageBox.Show(string.Format("Get settable enumeration number failed! ErrorCode[{0}]", res));
                    return;
                }

                pixelTypeList.nEnumEntryBufferSize = (uint) Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_EnumEntryInfo))*
                                                     nEntryNum;
                pixelTypeList.pEnumEntryInfo = Marshal.AllocHGlobal((int) pixelTypeList.nEnumEntryBufferSize);
                if (pixelTypeList.pEnumEntryInfo == IntPtr.Zero)
                {
                    MessageBox.Show("pEnumEntryInfo is NULL");
                    return;
                }

                res = cam.IMV_FG_GetEnumFeatureEntrys("PixelFormat", ref pixelTypeList);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    MessageBox.Show("get all Image Pixel Format failed");
                    return;
                }
                for (int i = 0; i < nEntryNum; i++)
                {
                    IMVFGDefine.IMV_FG_EnumEntryInfo entryInfo =
                        (IMVFGDefine.IMV_FG_EnumEntryInfo)
                            Marshal.PtrToStructure(
                                pixelTypeList.pEnumEntryInfo +
                                Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_EnumEntryInfo))*i,
                                typeof (IMVFGDefine.IMV_FG_EnumEntryInfo));
                    cmbDeviceList.Items.Add(entryInfo.name);
                }

                if (pixelTypeList.pEnumEntryInfo != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(pixelTypeList.pEnumEntryInfo);
                    pixelTypeList.pEnumEntryInfo = IntPtr.Zero;
                }

                btnGetDeviceParam_Click(null, null);
                SetCtrlWhenDeviceOpen();
            }
            else
            {
                MessageBox.Show("No Camera!");
            }
        }

        private void btnCloseDevice_Click(object sender, EventArgs e)
        {
            int res = IMVFGDefine.IMV_FG_OK;
            if (cam.getHandle() != IntPtr.Zero)
            {
                res = cam.IMV_FG_CloseDevice();
                if (IMVFGDefine.IMV_FG_OK != res)
                {
                    MessageBox.Show("Close device failed!");
                    return;
                }
            }

            SetCtrlWhenDeviceClose();
        }

        private void btnGetInterfaceParam_Click(object sender, EventArgs e)
        {
            if (false == card.IMV_FG_IsOpenInterface())
            {
                MessageBox.Show("Board is not opend!");
                return;
            }
            int res = IMVFGDefine.IMV_FG_OK;
            cmbInterfacePixelType.Text = string.Empty;

            //获取图像格式                   
            IMVFGDefine.IMV_FG_String pixelType = new IMVFGDefine.IMV_FG_String();
            res = card.IMV_FG_GetEnumFeatureSymbol("PixelFormat", ref pixelType);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("get image pixle format failed!");
            }
            cmbInterfacePixelType.SelectedText = pixelType.str;

            // 获取图像宽度
            // get Image Width
            long width = 0;
            res = card.IMV_FG_GetIntFeatureValue("Width", ref width);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("get Image failed!");
            }
            txtInterfaceWidth.Text = width.ToString();

            // 获取图像高度 
            // get Image Height 
            long height = 0;
            res = card.IMV_FG_GetIntFeatureValue("Height", ref height);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("get Image Height failed!");
            }
            txtInterfaceHeight.Text = height.ToString();
        }

        private void btnSetInterfaceParam_Click(object sender, EventArgs e)
        {
            if (false == card.IMV_FG_IsOpenInterface())
            {
                MessageBox.Show("Board is not opened!");
                return;
            }

            try
            {
                long.Parse(txtInterfaceWidth.Text);
                long.Parse(txtInterfaceHeight.Text);
            }
            catch
            {
                MessageBox.Show("Please enter correct type!");
                return;
            }

            int res = IMVFGDefine.IMV_FG_OK;

            // 获取可设置宽度值
            // get valid width 
            long width = long.Parse(txtInterfaceWidth.Text);
            long maxwidth = 0;
            long minwidth = 0;
            long incrementwidth = 0;
            res = card.IMV_FG_GetIntFeatureMin("Width", ref minwidth);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Get feature minimum width failed! ErrorCode[{0}]", res));
                return;
            }
            res = card.IMV_FG_GetIntFeatureMax("Width", ref maxwidth);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Get feature maximum width failed! ErrorCode[{0}]", res));
                return;
            }
            res = card.IMV_FG_GetIntFeatureInc("Width", ref incrementwidth);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Get width increment value failed! ErrorCode[{0}]", res));
                return;
            }
            if (width < minwidth || width > maxwidth || (width - minwidth)%incrementwidth != 0)
            {
                MessageBox.Show(
                    string.Format(
                        "Width is not valid,the max value is {0} ,the min value is {1},the increment value is {2}",
                        maxwidth, minwidth, incrementwidth));
                return;
            }

            // 获取可设置高度值
            // get valid height 
            long height = long.Parse(txtInterfaceHeight.Text);
            long maxheight = 0;
            long minheight = 0;
            long incrementheight = 0;
            res = card.IMV_FG_GetIntFeatureMin("Height", ref minheight);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Get feature minimum height failed! ErrorCode[{0}]", res));
                return;
            }
            res = card.IMV_FG_GetIntFeatureMax("Height", ref maxheight);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Get feature maximum height failed! ErrorCode[{0}]", res));
                return;
            }
            res = card.IMV_FG_GetIntFeatureInc("Height", ref incrementheight);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Get height increment value failed! ErrorCode[{0}]", res));
                return;
            }
            if (height < minheight || height > maxheight || (height - minheight)%incrementheight != 0)
            {
                MessageBox.Show(
                    string.Format(
                        "Height is not valid,the max value is {0} ,the min value is {1},the increment value is {2}",
                        maxheight, minheight, incrementheight));
                return;
            }

            //设置图像格式
            // set Pixel Type
            if (true == card.IMV_FG_IsOpenInterface() && false == card.IMV_FG_IsGrabbing())
            {
                string pixelType = cmbInterfacePixelType.Text;
                res = card.IMV_FG_SetEnumFeatureSymbol("PixelFormat", pixelType);
                if (IMVFGDefine.IMV_FG_OK != res)
                {
                    MessageBox.Show("set pixelFormat fail!");
                }
            }

            // 设置图像宽度
            // set image width
            res = card.IMV_FG_SetIntFeatureValue("Width", width);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("set width fail!");
            }

            // 设置图像高度
            // set image height
            res = card.IMV_FG_SetIntFeatureValue("Height", height);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("set height fail!");
            }
        }

        private void btnGetDeviceParam_Click(object sender, EventArgs e)
        {
            if (false == cam.IMV_FG_IsDeviceOpen())
            {
                MessageBox.Show("Device is not open!");
                return;
            }

            int res = IMVFGDefine.IMV_FG_OK;
            cmbDevicePixelType.Text = string.Empty;

            //获取图像格式                   
            IMVFGDefine.IMV_FG_String pixelType = new IMVFGDefine.IMV_FG_String();
            res = cam.IMV_FG_GetEnumFeatureSymbol("PixelFormat", ref pixelType);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("get image pixle format failed!");
            }
            cmbDevicePixelType.SelectedText = pixelType.str;

            // 获取曝光值
            // set ExposureTime 
            double exposureTime = 0;
            res = cam.IMV_FG_GetDoubleFeatureValue("ExposureTime", ref exposureTime);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("get exposuretime failed!");
            }
            txtDeviceExposuretime.Text = exposureTime.ToString("f2");

            // 设置增益 
            // set Gain 
            double gain = 0;
            res = cam.IMV_FG_GetDoubleFeatureValue("GainRaw", ref gain);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("get gainraw failed!");
            }
            txtDeviceGainRaw.Text = gain.ToString("f2");
        }

        private void btnSetDeviceParam_Click(object sender, EventArgs e)
        {
            if (false == cam.IMV_FG_IsDeviceOpen())
            {
                MessageBox.Show("Device is not open!");
                return;
            }

            try
            {
                double.Parse(txtDeviceExposuretime.Text);
                double.Parse(txtDeviceGainRaw.Text);
            }
            catch
            {
                MessageBox.Show("Please enter correct type!");
                return;
            }

            int res = IMVFGDefine.IMV_FG_OK;
            // 获取可设置曝光值
            // get valid ExposureTime 
            double exposureTime = double.Parse(txtDeviceExposuretime.Text);
            double maxexposuretime = 0;
            double minexposuretime = 0;
            res = cam.IMV_FG_GetDoubleFeatureMin("ExposureTime", ref minexposuretime);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Get feature minimum exposuretime failed! ErrorCode[{0}]", res));
                return;
            }
            res = cam.IMV_FG_GetDoubleFeatureMax("ExposureTime", ref maxexposuretime);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Get feature maximum exposuretime failed! ErrorCode[{0}]", res));
                return;
            }
            if (exposureTime < minexposuretime || exposureTime > maxexposuretime)
            {
                MessageBox.Show(string.Format("Exposuretime is not valid,the max value is {0} ,the min value is {1}",
                    maxexposuretime, minexposuretime));
                return;
            }

            // 获取可设置增益 
            // get valid Gain 
            double maxgain = 0;
            double mingain = 0;
            double gain = double.Parse(txtDeviceGainRaw.Text);
            res = cam.IMV_FG_GetDoubleFeatureMin("GainRaw", ref mingain);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Get feature minimum gainraw failed! ErrorCode[{0}]", res));
                return;
            }
            res = cam.IMV_FG_GetDoubleFeatureMax("GainRaw", ref maxgain);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show(string.Format("Get feature maximum gainraw failed! ErrorCode[{0}]", res));
                return;
            }
            if (gain < mingain || gain > maxgain)
            {
                MessageBox.Show(string.Format("Gain is not valid,the max value is {0} ,the min value is {1}", maxgain,
                    mingain));
                return;
            }


            //设置图像格式
            // set Pixel Type
            if (true == cam.IMV_FG_IsDeviceOpen() && true == cam.IMV_FG_FeatureIsWriteable("PixelFormat"))
            {
                string pixelType = cmbDevicePixelType.Text;
                res = cam.IMV_FG_SetEnumFeatureSymbol("PixelFormat", pixelType);
                if (IMVFGDefine.IMV_FG_OK != res)
                {
                    MessageBox.Show("set pixelFormat fail!");
                }
            }

            // 设置曝光值
            // set ExposureTime 
            res = cam.IMV_FG_SetDoubleFeatureValue("ExposureTime", exposureTime);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("set exposuretime fail!");
            }

            // 设置增益 
            // set Gain 
            res = cam.IMV_FG_SetDoubleFeatureValue("GainRaw", gain);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show("set gain fail!");
            }
        }

        private void SetContinuousConf()
        {
            cam.IMV_FG_SetEnumFeatureSymbol("TriggerMode", "Off");
        }

        private void SetSoftTriggerConf()
        {
            int res = IMVFGDefine.IMV_FG_OK;

#if TRIGGERBYBOARD //设置采集卡软触发
            // 设置采集卡触发CC1
            // Set capture trigger source to CC1
            res = card.IMV_FG_SetEnumFeatureSymbol("CC1", "SofwareTrigger");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggerSource value failed! ErrorCode[{0}]", res));
                return;
            }

            if (false == cam.IMV_FG_IsDeviceOpen())
            {
                MessageBox.Show("Please open device");
                return;
            }

            // 设置相机触发源为CC1
            // Set trigger source to Software
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerSource", "CC1");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggerSource value failed! ErrorCode[{0}]", res));
                return;
            }

            double LineDeboucerTime = 0;
            // 设置抖动时间
            // Set trigger LineDebouncerTime
            res = cam.IMV_FG_SetDoubleFeatureValue("LineDebouncerTime", LineDeboucerTime);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggerSource value failed! ErrorCode[{0}]", res));
                return;
            }

            // 设置触发器
            // Set trigger selector to FrameStart
            res = cam.IMV_FG_SetEnumFeatureSymbol("LineSelector", "CC1");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggerSource value failed! ErrorCode[{0}]", res));
                return;
            }

            // 设置触发模式
            // Set trigger mode to on
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerMode", "On");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggermode value failed! ErrorCode[{0}]", res));
                return;
            }
#else //设置相机软触发
    // 设置触发模式
    // Set trigger mode to on
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerMode", "On");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggermode value failed! ErrorCode[{0}]", res));
                return;
            }

            // 设置触发源
            // Set trigger source to Software
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerSource", "Software");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggermode value failed! ErrorCode[{0}]", res));
                return;
            }
#endif
        }

        private void SetLineTriggerConf()
        {
            int res = IMVFGDefine.IMV_FG_OK;

#if TRIGGERBYBOARD //设置采集卡外触发
            // 设置采集卡触发CC1
            // Set capture trigger source to CC1
            res = card.IMV_FG_SetEnumFeatureSymbol("CC1", "ExternalTrigger1");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggerSource value failed! ErrorCode[{0}]", res));
                return;
            }

            if (false == cam.IMV_FG_IsDeviceOpen())
            {
                MessageBox.Show("Please open device");
                return;
            }

            // 设置相机触发源为CC1
            // Set trigger source to Software
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerSource", "CC1");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggerSource value failed! ErrorCode[{0}]", res));
                return;
            }

            double LineDeboucerTime = 0;
            // 设置抖动时间
            // Set trigger LineDebouncerTime
            res = cam.IMV_FG_SetDoubleFeatureValue("LineDebouncerTime", LineDeboucerTime);
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggerSource value failed! ErrorCode[{0}]", res));
                return;
            }

            // 设置触发器
            // Set trigger selector to FrameStart
            res = cam.IMV_FG_SetEnumFeatureSymbol("LineSelector", "CC1");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggerSource value failed! ErrorCode[{0}]", res));
                return;
            }

            // 设置触发模式
            // Set trigger mode to on
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerMode", "On");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggermode value failed! ErrorCode[{0}]", res));
                return;
            }

            // 设置外触发为上升沿(下降沿为FallingEdge)
            // Set trigger activation to RisingEdge(FallingEdge in opposite)
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerActivation", "RisingEdge");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggerActivation value failed! ErrorCode[{0}]", res));
                return;
            }
#else // 相机外触发
    // 设置触发模式
    // Set trigger mode to on
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerMode", "On");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggermode value failed! ErrorCode[{0}]", res));
                return;
            }

            // 设置触发源
            // Set trigger source to Software
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerSource", "Line1");
            if (IMVFGDefine.IMV_FG_OK != res)
            {
                MessageBox.Show(string.Format("Set triggermode value failed! ErrorCode[{0}]", res));
                return;
            }
#endif
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
