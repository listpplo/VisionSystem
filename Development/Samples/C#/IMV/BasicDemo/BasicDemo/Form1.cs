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
using MVSDK_Net;

namespace BasicDemo
{
    public partial class Form1 : Form
    {
        private MyCamera cam = new MyCamera(); // 设备对象 | device object
        private List<IMVDefine.IMV_Frame> m_frameList = new List<IMVDefine.IMV_Frame>(); // 图像缓存列表
        private Thread renderThread = null; // 显示线程 | image display thread 
        private bool m_bShowLoop = true; // 线程控制变量 | thread looping flag 
        private Mutex m_mutex = new Mutex(); // 锁，保证多线程安全 | mutex 
        private Render m_Render; // 显示类 | display
        private bool m_bGrabbing = false;
        private Thread m_hReceiveThread = null; //取流线程 | get frame thread
        private IMVDefine.IMV_Frame m_frame;
        private IntPtr m_pDstData;
        private int m_nDataLenth = 0;
        private string m_binSavePath = Environment.CurrentDirectory + @"\Bins";
        private string m_bitMapSavePath = Environment.CurrentDirectory + @"\BitMaps";
        private IntPtr m_BufForDriver = IntPtr.Zero;
        private UInt32 m_nBufSizeForDriver = 0;
        private IMVDefine.IMV_FrameInfo frameInfo = new IMVDefine.IMV_FrameInfo();

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
            btnClose.Enabled = false;
            btnStart.Enabled = false;
            btnStop.Enabled = false;
            btnSoftware.Enabled = false;
            btnSaveBMP.Enabled = false;
            btnSaveBIN.Enabled = false;
            btnSetParam.Enabled = false;
            btnGetParam.Enabled = false;
            cbPixelType.Enabled = false;
            txtExposuretime.Enabled = false;
            txtGainRaw.Enabled = false;
            chbSoftTrigger.Enabled = false;
            radioContinue.Enabled = false;
            radioTrigger.Enabled = false;

            m_Render = new Render(pbImage.Handle);
            if (null == renderThread)
            {
                renderThread = new Thread(new ThreadStart(ShowThread));
                renderThread.Start();
            }
            btnEnumDevices_Click(null, null);
            m_stopWatch.Start();
        }

