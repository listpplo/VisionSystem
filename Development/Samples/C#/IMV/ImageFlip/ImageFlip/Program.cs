using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MVSDK_Net;

namespace ImageFlip
{
    internal class Program
    {
        private static MyCamera cam = new MyCamera();
        private const int MONO_CHANNEL_NUM = 1;
        private const int RGB_CHANNEL_NUM = 3;
        private const int BGR_CHANNEL_NUM = 3;

        private static void Main(string[] args)
        {

            Console.WriteLine("SDK Version:{0}", MyCamera.IMV_GetVersion());
            Console.WriteLine("Enum camera device.");
            IMVDefine.IMV_Frame frame = new IMVDefine.IMV_Frame();

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

                // 获取一帧图像
                // Get a frame image
                res = cam.IMV_GetFrame(ref frame, 500);
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Get frame failed! ErrorCode[{0}]", res);
                    break;
                }

                // 选择图像翻转方式
                // Select image flip type
                Console.WriteLine("\n--------------------------------------------");
                Console.WriteLine("\t0.Image Vertical Flip");
                Console.WriteLine("\t1.Image Horizontal Flip");
                Console.WriteLine("--------------------------------------------");
                int imageFlipType = 0;
                Console.Write("Please select the flip type index: ");
                try
                {
                    imageFlipType = Convert.ToInt32(Console.ReadLine());
                }
                catch
                {
                    Console.WriteLine("Invalid Input!Use default flip way:Image Vertical Flip");
                    imageFlipType = 0;
                }
                if (imageFlipType > 1 || imageFlipType < 0)
                {
                    Console.WriteLine("Input Error!Use default flip way:Image Vertical Flip");
                    imageFlipType = 0;
                }

                Console.WriteLine("BlockId ({0}) pixelFormat ({1}) imageFilpType({2}), Start image flip...",
                    frame.frameInfo.blockId, frame.frameInfo.pixelFormat, (IMVDefine.IMV_EFlipType) imageFlipType);

                // 图片翻转
                // Image flip
                imageFlip(frame, (IMVDefine.IMV_EFlipType) imageFlipType);

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

