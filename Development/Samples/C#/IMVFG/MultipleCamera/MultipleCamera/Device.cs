using System;
using System.Collections.Generic;
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

namespace MultipleCamera
{
    class Device
    {
        private List<IMVFGDefine.IMV_FG_Frame> m_frameList = new List<IMVFGDefine.IMV_FG_Frame>(); // 图像缓存列表 | frame data list 
        private Mutex m_mutex = new Mutex();        // 锁，保证多线程安全 | mutex 
        private Thread renderThread = null;         // 显示线程 | image display thread 
        private bool m_bShowLoop = true;            // 线程控制变量 | thread looping flag 
        private IMVFGDefine.IMV_FG_FrameCallBack frameCallBack;
        private Render m_Render = null;
        private int res = IMVFGDefine.IMV_FG_OK;

        private CamDev camera = new CamDev();
        private CardDev card = new CardDev();

        private IntPtr m_pDstData = IntPtr.Zero;
        private int m_iDstDataSize = 0;
        private bool bOpen = false;

        public Device(Render render)
        {
            m_Render = render;
            m_Render.Open();
            if (null == renderThread)
            {
                renderThread = new Thread(new ThreadStart(ShowThread));
                renderThread.Start();
            }
            frameCallBack = new IMVFGDefine.IMV_FG_FrameCallBack(OnImageGrabbed);
            m_stopWatch.Start();
        }

        // 打开采集卡和相机(使用对应的相机和卡的CameraKey)
        // Open the card and camera (using the corresponding camera and card's CameraKey)
        public bool Open(string cardKey, string cameraKey)
        {

            // 打开采集卡设备
            // Connect to card
            res = card.IMV_FG_OpenInterfaceEx(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_CAMERAKEY, cameraStr: cardKey);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Open camera failed!");
                return false;
            }

            // 打开采集卡相机设备 
            // Connect to camera 
            res = camera.IMV_FG_OpenDevice(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_CAMERAKEY, cameraStr: cameraKey);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Open camera failed!");
                return false;
            }