        public void ReceiveThreadProcess()
        {
            while (m_bGrabbing)
            {
                if (cam != null && (IMVDefine.IMV_OK == cam.IMV_GetFrame(ref m_frame, 1000)))
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
                IMVDefine.IMV_Frame frame = m_frameList.ElementAt(0);
                m_frameList.Remove(frame);
                m_mutex.ReleaseMutex();

                // 主动调用回收垃圾 
                // call garbage collection 
                GC.Collect();

                // 控制显示最高帧率为25FPS 
                // control frame display rate to be 25 FPS 
                if (false == isTimeToDisplay())
                {
                    cam.IMV_ReleaseFrame(ref frame);
                    continue;
                }

                try
                {
                    if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelMono8)
                    {
                        m_Render.Display(frame.pData, (int) frame.frameInfo.width, (int) frame.frameInfo.height,
                            Render.VR_PIXEL_TYPE_E.VR_PIXEL_FMT_MONO8);
                    }
                    else if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelBGR8)
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
                    cam.IMV_ReleaseFrame(ref frame);
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

        private void btnOpen_Click(object sender, EventArgs e)
        {
            try
            {
                if (cbDeviceInfos.Items.Count > 0)
                {
                    // 创建设备句柄
                    // Create Device Handle
                    int res = cam.IMV_CreateHandle(IMVDefine.IMV_ECreateHandleMode.modeByIndex,
                        cbDeviceInfos.SelectedIndex);

                    // 打开设备 
                    // open device 
                    res = cam.IMV_Open();
                    if (res != IMVDefine.IMV_OK)
                    {
                        MessageBox.Show("Open camera failed");
                        return;
                    }

                    cbPixelType.Items.Clear();
                    cbPixelType.Text = string.Empty;

                    // 获取所有可以设置的像素格式 
                    // get all Image Pixel Format
                    uint nEntryNum = 0;
                    IMVDefine.IMV_EnumEntryList pixelTypeList = new IMVDefine.IMV_EnumEntryList();
                    res = cam.IMV_GetEnumFeatureEntryNum("PixelFormat", ref nEntryNum);
                    if (res != IMVDefine.IMV_OK)
                    {
                        MessageBox.Show(string.Format("Get settable enumeration number failed! ErrorCode[{0}]", res));
                        return;
                    }

                    pixelTypeList.nEnumEntryBufferSize = (uint) Marshal.SizeOf(typeof (IMVDefine.IMV_EnumEntryInfo))*
                                                         nEntryNum;
                    pixelTypeList.pEnumEntryInfo = Marshal.AllocHGlobal((int) pixelTypeList.nEnumEntryBufferSize);
                    if (pixelTypeList.pEnumEntryInfo == IntPtr.Zero)
                    {
                        MessageBox.Show("pEnumEntryInfo is NULL");
                        return;
                    }

                    res = cam.IMV_GetEnumFeatureEntrys("PixelFormat", ref pixelTypeList);
                    if (res != IMVDefine.IMV_OK)
                    {
                        MessageBox.Show("get all Image Pixel Format failed");
                        return;
                    }
                    for (int i = 0; i < nEntryNum; i++)
                    {
                        IMVDefine.IMV_EnumEntryInfo entryInfo =
                            (IMVDefine.IMV_EnumEntryInfo)
                                Marshal.PtrToStructure(
                                    pixelTypeList.pEnumEntryInfo +
                                    Marshal.SizeOf(typeof (IMVDefine.IMV_EnumEntryInfo))*i,
                                    typeof (IMVDefine.IMV_EnumEntryInfo));
                        cbPixelType.Items.Add(entryInfo.name);
                    }

                    if (pixelTypeList.pEnumEntryInfo != IntPtr.Zero)
                    {
                        Marshal.FreeHGlobal(pixelTypeList.pEnumEntryInfo);
                        pixelTypeList.pEnumEntryInfo = IntPtr.Zero;
                    }

                    //获取当前相机的触发源
                    //get Trigger Source
                    IMVDefine.IMV_String triggerSourcce = new IMVDefine.IMV_String();
                    res = cam.IMV_GetEnumFeatureSymbol("TriggerSource", ref triggerSourcce);
                    if (res != IMVDefine.IMV_OK)
                    {
                        MessageBox.Show("get TriggerSource failed");
                        return;
                    }
                    if (triggerSourcce.str == "Software")
                    {
                        chbSoftTrigger.Checked = true;
                    }
                    else
                    {
                        chbSoftTrigger.Checked = false;
                    }

                    //获取当前相机的触发模式
                    //get Trigger Mode
                    IMVDefine.IMV_String triggerMode = new IMVDefine.IMV_String();
                    res = cam.IMV_GetEnumFeatureSymbol("TriggerMode", ref triggerMode);
                    if (res != IMVDefine.IMV_OK)
                    {
                        MessageBox.Show("get TriggerMode failed");
                        return;
                    }
                    if (triggerMode.str == "Off")
                    {
                        radioContinue.Checked = true;
                    }
                    else
                    {
                        radioTrigger.Checked = true;
                    }

                    // 获取当前相机的参数
                    // get Parameters
                    getParam();

                    // 设置缓存个数为8（默认值为16） 
                    // set buffer count to 8 (default 16) 
                    res = cam.IMV_SetBufferCount(8);

                    SetCtrlWhenOpen();
                }
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.ToString());
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            try
            {
                if (cam != null)
                {
                    if (true == cam.IMV_IsGrabbing())
                    {
                        btnStop_Click(null, null);
                    }
                    cam.IMV_Close(); // 关闭相机 | close camera 

                    SetCtrlWhenClose();
                }
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.ToString());
            }
        }

