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
    public class Camera
    {
        private List<FrameInfo> m_frameList = new List<FrameInfo>(); // 图像缓存列表 | frame data list 
        private Mutex m_mutex = new Mutex(); // 锁，保证多线程安全 | mutex 
        private Thread renderThread = null; // 显示线程 | image display thread 
        private bool m_bShowLoop = true; // 线程控制变量 | thread looping flag 
        private IMVDefine.IMV_FrameCallBack frameCallBack;
        private Render m_Render = null;
        private int res = IMVDefine.IMV_OK;
        private MyCamera cam = new MyCamera();
        private IntPtr m_pDstData = IntPtr.Zero;
        private int m_iDstDataSize = 0;
        private bool bOpen = false;

        public Camera(Render render)
        {
            m_Render = render;
            m_Render.Open();
            if (null == renderThread)
            {
                renderThread = new Thread(new ThreadStart(ShowThread));
                renderThread.Start();
            }
            m_stopWatch.Start();
        }

        public bool Open(int cameraId)
        {

            // 创建设备句柄
            // Create Device Handle
            res = cam.IMV_CreateHandle(IMVDefine.IMV_ECreateHandleMode.modeByIndex, cameraId);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Create devHandle failed! ErrorCode[{0}]", res));
                return false;
            }

            // 打开相机设备 
            // Connect to camera 
            res = cam.IMV_Open();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Open camera failed! ErrorCode:[{0}]", res));
                return false;
            }

            //设置缓存个数为8
            //set buffer count to 8
            res = cam.IMV_SetBufferCount(8);


            // 注册数据帧回调函数
            // Register data frame callback function
            frameCallBack = onGetFrame;
            res = cam.IMV_AttachGrabbing(frameCallBack, IntPtr.Zero);
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Attach grabbing failed! ErrorCode:[{0}]", res));
                return false;
            }

            // 开始拉流 
            // Start grabbing
            res = cam.IMV_StartGrabbing();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Start grabbing failed! ErrorCode[{0}]", res));
                return false;
            }
            bOpen = true;
            return true;
        }

        public bool Close()
        {
            // 停止拉流 
            // Stop grabbing 
            res = cam.IMV_StopGrabbing();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Stop grabbing failed! ErrorCode[{0}]", res));
                return false;
            }

            renderThread.Join(200);

            //关闭相机
            //Close cam 
            res = cam.IMV_Close();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Close camera failed! ErrorCode[{0}]", res));
                return false;
            }

            // 销毁设备句柄
            // Destroy Device Handle
            res = cam.IMV_DestroyHandle();
            if (res != IMVDefine.IMV_OK)
            {
                MessageBox.Show(String.Format("Destroy camera failed! ErrorCode[{0}]", res));
                return false;
            }

            bOpen = false;
            return true;
        }


        // 码流数据回调 
        // grab callback function 
        private void onGetFrame(ref IMVDefine.IMV_Frame frame, IntPtr pUser)
        {
            m_mutex.WaitOne();
            m_frameList.Add(CloneFrame(ref frame));
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

                // 图像队列取最新帧 
                // always get the latest frame in list 
                m_mutex.WaitOne();
                FrameInfo frame = m_frameList.ElementAt(m_frameList.Count - 1);
                m_frameList.Clear();
                m_mutex.ReleaseMutex();

                // 主动调用回收垃圾 
                // call garbage collection 
                GC.Collect();

                // 控制显示最高帧率为25FPS 
                // control frame display rate to be 25 FPS 
                if (false == isTimeToDisplay())
                {
                    continue;
                }

                if (frame.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelMono8)
                {
                    m_pDstData = Marshal.UnsafeAddrOfPinnedArrayElement(frame.pData, 0);
                    m_Render.Display(m_pDstData, frame.width, frame.height,
                        Render.VR_PIXEL_TYPE_E.VR_PIXEL_FMT_MONO8);
                }
                else
                {
                    if (ConvertToBGR24(ref frame))
                        m_Render.Display(m_pDstData, frame.width, frame.height,
                            Render.VR_PIXEL_TYPE_E.VR_PIXEL_FMT_RGB24);
                }
            }
        }

        //转码函数
        //transcoding function
        private bool ConvertToBGR24(ref FrameInfo frame)
        {
            IMVDefine.IMV_PixelConvertParam stPixelConvertParam = new IMVDefine.IMV_PixelConvertParam();

            //当内存申请失败，返回false
            try
            {
                if (m_pDstData == IntPtr.Zero || frame.size * 3 > m_iDstDataSize)
                {
                    if (m_pDstData != IntPtr.Zero)
                    {
                        Marshal.FreeHGlobal(m_pDstData);
                        m_pDstData = IntPtr.Zero;
                    }
                    m_pDstData = Marshal.AllocHGlobal(frame.size * 3);
                    m_iDstDataSize = frame.size * 3;
                }
            }
            catch
            {
                return false;
            }


            // 图像转换成BGR8
            // convert image to BGR8
            stPixelConvertParam.nWidth = (uint)frame.width;
            stPixelConvertParam.nHeight = (uint)frame.height;
            stPixelConvertParam.ePixelFormat = frame.pixelFormat;
            stPixelConvertParam.pSrcData = Marshal.UnsafeAddrOfPinnedArrayElement(frame.pData, 0);
            stPixelConvertParam.nSrcDataLen = (uint)frame.size;
            stPixelConvertParam.nPaddingX = frame.paddingX;
            stPixelConvertParam.nPaddingY = frame.paddingY;
            stPixelConvertParam.eBayerDemosaic = IMVDefine.IMV_EBayerDemosaic.demosaicBilinear;
            stPixelConvertParam.eDstPixelFormat = IMVDefine.IMV_EPixelType.gvspPixelBGR8;
            stPixelConvertParam.pDstBuf = m_pDstData;
            stPixelConvertParam.nDstBufSize = (uint)m_iDstDataSize;

            res = cam.IMV_PixelConvert(ref stPixelConvertParam);
            if (res == IMVDefine.IMV_OK) return true;
            Console.WriteLine("image convert to BGR8 failed! ErrorCode[{0}]", res);
            return false;

        }

        public void Dispose()
        {
            if (bOpen) Close();
            m_bShowLoop = false;
            renderThread.Join();
            if (m_pDstData != IntPtr.Zero && m_iDstDataSize != 0)
            {
                Marshal.FreeHGlobal(m_pDstData);
                m_pDstData = IntPtr.Zero;
            }
            m_Render.Close();
            m_Render = null;
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

        public struct FrameInfo
        {
            public byte[] pData;
            public int width;
            public int height;
            public ulong blockId;
            public IMVDefine.IMV_EPixelType pixelFormat;
            public uint paddingX;
            public uint paddingY;
            public int size;
        }

        private FrameInfo CloneFrame(ref IMVDefine.IMV_Frame frame)
        {
            FrameInfo frameClone = new FrameInfo();
            frameClone.pData = new byte[frame.frameInfo.size];

            Marshal.Copy(frame.pData, frameClone.pData, 0, (int)frame.frameInfo.size);
            frameClone.width = (int)frame.frameInfo.width;
            frameClone.height = (int)frame.frameInfo.height;
            frameClone.blockId = frame.frameInfo.blockId;
            frameClone.pixelFormat = frame.frameInfo.pixelFormat;
            frameClone.paddingX = frame.frameInfo.paddingX;
            frameClone.paddingY = frame.frameInfo.paddingY;
            frameClone.size = (int)frame.frameInfo.size;
            return frameClone;
        }
    }
}