            res = camera.IMV_FG_SetEnumFeatureSymbol("TriggerMode", "Off");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Set enum feature value failed!");
                return false;
            }

            res = card.IMV_FG_AttachGrabbing(frameCallBack, IntPtr.Zero);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Set feature value failed!");
                return false;
            }

            // 开始拉流 
            // Start grabbing
            res = card.IMV_FG_StartGrabbing();
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Start grabbing failed!");
                return false;
            }
            bOpen = true;
            return true;

        }

        public bool Close()
        {

            // 停止拉流 
            // Stop grabbing 
            res = card.IMV_FG_StopGrabbing();
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Stop grabbing failed!");
                return false;
            }

            // 等待所有帧数据显示完毕
            // Wait for all frame data to be displayed
            while (m_frameList.Count > 0)
            {
                Thread.Sleep(100);
            }

            // 关闭相机
            // Close camera 
            res = camera.IMV_FG_CloseDevice();
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Close camera failed!");
                return false;
            }

            // 关闭采集卡
            // Close capture device 
            res = card.IMV_FG_CloseInterface();
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                MessageBox.Show("Close cameralink board failed!");
                return false;
            }
            bOpen = false;

            return true;
        }

        public void OnImageGrabbed(ref IMVFGDefine.IMV_FG_Frame pFrame, IntPtr pUser)
        {
            // 可以使用IMV_FG_CloneFrame拷贝帧数据，但是必须要调用IMV_FG_ReleaseFrame释放内存
            // It is possible to copy frame data using IMV_FG_CloneFrame-CloneFrame, but it is necessary to call IMV_FG_ReleaseFrame to free up memory
            IMVFGDefine.IMV_FG_Frame frameclone = new IMVFGDefine.IMV_FG_Frame();
            card.IMV_FG_CloneFrame(ref pFrame, ref frameclone);
            m_mutex.WaitOne();
            m_frameList.Add(frameclone);
            m_mutex.ReleaseMutex();
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

                // 图像队列取第一帧 
                // always get the first frame in list 
                m_mutex.WaitOne();
                IMVFGDefine.IMV_FG_Frame frame = m_frameList.ElementAt(0);
                m_frameList.RemoveAt(0);
                m_mutex.ReleaseMutex();

                // 主动调用回收垃圾 
                // call garbage collection 
                GC.Collect();

                try
                {
                    // 控制显示最高帧率为25FPS 
                    // control frame display rate to be 25 FPS 
                    if (false == isTimeToDisplay())
                    {
                        // 必须调用IMV_FG_ReleaseFrame()主动释放缓存
                        // Must call IMV_FG_ReleaseFrame() to actively release the cache
                        card.IMV_FG_ReleaseFrame(ref frame);
                        continue;
                    }

                    if (frame.frameInfo.pixelFormat == IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8)
                    {
                        m_Render.Display(frame.pData, (int)frame.frameInfo.width, (int)frame.frameInfo.height,
                            Render.VR_PIXEL_TYPE_E.VR_PIXEL_FMT_MONO8);
                    }
                    else
                    {
                        if (ConvertToBGR24(ref frame))
                            m_Render.Display(m_pDstData, (int)frame.frameInfo.width, (int)frame.frameInfo.height,
                                Render.VR_PIXEL_TYPE_E.VR_PIXEL_FMT_RGB24);
                    }

                    // 必须调用IMV_FG_ReleaseFrame()主动释放缓存
                    // Must call IMV_FG_ReleaseFrame() to actively release the cache
                    card.IMV_FG_ReleaseFrame(ref frame);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        //转码函数
        //transcoding function
        private bool ConvertToBGR24(ref IMVFGDefine.IMV_FG_Frame frame)
        {
            IMVFGDefine.IMV_FG_PixelConvertParam stPixelConvertParam = new IMVFGDefine.IMV_FG_PixelConvertParam();

            //当内存申请失败，返回false
            try
            {
                if (m_pDstData == IntPtr.Zero || frame.frameInfo.size * 3 > m_iDstDataSize)
                {
                    if (m_pDstData != IntPtr.Zero)
                    {
                        Marshal.FreeHGlobal(m_pDstData);
                        m_pDstData = IntPtr.Zero;
                    }
                    m_pDstData = Marshal.AllocHGlobal((int)frame.frameInfo.size * 3);
                    m_iDstDataSize = (int)frame.frameInfo.size * 3;
                }
            }
            catch
            {
                if (m_pDstData != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(m_pDstData);
                    m_pDstData = IntPtr.Zero;
                }
                m_iDstDataSize = 0;
                return false;
            }

            // 图像转换成BGR8
            // convert image to BGR8
            stPixelConvertParam.nWidth = (uint)frame.frameInfo.width;
            stPixelConvertParam.nHeight = (uint)frame.frameInfo.height;
            stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat;
            stPixelConvertParam.pSrcData = frame.pData;
            stPixelConvertParam.nSrcDataLen = (uint)frame.frameInfo.size;
            stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX;
            stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY;
            stPixelConvertParam.eBayerDemosaic = IMVFGDefine.IMV_FG_EBayerDemosaic.IMV_FG_DEMOSAIC_BILINEAR;
            stPixelConvertParam.eDstPixelFormat = IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8;
            stPixelConvertParam.pDstBuf = m_pDstData;
            stPixelConvertParam.nDstBufSize = (uint)m_iDstDataSize;

            res = card.IMV_FG_PixelConvert(ref stPixelConvertParam);
            if (res == IMVFGDefine.IMV_FG_OK) return true;
            return false;
        }

        public void Dispose()
        {
            if (bOpen) Close();
            m_bShowLoop = false;
            renderThread.Join();//将显示线程暂停，防止因为m_pDstData释放导致报错
            if (m_pDstData != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(m_pDstData);
                m_pDstData = IntPtr.Zero;
            }
            m_Render.Close();
            m_Render = null;

        }

        const int DEFAULT_INTERVAL = 40;
        Stopwatch m_stopWatch = new Stopwatch();

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
    }
}