        // 窗口关闭 
        // Window Closed 
        protected override void OnClosed(EventArgs e)
        {
            if (true == cam.IMV_IsOpen())
            {
                btnClose_Click(null, null);
            }

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

        private void btnEnumDevices_Click(object sender, EventArgs e)
        {
            cbDeviceInfos.Items.Clear();

            // 设备搜索 
            // device search 
            IMVDefine.IMV_DeviceList deviceList = new IMVDefine.IMV_DeviceList();
            IMVDefine.IMV_EInterfaceType interfaceTp = IMVDefine.IMV_EInterfaceType.interfaceTypeAll;
            int res = MyCamera.IMV_EnumDevices(ref deviceList, (uint) interfaceTp);

            // 添加设备信息
            // Add device info
            if (res == IMVDefine.IMV_OK && deviceList.nDevNum > 0)
            {
                for (int i = 0; i < deviceList.nDevNum; i++)
                {
                    IMVDefine.IMV_DeviceInfo deviceInfo =
                        (IMVDefine.IMV_DeviceInfo)
                            Marshal.PtrToStructure(
                                deviceList.pDevInfo + Marshal.SizeOf(typeof (IMVDefine.IMV_DeviceInfo))*i,
                                typeof (IMVDefine.IMV_DeviceInfo));
                    cbDeviceInfos.Items.Add(deviceInfo.cameraName + "[" + deviceInfo.serialNumber + "]");
                }
                //选择第一项
                //select the first item
                cbDeviceInfos.SelectedIndex = 0;
            }
            else
            {
                MessageBox.Show("Enumerate devices fail!Please check!");
            }
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
            if (IMVDefine.IMV_OK != cam.IMV_StartGrabbing())
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
            if (IMVDefine.IMV_OK != cam.IMV_StopGrabbing())
            {
                MessageBox.Show("Stop Grabbing Fail!");
            }

            SetCtrlWhenStopGrab();
        }

        //软触发一次
        //Software Trigger
        private void btnSoftware_Click(object sender, EventArgs e)
        {
            if (cam.IMV_IsGrabbing())
            {
                //发送一次触发命令
                //Send Trigger Command
                int res = IMVDefine.IMV_OK;
                res = cam.IMV_ExecuteCommandFeature("TriggerSoftware");
                if (IMVDefine.IMV_OK != res)
                {
                    MessageBox.Show(string.Format("Execute TriggerSoftware failed! ErrorCode[{0}]", res));
                    return;
                }
            }
        }

        private void btnSetParam_Click(object sender, EventArgs e)
        {
            if (false == cam.IMV_IsOpen())
            {
                MessageBox.Show("Device is not connect!");
                return;
            }

            try
            {
                double.Parse(txtExposuretime.Text);
                double.Parse(txtGainRaw.Text);
            }
            catch
            {
                MessageBox.Show("Please enter correct type!");
                return;
            }

            int res = IMVDefine.IMV_OK;
            // 获取可设置曝光值
            // get valid ExposureTime 
            double exposureTime = double.Parse(txtExposuretime.Text);
            double maxexposuretime = 0;
            double minexposuretime = 0;
            res = cam.IMV_GetDoubleFeatureMin("ExposureTime", ref minexposuretime);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Get feature minimum exposuretime failed! ErrorCode[{0}]", res));
                return;
            }
            res = cam.IMV_GetDoubleFeatureMax("ExposureTime", ref maxexposuretime);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine(string.Format("Get feature maximum exposuretime failed! ErrorCode[{0}]", res));
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
            double gain = double.Parse(txtGainRaw.Text);
            res = cam.IMV_GetDoubleFeatureMin("GainRaw", ref mingain);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(string.Format("Get feature minimum gainraw failed! ErrorCode[{0}]", res));
                return;
            }
            res = cam.IMV_GetDoubleFeatureMax("GainRaw", ref maxgain);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine(string.Format("Get feature maximum gainraw failed! ErrorCode[{0}]", res));
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
            if (true == cam.IMV_IsOpen() && false == cam.IMV_IsGrabbing())
            {
                string pixelType = cbPixelType.Text;
                res = cam.IMV_SetEnumFeatureSymbol("PixelFormat", pixelType);
                if (IMVDefine.IMV_OK != res)
                {
                    MessageBox.Show("set pixelFormat fail!");
                }
            }

