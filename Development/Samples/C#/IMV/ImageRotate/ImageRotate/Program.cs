using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MVSDK_Net;

namespace ImageRotate
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

                // 选择图像旋转角度
                // Select image rotate angle
                Console.WriteLine("\n--------------------------------------------");
                Console.WriteLine("\t0.Image rotation 90 degree angle");
                Console.WriteLine("\t1.Image rotation 180 degree angle");
                Console.WriteLine("\t2.Image rotation 270 degree angle");
                Console.WriteLine("--------------------------------------------");
                int rotationAngle = 0;
                Console.Write("Please select the rotation angle index: ");
                try
                {
                    rotationAngle = Convert.ToInt32(Console.ReadLine());
                }
                catch
                {
                    Console.WriteLine("Invalid Input!Use default rotation angle:Image rotation 90 degree angle");
                    rotationAngle = 0;
                }
                if (rotationAngle > 2 || rotationAngle < 0)
                {
                    Console.WriteLine("Input Error!Use default rotation angle:Image rotation 90 degree angle");
                    rotationAngle = 0;
                }

                Console.WriteLine("BlockId ({0}) pixelFormat ({1}) imageRotateAngle({2}), Start image rotate...",
                    frame.frameInfo.blockId, frame.frameInfo.pixelFormat, (IMVDefine.IMV_ERotationAngle) rotationAngle);

                // 图片旋转
                // Image rotate
                imageRotate(frame, (IMVDefine.IMV_ERotationAngle) rotationAngle);

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
        private static void imageRotate(IMVDefine.IMV_Frame frame, IMVDefine.IMV_ERotationAngle rotationAngle)
        {
            IMVDefine.IMV_PixelConvertParam stPixelConvertParam = new IMVDefine.IMV_PixelConvertParam();
            IMVDefine.IMV_RotateImageParam stRotateImageParam = new IMVDefine.IMV_RotateImageParam();
            UInt32 nChannelNum = 0;
            UInt32 nConvertBufSize = 0;
            IntPtr pConvertBuf = IntPtr.Zero;
            UInt32 nRotateBufSize = 0;
            IntPtr pRotateBuf = IntPtr.Zero;
            int res = IMVDefine.IMV_OK;
            string pFileName = "";

            if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelMono8)
            {
                stRotateImageParam.pSrcData = frame.pData;
                stRotateImageParam.nSrcDataLen = frame.frameInfo.width*frame.frameInfo.height*MONO_CHANNEL_NUM;
                stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
                nChannelNum = MONO_CHANNEL_NUM;
            }
            else if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelBGR8)
            {
                stRotateImageParam.pSrcData = frame.pData;
                stRotateImageParam.nSrcDataLen = frame.frameInfo.width*frame.frameInfo.height*BGR_CHANNEL_NUM;
                stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
                nChannelNum = BGR_CHANNEL_NUM;
            }
            else if (frame.frameInfo.pixelFormat == IMVDefine.IMV_EPixelType.gvspPixelRGB8)
            {
                stRotateImageParam.pSrcData = frame.pData;
                stRotateImageParam.nSrcDataLen = frame.frameInfo.width*frame.frameInfo.height*RGB_CHANNEL_NUM;
                stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
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
                    stRotateImageParam.pSrcData = pConvertBuf;
                    stRotateImageParam.nSrcDataLen = stPixelConvertParam.nDstDataLen;
                    stRotateImageParam.ePixelFormat = IMVDefine.IMV_EPixelType.gvspPixelBGR8;
                    nChannelNum = BGR_CHANNEL_NUM;
                }
                else
                {
                    stRotateImageParam.pSrcData = IntPtr.Zero;
                    Console.WriteLine("image convert to BGR8 failed! ErrorCode[{0}]", res);
                }
            }

            do
            {
                if (stRotateImageParam.pSrcData == IntPtr.Zero)
                {
                    Console.WriteLine("stRotateImageParam pSrcData is NULL!");
                    break;
                }

                nRotateBufSize = frame.frameInfo.width*frame.frameInfo.height*nChannelNum;
                pRotateBuf = Marshal.AllocHGlobal((int) nRotateBufSize);
                if (pRotateBuf == IntPtr.Zero)
                {
                    Console.WriteLine("malloc pFlipBuf failed!");
                    break;
                }

                stRotateImageParam.nWidth = frame.frameInfo.width;
                stRotateImageParam.nHeight = frame.frameInfo.height;
                stRotateImageParam.eRotationAngle = rotationAngle;
                stRotateImageParam.pDstBuf = pRotateBuf;
                stRotateImageParam.nDstBufSize = nRotateBufSize;

                res = cam.IMV_RotateImage(ref stRotateImageParam);
                if (res == IMVDefine.IMV_OK)
                {
                    if (rotationAngle == IMVDefine.IMV_ERotationAngle.rotationAngle90)
                    {
                        Console.WriteLine("Image rotation angle 90 degree successfully!");
                        pFileName = "rotationAngle90.bin";
                    }
                    else if (rotationAngle == IMVDefine.IMV_ERotationAngle.rotationAngle180)
                    {
                        Console.WriteLine("Image rotation angle 180 degree successfully!");
                        pFileName = "rotationAngle180.bin";
                    }
                    else
                    {
                        Console.WriteLine("Image rotation angle 270 degree successfully!");
                        pFileName = "rotationAngle270.bin";
                    }
                    SaveToBin(pRotateBuf, pFileName, (int) nRotateBufSize);
                }
                else
                {
                    if (rotationAngle == IMVDefine.IMV_ERotationAngle.rotationAngle90)
                    {
                        Console.WriteLine("Image rotation angle 90 degree failed! ErrorCode[{0}]", res);
                    }
                    else if (rotationAngle == IMVDefine.IMV_ERotationAngle.rotationAngle180)
                    {
                        Console.WriteLine("Image rotation angle 180 degree failed! ErrorCode[{0}]", res);
                    }
                    else
                    {
                        Console.WriteLine("Image rotation angle 270 degree failed! ErrorCode[{0}]", res);
                    }
                }

            } while (false);

            if (pConvertBuf != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pConvertBuf);
                pConvertBuf = IntPtr.Zero;
            }
            if (pRotateBuf != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pRotateBuf);
                pRotateBuf = IntPtr.Zero;
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
