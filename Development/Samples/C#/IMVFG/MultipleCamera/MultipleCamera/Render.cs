using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace MultipleCamera
{
    public class Render
    {

        private const string SdkFilePath = @".\VideoRender.dll";
        public bool m_IsOpen;
        public IntPtr m_wind;
        public IntPtr m_handler;
        public VR_OPEN_PARAM_S m_params;

        /// <summary>
        /// 生成一个播放对象
        /// </summary>
        /// <param name="pParam">VR_OPEN_PARAM_S结构体。生成播放对象所需要的参数</param>
        /// <param name="pHandle">IntPtr类型，输出生成的播放的对象</param>
        /// <returns>成功返回VR_Success，其它值见VR_ERR_E枚举</returns>
        /// 执行成功后，需要调用VR_Close来释放资源
        [DllImport(SdkFilePath, CallingConvention = CallingConvention.StdCall)]
        public static extern VR_ERR_E VR_Open(ref VR_OPEN_PARAM_S pParam, ref IntPtr pHandle);

        /// <summary>
        /// 将输入的一帧数据显示出图像
        /// </summary>
        /// <param name="handle">VR_HANDLE句柄，由VR_OPEN创建</param>
        /// <param name="param">VR_FRAME_S结构体</param>
        /// <param name="pEnlargeRect">VR_Rect结构体，放大矩形区域。原图大小显示，不需要放大时，填原图大小</param>
        /// <returns>成功返回VR_Success，其它值见VR_ERR_E枚举</returns>
        [DllImport(SdkFilePath, CallingConvention = CallingConvention.StdCall)]
        public static extern VR_ERR_E VR_RenderFrame(IntPtr handle, ref VR_FRAME_S param, ref VR_Rect pEnlargeRect);

        /// <summary>
        /// 关闭播放对象
        /// </summary>
        /// <param name="handle">VR_HANDLE句柄，由VR_Open创建</param>
        /// <returns>成功返回VR_Success，其它值见VR_ERR_E枚举</returns>
        [DllImport(SdkFilePath, CallingConvention = CallingConvention.StdCall)]
        public static extern VR_ERR_E VR_Close(IntPtr handle);

        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public struct VR_FRAME_S
        {
            /// unsigned char*[4]
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray,
                SizeConst = 4, ArraySubType = System.Runtime.InteropServices.UnmanagedType.SysUInt)] public IntPtr[]
                data;

            /// int[4]
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray,
                SizeConst = 4, ArraySubType = System.Runtime.InteropServices.UnmanagedType.I4)] public int[] steide;

            public int nWidth;

            public int nHeight;
            public VR_PIXEL_TYPE_E format;
        }

        /// <summary>
        /// 矩形
        /// </summary>
        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public struct VR_Rect
        {
            public int left;
            public int top;
            public int right;
            public int bottom;
        }

        /// <summary>
        /// 接口返回值
        /// </summary>
        public enum VR_ERR_E
        {
            VR_Success,
            VR_ILLEGAL_PARAM, //非法参数
            VR_ERR_ORDER, //调用接口顺序错误
            VR_NO_MEMORY, //内存不足
            VR_NOT_SUPPORT, //不支持，或是缺少系统库，或是不支持输入的参数值
            VR_D3D_PRESENT_FAILED, //D3D显示错误
            VR_GDI_CREATE_OBJ_FAILED, //GDI创建对象失败
            VR_DEFAULT_FONT_NOT_EXIST, //默认字体不存在
        }

        public enum VR_MODE_E
        {
            VR_MODE_D3D = 0, //使用D3D的方式进行显示
            VR_MODE_GDI, //使用GDI显示模式
            VR_MODE_OPENGLX, //使用Opengl的方式进行显示
            VR_MODE_X11, //使用X11的方式进行显示
        }

        /// <summary>
        /// 帧像素类型
        /// </summary>
        public enum VR_PIXEL_TYPE_E
        {
            VR_PIXEL_FMT_NONE = -1,
            VR_PIXEL_FMT_YUV420P,
            VR_PIXEL_FMT_RGB24,
            VR_PIXEL_FMT_MONO8,
        }

        /// <summary>
        /// 打开视频显示所需要的参数，供VR_OPEN接口使用
        /// </summary>
        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public struct VR_OPEN_PARAM_S
        {
            public IntPtr hWnd; //窗口句柄
            public VR_MODE_E eVideoRenderMode; //显示模式
            public int nWidth; //视频宽
            public int nHeight; //视频高
        }

        public Render(IntPtr wind)
        {
            m_wind = wind;
            m_handler = IntPtr.Zero;
        }

        public bool Open(int width = 16, int height = 16)
        {
            if (m_wind == null || m_wind == IntPtr.Zero)
            {
                return false;
            }

            m_params.eVideoRenderMode = VR_MODE_E.VR_MODE_GDI;
            m_params.hWnd = m_wind;
            m_params.nWidth = width;
            m_params.nHeight = height;

            VR_ERR_E ret = VR_Open(ref m_params, ref m_handler);
            if (VR_ERR_E.VR_Success != ret)
            {
                return false;
            }

            m_IsOpen = true;
            return true;
        }

        public bool Display(IntPtr displayBuffer, int iWidth, int iHeight, VR_PIXEL_TYPE_E iPixelFormat)
        {
            if (displayBuffer == IntPtr.Zero || iWidth == 0 || iHeight == 0)
            {
                return false;
            }

            if (m_IsOpen && (m_params.nWidth != iWidth || m_params.nHeight != iHeight))
            {
                Close();
            }

            if (!m_IsOpen)
            {
                Open(iWidth, iHeight);
            }

            if (m_IsOpen)
            {
                VR_FRAME_S renderParam = new VR_FRAME_S();
                renderParam.data = new IntPtr[4];
                renderParam.steide = new int[4];
                renderParam.data[0] = displayBuffer;
                renderParam.steide[0] = iWidth;
                renderParam.nWidth = iWidth;
                renderParam.nHeight = iHeight;

                if (iPixelFormat == VR_PIXEL_TYPE_E.VR_PIXEL_FMT_MONO8)
                {
                    renderParam.format = VR_PIXEL_TYPE_E.VR_PIXEL_FMT_MONO8;
                }
                else
                {
                    renderParam.format = VR_PIXEL_TYPE_E.VR_PIXEL_FMT_RGB24;
                }

                VR_Rect rect = new VR_Rect()
                {
                    left = 0,
                    right = iWidth,
                    top = 0,
                    bottom = iHeight
                };

                VR_ERR_E ret = VR_RenderFrame(m_handler, ref renderParam, ref rect);
                if (VR_ERR_E.VR_Success != ret)
                {
                    return false;
                }
            }
            return false;
        }

        public bool Close()
        {
            if (m_handler != IntPtr.Zero)
            {
                VR_Close(m_handler);
                m_handler = IntPtr.Zero;
                m_IsOpen = false;
            }
            return true;
        }
    }
}