            // 设置曝光值
            // set ExposureTime 
            res = cam.IMV_SetDoubleFeatureValue("ExposureTime", exposureTime);
            if (IMVDefine.IMV_OK != res)
            {
                MessageBox.Show("set exposuretime fail!");
            }

            // 设置增益 
            // set Gain 
            res = cam.IMV_SetDoubleFeatureValue("GainRaw", gain);
            if (IMVDefine.IMV_OK != res)
            {
                MessageBox.Show("set gain fail!");
            }
        }

        //转码函数
        //transcoding function
        private bool ConvertToBGR24(IMVDefine.IMV_Frame frame)
        {
            IMVDefine.IMV_PixelConvertParam stPixelConvertParam = new IMVDefine.IMV_PixelConvertParam();

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
            stPixelConvertParam.eBayerDemosaic = IMVDefine.IMV_EBayerDemosaic.demosaicBilinear;
            stPixelConvertParam.eDstPixelFormat = IMVDefine.IMV_EPixelType.gvspPixelBGR8;
            stPixelConvertParam.pDstBuf = m_pDstData;
            stPixelConvertParam.nDstBufSize = (uint) m_nDataLenth;

            int res = cam.IMV_PixelConvert(ref stPixelConvertParam);
            if (res != IMVDefine.IMV_OK)
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

            if (frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelMono8) //图像格式为Mono8时，无需转码，直接转成bitmap进行保存
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
            else if (frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelBGR8) //图像格式为BGR8时，无需转码，直接转成bitmap进行保存
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

                IMVDefine.IMV_PixelConvertParam stPixelConvertParam = new IMVDefine.IMV_PixelConvertParam();
                int res = 0;
                // 转码参数
                stPixelConvertParam.nWidth = frameInfo.width;
                stPixelConvertParam.nHeight = frameInfo.height;
                stPixelConvertParam.ePixelFormat = frameInfo.pixelFormat;
                stPixelConvertParam.pSrcData = pSrcData;
                stPixelConvertParam.nSrcDataLen = frameInfo.size;
                stPixelConvertParam.nPaddingX = frameInfo.paddingX;
                stPixelConvertParam.nPaddingY = frameInfo.paddingY;
                stPixelConvertParam.eBayerDemosaic = IMVDefine.IMV_EBayerDemosaic.demosaicNearestNeighbor;
                stPixelConvertParam.eDstPixelFormat = IMVDefine.IMV_EPixelType.gvspPixelBGR8;
                stPixelConvertParam.pDstBuf = pDstRGB;
                stPixelConvertParam.nDstBufSize = (uint) ImgSize;

                res = cam.IMV_PixelConvert(ref stPixelConvertParam);
                if (IMVDefine.IMV_OK != res)
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

        private void SetCtrlWhenOpen()
        {
            this.Invoke(new Action(() =>
            {
                btnOpen.Enabled = false;
                btnClose.Enabled = true;
                btnStart.Enabled = true;
                btnStop.Enabled = false;
                btnSoftware.Enabled = false;
                btnSaveBMP.Enabled = false;
                btnSaveBIN.Enabled = false;
                btnSetParam.Enabled = true;
                btnGetParam.Enabled = true;

                cbPixelType.Enabled = true;
                txtExposuretime.Enabled = true;
                txtGainRaw.Enabled = true;

                chbSoftTrigger.Enabled = true;

                radioContinue.Enabled = true;
                radioTrigger.Enabled = true;
            }));
        }

        private void SetCtrlWhenClose()
        {
            this.Invoke(new Action(() =>
            {
                btnOpen.Enabled = true;
                btnClose.Enabled = false;
                btnStart.Enabled = false;
                btnStop.Enabled = false;
                btnSoftware.Enabled = false;
                btnSaveBMP.Enabled = false;
                btnSaveBIN.Enabled = false;
                btnSetParam.Enabled = false;
                btnGetParam.Enabled = false;

                cbPixelType.Enabled = false;
                txtExposuretime.Enabled = false;
                txtGainRaw.Enabled = false;

                chbSoftTrigger.Enabled = false;

                radioContinue.Enabled = false;
                radioTrigger.Enabled = false;
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
                cbPixelType.Enabled = false;

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
                cbPixelType.Enabled = true;
            }));
        }

        private void btnGetParam_Click(object sender, EventArgs e)
        {
            getParam();
        }

        private void getParam()
        {
            int res = IMVDefine.IMV_OK;
            cbPixelType.Text = string.Empty;
            //获取图像格式                   
            IMVDefine.IMV_String pixelType = new IMVDefine.IMV_String();
            res = cam.IMV_GetEnumFeatureSymbol("PixelFormat", ref pixelType);
            if (IMVDefine.IMV_OK != res)
            {
                MessageBox.Show("get image pixle format failed!");
            }
            cbPixelType.SelectedText = pixelType.str;

            // 获取曝光值
            // set ExposureTime 
            double exposureTime = 0;
            res = cam.IMV_GetDoubleFeatureValue("ExposureTime", ref exposureTime);
            if (IMVDefine.IMV_OK != res)
            {
                MessageBox.Show("get exposuretime failed!");
            }
            txtExposuretime.Text = exposureTime.ToString("f2");

            // 设置增益 
            // set Gain 
            double gain = 0;
            res = cam.IMV_GetDoubleFeatureValue("GainRaw", ref gain);
            if (IMVDefine.IMV_OK != res)
            {
                MessageBox.Show("get gainraw failed!");
            }
            txtGainRaw.Text = gain.ToString("f2");
        }

        private void radioContinue_CheckedChanged(object sender, EventArgs e)
        {
            if (radioContinue.Checked)
            {
                //关闭触发模式
                //Close Trigger Mode
                int res = IMVDefine.IMV_OK;
                res = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "Off");
                if (IMVDefine.IMV_OK != res)
                {
                    MessageBox.Show("Set Trigger Mode failed!");
                    return;
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
                //打开触发模式
                //Open Trigger Mode
                int res = IMVDefine.IMV_OK;
                res = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "On");
                if (IMVDefine.IMV_OK != res)
                {
                    MessageBox.Show("Set Trigger Mode failed!");
                    return;
                }

                if (chbSoftTrigger.Checked)
                {
                    //设置触发源为软触发
                    //Set Trigger Source to Software
                    res = cam.IMV_SetEnumFeatureSymbol("TriggerSource", "Software");
                    if (IMVDefine.IMV_OK != res)
                    {
                        MessageBox.Show("Set Trigger Source failed!");
                        return;
                    }

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
                    res = cam.IMV_SetEnumFeatureSymbol("TriggerSource", "Line1");
                    if (IMVDefine.IMV_OK != res)
                    {
                        MessageBox.Show("Set Trigger Source failed!");
                        return;
                    }

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
            int res = IMVDefine.IMV_OK;
            if (chbSoftTrigger.Checked)
            {
                //设置触发源为软触发
                //Set Trigger Source to Software
                res = cam.IMV_SetEnumFeatureSymbol("TriggerSource", "Software");
                if (IMVDefine.IMV_OK != res)
                {
                    MessageBox.Show("Set Trigger Source failed!");
                    return;
                }

                this.Invoke(new Action(() =>
                {
                    btnSoftware.Enabled = true;
                }));
            }
            else
            {
                //设置触发源为Line1
                //Set Trigger Source to Line1
                res = cam.IMV_SetEnumFeatureSymbol("TriggerSource", "Line1");
                if (IMVDefine.IMV_OK != res)
                {
                    MessageBox.Show("Set Trigger Source failed!");
                    return;
                }

                this.Invoke(new Action(() =>
                {
                    btnSoftware.Enabled = false;
                }));
            }
        }
    }
}
