using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;
using MVSDK_Net;

namespace SaveImageToBmp
{
    internal class Program
    {
        private static MyCamera cam = new MyCamera();
        private const string SdkFilePath = @".\ImageConvert.dll";
        private static string m_binSavePath = Environment.CurrentDirectory + @"\Bins";
        private static string m_bitMapSavePath = Environment.CurrentDirectory + @"\BitMaps";

        /// <summary>
        /// 指针之间进行数据拷贝
        /// </summary>
        /// <param name="pDst">目标地址</param>
        /// <param name="pSrc">源地址</param>
        /// <param name="len">拷贝数据长度</param>
        [DllImport("Kernel32.dll", EntryPoint = "RtlMoveMemory", CharSet = CharSet.Ansi)]
        internal static extern void CopyMemory(IntPtr pDst, IntPtr pSrc, int len);

        private static void Main(string[] args)
        {
            Console.WriteLine("SDK Version:{0}", MyCamera.IMV_GetVersion());
            Console.WriteLine("Enum camera device.");

            //枚举设备
            // Discover device
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_DeviceList deviceList = new IMVDefine.IMV_DeviceList();
            IMVDefine.IMV_EInterfaceType interfaceTp = IMVDefine.IMV_EInterfaceType.interfaceTypeAll;
            res = MyCamera.IMV_EnumDevices(ref deviceList, (uint) interfaceTp);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Enumeration devices failed! ErrorCode:[{0}]", res);
                Console.Read();
                return;
            }
            if (deviceList.nDevNum < 1)
            {
                Console.WriteLine("No device find. ErrorCode:[{0}]", res);
                Console.Read();
                return;
            }

            // 打印设备基本信息
            // Print device info 
            displayDeviceInfo(deviceList);

            int nCamIndex = 0;
            Console.Write("Please input the camera index: ");
            try
            {
                nCamIndex = Convert.ToInt32(Console.ReadLine());
            }
            catch
            {
                Console.WriteLine("Invalid Input!");
                Console.Read();
                return;
            }
            if (nCamIndex > deviceList.nDevNum - 1 || nCamIndex < 0)
            {
                Console.WriteLine("Input Error!");
                Console.Read();
                return;
            }

            do
            {
                // 创建设备句柄
                // Create Device Handle
                res = cam.IMV_CreateHandle(IMVDefine.IMV_ECreateHandleMode.modeByIndex, nCamIndex);
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Create devHandle failed! ErrorCode[{0}]", res);
                    break;
                }

                Console.WriteLine("Open camera device.");

                // 打开相机设备 
                // Connect to camera 
                res = cam.IMV_Open();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Open camera failed! ErrorCode:[{0}]", res);
                    break;
                }

