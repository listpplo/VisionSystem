using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MVSDK_Net;

namespace ImageConvert
{
    internal class Program
    {
        private static MyCamera cam = new MyCamera();

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

                // 选择图像转换目标格式
                // Select target format for image convert
                Console.WriteLine("\n--------------------------------------------");
                Console.WriteLine("\t0.Convert to mono8");
                Console.WriteLine("\t1.Convert to RGB24");
                Console.WriteLine("\t2.Convert to BGR24");
                Console.WriteLine("\t3.Convert to BGRA32");
                Console.WriteLine("--------------------------------------------");
                int nConvertFormat = 0;
                Console.Write("Please select the convert format index: ");
                try
                {
                    nConvertFormat = Convert.ToInt32(Console.ReadLine());
                }
                catch
                {
                    Console.WriteLine("Invalid Input!Convet to mono8");
                    nConvertFormat = 0;
                }
                if (nConvertFormat > 3 || nConvertFormat < 0)
                {
                    Console.WriteLine("Input Error!Convet to mono8");
                    nConvertFormat = 0;
                }

                Console.WriteLine("BlockId ({0}) pixelFormat ({1}), Start image convert...",frame.frameInfo.blockId,frame.frameInfo.pixelFormat);

                // 图片转化
                // Image convert
                imageConvert(frame, nConvertFormat);

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


        private static void imageConvert(IMVDefine.IMV_Frame frame, int format)
        {
            IMVDefine.IMV_PixelConvertParam stPixelConvertParam = new IMVDefine.IMV_PixelConvertParam();
            uint nDstBufSize = 0;
            string pConvertFormatStr = "";
            string pFileName = "";
            IMVDefine.IMV_EPixelType convertFormat = IMVDefine.IMV_EPixelType.gvspPixelMono8;
            switch (format)
            {
                case 0:
                    nDstBufSize = frame.frameInfo.width*frame.frameInfo.height;
                    pConvertFormatStr = "Mono8";
                    pFileName = "convertMono8.bin";
                    break;
                case 1:
                    nDstBufSize = frame.frameInfo.width*frame.frameInfo.height*3;
                    pConvertFormatStr = "RGB8";
                    pFileName = "convertRGB8.bin";
                    convertFormat = IMVDefine.IMV_EPixelType.gvspPixelRGB8;
                    break;
                case 2:
                    nDstBufSize = frame.frameInfo.width*frame.frameInfo.height*3;
                    pConvertFormatStr = "BGR8";
                    pFileName = "convertBGR8.bin";
                    convertFormat = IMVDefine.IMV_EPixelType.gvspPixelBGR8;
                    break;
                case 3:
                    nDstBufSize = frame.frameInfo.width*frame.frameInfo.height*4;
                    pConvertFormatStr = "BGRA8";
                    pFileName = "convertBGRA8.bin";
                    convertFormat=IMVDefine.IMV_EPixelType.gvspPixelBGRA8;
                    break;
            }
            IntPtr pDstBuf = Marshal.AllocHGlobal((int) nDstBufSize);

            stPixelConvertParam.nWidth = frame.frameInfo.width;
            stPixelConvertParam.nHeight = frame.frameInfo.height;
            stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat;
            stPixelConvertParam.pSrcData = frame.pData;
            stPixelConvertParam.nSrcDataLen = frame.frameInfo.size;
            stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX;
            stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY;
            stPixelConvertParam.eBayerDemosaic = IMVDefine.IMV_EBayerDemosaic.demosaicNearestNeighbor;
            stPixelConvertParam.eDstPixelFormat = convertFormat;
            stPixelConvertParam.pDstBuf = pDstBuf;
            stPixelConvertParam.nDstBufSize = nDstBufSize;

            int res = cam.IMV_PixelConvert(ref stPixelConvertParam);
            if (res == IMVDefine.IMV_OK)
            {
                Console.WriteLine("image convert to {0} successfully! nDstDataLen {1}", pConvertFormatStr,
                    stPixelConvertParam.nDstBufSize);

                SaveToBin(pDstBuf, pFileName, (int) nDstBufSize);
            }
            else
            {
                Console.WriteLine("image convert to {0} failed! ErrorCode[{1}]", pConvertFormatStr, res);
            }

            if (pDstBuf != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pDstBuf);
                pDstBuf = IntPtr.Zero;
            }
        }

        //保存为bin文件
        //Save to Bin
        private static bool SaveToBin(IntPtr pSource,string path,int size)
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
                            deviceInfoList.pDevInfo + Marshal.SizeOf(typeof(IMVDefine.IMV_DeviceInfo)) * i,
                            typeof(IMVDefine.IMV_DeviceInfo));

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
                                typeof(IMVDefine.IMV_GigEDeviceInfo));

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