        // 图片翻转
        // Image flip
        private static void imageFlip(IMVDefine.IMV_Frame frame, IMVDefine.IMV_EFlipType imageFlipType)
        {
            IMVDefine.IMV_PixelConvertParam stPixelConvertParam = new IMVDefine.IMV_PixelConvertParam();
            IMVDefine.IMV_FlipImageParam stFlipImageParam = new IMVDefine.IMV_FlipImageParam();
            UInt32 nChannelNum = 0;
            UInt32 nConvertBufSize = 0;
            IntPtr pConvertBuf = IntPtr.Zero;
            UInt32 nFlipBufSize = 0;
            IntPtr pFlipBuf = IntPtr.Zero;
            int res = IMVDefine.IMV_OK;
            string pFileName = "";

            if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelMono8)
            {
                stFlipImageParam.pSrcData = frame.pData;
                stFlipImageParam.nSrcDataLen = frame.frameInfo.width*frame.frameInfo.height*MONO_CHANNEL_NUM;
                stFlipImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
                nChannelNum = MONO_CHANNEL_NUM;
            }
            else if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelBGR8)
            {
                stFlipImageParam.pSrcData = frame.pData;
                stFlipImageParam.nSrcDataLen = frame.frameInfo.width*frame.frameInfo.height*BGR_CHANNEL_NUM;
                stFlipImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
                nChannelNum = BGR_CHANNEL_NUM;
            }
            else if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelRGB8)
            {
                stFlipImageParam.pSrcData = frame.pData;
                stFlipImageParam.nSrcDataLen = frame.frameInfo.width*frame.frameInfo.height*RGB_CHANNEL_NUM;
                stFlipImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
                nChannelNum = RGB_CHANNEL_NUM;
            }
            // MONO8/RGB24/BGR24以外的格式都转化成BGR24
            // All formats except MONO8/RGB24/BGR24 are converted to BGR24
            else
            {
                nConvertBufSize = frame.frameInfo.width*frame.frameInfo.height*BGR_CHANNEL_NUM;
                pConvertBuf = Marshal.AllocHGlobal((int) nConvertBufSize);
                if (pConvertBuf == IntPtr.Zero)
                {
                    Console.WriteLine("malloc pConvertBuf failed!");
                    return;
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
                stPixelConvertParam.pDstBuf = pConvertBuf;
                stPixelConvertParam.nDstBufSize = nConvertBufSize;

                res = cam.IMV_PixelConvert(ref stPixelConvertParam);
                if (res == IMVDefine.IMV_OK)
                {
                    stFlipImageParam.pSrcData = pConvertBuf;
                    stFlipImageParam.nSrcDataLen = stPixelConvertParam.nDstDataLen;
                    stFlipImageParam.ePixelFormat = IMVDefine.IMV_EPixelType.gvspPixelBGR8;
                    nChannelNum = BGR_CHANNEL_NUM;
                }
                else
                {
                    stFlipImageParam.pSrcData = IntPtr.Zero;
                    Console.WriteLine("image convert to BGR8 failed! ErrorCode[{0}]", res);
                }
            }

            do
            {
                if (stFlipImageParam.pSrcData == IntPtr.Zero)
                {
                    Console.WriteLine("stFlipImageParam pSrcData is NULL!");
                    break;
                }

                nFlipBufSize = frame.frameInfo.width*frame.frameInfo.height*nChannelNum;
                pFlipBuf = Marshal.AllocHGlobal((int) nFlipBufSize);
                if (pFlipBuf == IntPtr.Zero)
                {
                    Console.WriteLine("malloc pFlipBuf failed!");
                    break;
                }

                stFlipImageParam.nWidth = frame.frameInfo.width;
                stFlipImageParam.nHeight = frame.frameInfo.height;
                stFlipImageParam.eFlipType = imageFlipType;
                stFlipImageParam.pDstBuf = pFlipBuf;
                stFlipImageParam.nDstBufSize = nFlipBufSize;

                res = cam.IMV_FlipImage(ref stFlipImageParam);
                if (res == IMVDefine.IMV_OK)
                {
                    if (imageFlipType == IMVDefine.IMV_EFlipType.typeFlipVertical)
                    {
                        Console.WriteLine("Image vertical flip successfully!");
                        pFileName = "verticalFlip.bin";
                    }
                    else
                    {
                        Console.WriteLine("Image horizontal flip successfully!");
                        pFileName = "horizontalFlip.bin";
                    }
                    SaveToBin(pFlipBuf, pFileName, (int) nFlipBufSize);
                }
                else
                {
                    if (imageFlipType == IMVDefine.IMV_EFlipType.typeFlipVertical)
                    {
                        Console.WriteLine("Image vertical flip failed! ErrorCode[{0}]", res);
                    }
                    else
                    {
                        Console.WriteLine("Image horizontal flip failed! ErrorCode[{0}]", res);
                    }
                }

            } while (false);

            if (pConvertBuf != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pConvertBuf);
                pConvertBuf = IntPtr.Zero;
            }
            if (pFlipBuf != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pFlipBuf);
                pFlipBuf = IntPtr.Zero;
            }
        }


        //保存为bin文件
        //Save to Bin
        private static bool SaveToBin(IntPtr pSource, string path, int size)
        {
            byte[] pBuffer = new byte[size];
            Marshal.Copy(pSource, pBuffer, 0, size);
            try
            {
                using (Stream stream = new FileStream(path, FileMode.Create))
                {
                    using (BinaryWriter sw = new BinaryWriter(stream)) //建立二进制文件流
                    {
                        sw.Write(pBuffer);
                    }
                }
                Console.WriteLine("Save bin Successfully!Save path is [{0}]", path);
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception.ToString());
                return false;
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