                // 开始拉流 
                // Start grabbing 
                res = cam.IMV_StartGrabbing();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Start grabbing failed! ErrorCode:[{0}]", res);
                    break;
                }

                //取图
                //get frame 
                IMVDefine.IMV_Frame frame = new IMVDefine.IMV_Frame();
                res = cam.IMV_GetFrame(ref frame, 1000);
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Get frame failed! ErrorCode:[{0}]", res);
                    break;
                }

                //保存bin文件
                //save bin file
                if (!SaveToBin(ref frame))
                {
                    Console.WriteLine("Save Bin failed");
                }

                //保存bitmap文件
                //save to bitmap
                if (!SaveToBitmap(ref frame))
                {
                    Console.WriteLine("Save Bitmap failed");
                }

                // 释放图像缓存
                // Free image buffer
                res = cam.IMV_ReleaseFrame(ref frame);
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Release frame failed! ErrorCode:[{0}]", res);
                    break;
                }

                // 停止拉流 
                // Stop grabbing 
                res = cam.IMV_StopGrabbing();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Stop grabbing failed! ErrorCode:[{0}]", res);
                    break;
                }

                //关闭相机
                //Close camera 
                res = cam.IMV_Close();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Close camera failed! ErrorCode:[{0}]", res);
                    break;
                }

                // 销毁设备句柄
                // Destroy Device Handle
                res = cam.IMV_DestroyHandle();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Destroy camera failed! ErrorCode[{0}]", res);
                    break;
                }
            } while (false);

            if (res != IMVDefine.IMV_OK)
            {
                // 销毁设备句柄
                // Destroy Device Handle
                res = cam.IMV_DestroyHandle();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Destroy camera failed! ErrorCode[{0}]", res);
                }
            }

            Console.WriteLine("Press enter to exit");
            Console.Read();
        }

        //保存为bin文件
        //Save to Bin
        private static bool SaveToBin(ref IMVDefine.IMV_Frame frame)
        {
            byte[] pBuffer = new byte[frame.frameInfo.size];
            Marshal.Copy(frame.pData, pBuffer, 0, (int) frame.frameInfo.size);
            if (!Directory.Exists(m_binSavePath))
            {
                Directory.CreateDirectory(m_binSavePath);
            }
            try
            {
                string binPath = m_binSavePath + @"\" + frame.frameInfo.blockId + ".bin";
                using (Stream filePath = new FileStream(binPath, FileMode.Create))
                {
                    using (BinaryWriter sw = new BinaryWriter(filePath)) //建立二进制文件流
                    {
                        sw.Write(pBuffer);
                    }
                }
                Console.WriteLine("Save bin Successfully!Save path is [{0}]", binPath);
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception.ToString());
                return false;
            }
            return true;
        }

        //保存为Bitmap
        //Save to Bitmap
        private static bool SaveToBitmap(ref IMVDefine.IMV_Frame frame)
        {
            Bitmap bitmap = null;
            if (!ConvertToBitmap(ref frame, ref bitmap))
            {
                return false;
            }
            if (!Directory.Exists(m_bitMapSavePath))
            {
                Directory.CreateDirectory(m_bitMapSavePath);
            }
            try
            {
                string imageName = m_bitMapSavePath + @"\" + frame.frameInfo.blockId + ".bmp";
                bitmap.Save(imageName);
                bitmap.Dispose();
                Console.WriteLine("Save bitmap Successfully!Save path is [{0}]", imageName);
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception.ToString());
                if (bitmap != null)
                {
                    bitmap.Dispose();
                }
                return false;
            }
            return true;
        }

        private static bool ConvertToBitmap(ref IMVDefine.IMV_Frame frame, ref Bitmap bitmap)
        {
            IntPtr pDstRGB = IntPtr.Zero;
            BitmapData bmpData;
            Rectangle bitmapRect = new Rectangle();
            IMVDefine.IMV_PixelConvertParam stPixelConvertParam = new IMVDefine.IMV_PixelConvertParam();
            int res = IMVDefine.IMV_OK;

            // mono8和BGR8裸数据不需要转码
            // mono8 and BGR8 raw data is not need to convert
            if (frame.frameInfo.pixelFormat != IMVDefine.IMV_EPixelType.gvspPixelMono8
                && frame.frameInfo.pixelFormat != IMVDefine.IMV_EPixelType.gvspPixelBGR8)
            {
                //转目标内存 彩色
                var ImgSize = (int) frame.frameInfo.width*(int) frame.frameInfo.height*3;

                //当内存申请失败，返回false
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

                // 图像转换成BGR8
                // convert image to BGR8
                stPixelConvertParam.nWidth = frame.frameInfo.width;
                stPixelConvertParam.nHeight = frame.frameInfo.height;
                stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat;
                stPixelConvertParam.pSrcData = frame.pData;
                stPixelConvertParam.nSrcDataLen = frame.frameInfo.size;
                stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX;
                stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY;
                stPixelConvertParam.eBayerDemosaic = IMVDefine.IMV_EBayerDemosaic.demosaicNearestNeighbor;
                stPixelConvertParam.eDstPixelFormat = IMVDefine.IMV_EPixelType.gvspPixelBGR8;
                stPixelConvertParam.pDstBuf = pDstRGB;
                stPixelConvertParam.nDstBufSize = (uint) ImgSize;
                res = cam.IMV_PixelConvert(ref stPixelConvertParam);
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("image convert to BGR failed!");
                    return false;
                }
            }
            else
            {
                pDstRGB = frame.pData;
            }

            if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelMono8)
            {
                // 用Mono8数据生成Bitmap
                bitmap = new Bitmap((int)frame.frameInfo.width, (int)frame.frameInfo.height, PixelFormat.Format8bppIndexed);
                ColorPalette colorPalette = bitmap.Palette;
                for (int i = 0; i != 256; ++i)
                {
                    colorPalette.Entries[i] = Color.FromArgb(i, i, i);
                }
                bitmap.Palette = colorPalette;

                bitmapRect.Height = bitmap.Height;
                bitmapRect.Width = bitmap.Width;
                bmpData = bitmap.LockBits(bitmapRect, ImageLockMode.ReadWrite, bitmap.PixelFormat);
                CopyMemory(bmpData.Scan0, pDstRGB, bmpData.Stride * bitmap.Height);
                bitmap.UnlockBits(bmpData);
            }
            else
            {
                // 用BGR24数据生成Bitmap
                bitmap = new Bitmap((int)frame.frameInfo.width, (int)frame.frameInfo.height, PixelFormat.Format24bppRgb);
                bitmapRect.Height = bitmap.Height;
                bitmapRect.Width = bitmap.Width;
                bmpData = bitmap.LockBits(bitmapRect, ImageLockMode.ReadWrite, bitmap.PixelFormat);
                CopyMemory(bmpData.Scan0, pDstRGB, bmpData.Stride * bitmap.Height);
                bitmap.UnlockBits(bmpData);
                if (frame.frameInfo.pixelFormat != IMVDefine.IMV_EPixelType.gvspPixelBGR8)
                {
                    Marshal.FreeHGlobal(pDstRGB);
                }
            }

            return true;
        }

        #region 展示设备列表

        private static void displayDeviceInfo(IMVDefine.IMV_DeviceList deviceInfoList)
        {

            Console.WriteLine("Enum device size : {0}\n", deviceInfoList.nDevNum);
            for (int i = 0; i < deviceInfoList.nDevNum; i++)
            {
                IMVDefine.IMV_DeviceInfo deviceInfo =
                    (IMVDefine.IMV_DeviceInfo)
                        Marshal.PtrToStructure(
                            deviceInfoList.pDevInfo + Marshal.SizeOf(typeof (IMVDefine.IMV_DeviceInfo))*i,
                            typeof (IMVDefine.IMV_DeviceInfo));

                // 相机设备列表的索引
                // Device index in device list
                Console.WriteLine("Camera index : {0}", i);
                // 接口类型（GigE，U3V，CL，PCIe）
                // Interface type 
                Console.WriteLine("nInterfaceType : {0}", deviceInfo.nInterfaceType);
                // 设备ID信息
                // Device ID
                Console.WriteLine("cameraKey : {0}", deviceInfo.cameraKey);
                // 设备的型号信息
                // Device model name
                Console.WriteLine("modelName : {0}", deviceInfo.modelName);
                // 设备的序列号
                // Device serial number
                Console.WriteLine("serialNumber : {0}", deviceInfo.serialNumber);

                if (deviceInfo.nCameraType == IMVDefine.IMV_ECameraType.typeGigeCamera)
                {
                    IMVDefine.IMV_GigEDeviceInfo gigEDeviceInfo =
                        (IMVDefine.IMV_GigEDeviceInfo)
                            ByteToStruct(deviceInfo.deviceSpecificInfo.gigeDeviceInfo,
                                typeof (IMVDefine.IMV_GigEDeviceInfo));

                    Console.WriteLine("ipAddress : {0}", gigEDeviceInfo.ipAddress);
                }
                Console.WriteLine();
            }

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

        #endregion
    }
}
