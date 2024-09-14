using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using CaptureCard_Net;

namespace ImageRotate
{
    internal class Program
    {
        private static CamDev cam = new CamDev();
        private static CardDev card = new CardDev();
        private const int INTERFACE = 1;
        private const int CAMERA = 0;
        private const int MONO_CHANNEL_NUM = 1;
        private const int RGB_CHANNEL_NUM = 3;
        private const int BGR_CHANNEL_NUM = 3;

        private static void Main(string[] args)
        {

            uint boardIndex = 0;
            int CamDevIndex = 0;
            Console.WriteLine("SDK Version:{0}", CardDev.IMV_FG_GetVersion());
            Console.WriteLine("Enum capture board interface info.");
            //枚举采集卡设备
            // Discover capture board device
            int res = IMVFGDefine.IMV_FG_OK;
            IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST interfaceList = new IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST();
            IMVFGDefine.IMV_FG_EInterfaceType interfaceTp = IMVFGDefine.IMV_FG_EInterfaceType.typeCLInterface;
            IMVFGDefine.IMV_FG_Frame frame=new IMVFGDefine.IMV_FG_Frame();

            res = CardDev.IMV_FG_EnumInterface((uint) interfaceTp, ref interfaceList);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Enumeration devices failed!errorCode:[{0}]", res);
                Console.Read();
                return;
            }
            if (interfaceList.nInterfaceNum == 0)
            {
                Console.WriteLine("No board device find.errorCode:[{0}]", res);
                Console.Read();
                return;
            }

            Console.WriteLine("Enum camera device.");
            IMVFGDefine.IMV_FG_DEVICE_INFO_LIST camListPtr = new IMVFGDefine.IMV_FG_DEVICE_INFO_LIST();
            //枚举相机设备
            // discover camera 
            res = CamDev.IMV_FG_EnumDevices((uint) interfaceTp, ref camListPtr);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Enumeration camera devices failed!errorCode:[{0}]", res);
                Console.Read();
                return;
            }
            if (camListPtr.nDevNum == 0)
            {
                Console.WriteLine("No camera device find.errorCode:[{0}]", res);
                Console.Read();
                return;
            }

            // 打印设备基本信息（序号,类型,制造商信息,型号,序列号,用户自定义ID)
            // Print device info (Index, Type, Vendor, Model, Serial number, DeviceUserID) 
            displayDeviceInfo(interfaceList, camListPtr);

            // 选择需要连接的采集卡设备开流
            // Select one camera to connect to  
            boardIndex = selectDevice(interfaceList.nInterfaceNum, camListPtr.nDevNum, INTERFACE);
            CamDevIndex = (int) selectDevice(interfaceList.nInterfaceNum, camListPtr.nDevNum, CAMERA);

            do
            {
                Console.WriteLine("Open capture device.");
                // 打开采集卡设备
                // Open capture device 
                res = card.IMV_FG_OpenInterface(boardIndex);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Open cameralink capture board device failed!errorCode:[{0}]", res);
                    break;
                }

                Console.WriteLine("Open camera device.");
                // 打开采集卡相机设备 
                // Connect to CamDev 
                res = cam.IMV_FG_OpenDevice(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_INDEX, CamDevIndex);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Open camera failed!errorCode:[{0}]", res);
                    break;
                }

                // 开始拉流 
                // Start grabbing 
                res = card.IMV_FG_StartGrabbing();
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Start grabbing failed! ErrorCode:[{0}]", res);
                    break;
                }

                // 获取一帧图像
                // Get a frame image
                res = card.IMV_FG_GetFrame(ref frame, 500);
                if (res != IMVFGDefine.IMV_FG_OK)
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
                    frame.frameInfo.blockId, frame.frameInfo.pixelFormat, (IMVFGDefine.IMV_FG_ERotationAngle) rotationAngle);

                // 图片旋转
                // Image rotate
                imageRotate(frame, (IMVFGDefine.IMV_FG_ERotationAngle)rotationAngle);

                // 释放图像缓存
                // Free image buffer
                res = card.IMV_FG_ReleaseFrame(ref frame);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Release frame failed! ErrorCode:[{0}]", res);
                    break;
                }

                // 停止拉流 
                // Stop grabbing 
                res = card.IMV_FG_StopGrabbing();
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Stop grabbing failed! ErrorCode:[{0}]", res);
                    break;
                }
               
            } while (false);

            Console.WriteLine("Close camera device.");
            if (cam.getHandle() != IntPtr.Zero)
            {
                //关闭相机
                //Close camera 
                res = cam.IMV_FG_CloseDevice();
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Close camera failed!");
                }
            }

            Console.WriteLine("Close capture device.");
            if (card.getHandle() != IntPtr.Zero)
            {
                // 关闭采集卡
                // Close capture device 
                res = card.IMV_FG_CloseInterface();
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Close cameralink board failed!");
                }
            }

            Console.WriteLine("Press enter key to exit...");
            Console.Read();

        }

        // 图片翻转
        // Image flip
        private static void imageRotate(IMVFGDefine.IMV_FG_Frame frame, IMVFGDefine.IMV_FG_ERotationAngle rotationAngle)
        {
            IMVFGDefine.IMV_FG_PixelConvertParam stPixelConvertParam = new IMVFGDefine.IMV_FG_PixelConvertParam();
            IMVFGDefine.IMV_FG_RotateImageParam stRotateImageParam = new IMVFGDefine.IMV_FG_RotateImageParam();
            UInt32 nChannelNum = 0;
            UInt32 nConvertBufSize = 0;
            IntPtr pConvertBuf = IntPtr.Zero;
            UInt32 nRotateBufSize = 0;
            IntPtr pRotateBuf = IntPtr.Zero;
            int res = IMVFGDefine.IMV_FG_OK;
            string pFileName = "";

            if (frame.frameInfo.pixelFormat == IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8)
            {
                stRotateImageParam.pSrcData = frame.pData;
                stRotateImageParam.nSrcDataLen = frame.frameInfo.width*frame.frameInfo.height*MONO_CHANNEL_NUM;
                stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
                nChannelNum = MONO_CHANNEL_NUM;
            }
            else if (frame.frameInfo.pixelFormat == IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8)
            {
                stRotateImageParam.pSrcData = frame.pData;
                stRotateImageParam.nSrcDataLen = frame.frameInfo.width*frame.frameInfo.height*BGR_CHANNEL_NUM;
                stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
                nChannelNum = BGR_CHANNEL_NUM;
            }
            else if (frame.frameInfo.pixelFormat == IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_RGB8)
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
                stPixelConvertParam.eBayerDemosaic = IMVFGDefine.IMV_FG_EBayerDemosaic.IMV_FG_DEMOSAIC_BILINEAR;
                stPixelConvertParam.eDstPixelFormat = IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8;
                stPixelConvertParam.pDstBuf = pConvertBuf;
                stPixelConvertParam.nDstBufSize = nConvertBufSize;

                res = card.IMV_FG_PixelConvert(ref stPixelConvertParam);
                if (res == IMVFGDefine.IMV_FG_OK)
                {
                    stRotateImageParam.pSrcData = pConvertBuf;
                    stRotateImageParam.nSrcDataLen = stPixelConvertParam.nDstDataLen;
                    stRotateImageParam.ePixelFormat = IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8;
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

                res = card.IMV_FG_RotateImage(ref stRotateImageParam);
                if (res == IMVFGDefine.IMV_FG_OK)
                {
                    if (rotationAngle == IMVFGDefine.IMV_FG_ERotationAngle.IMV_FG_ROTATION_ANGLE90)
                    {
                        Console.WriteLine("Image rotation angle 90 degree successfully!");
                        pFileName = "rotationAngle90.bin";
                    }
                    else if (rotationAngle == IMVFGDefine.IMV_FG_ERotationAngle.IMV_FG_ROTATION_ANGLE180)
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
                    if (rotationAngle == IMVFGDefine.IMV_FG_ERotationAngle.IMV_FG_ROTATION_ANGLE90)
                    {
                        Console.WriteLine("Image rotation angle 90 degree failed! ErrorCode[{0}]", res);
                    }
                    else if (rotationAngle == IMVFGDefine.IMV_FG_ERotationAngle.IMV_FG_ROTATION_ANGLE180)
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

        private static uint selectDevice(uint interfaceCnt, uint cameraCnt, int devType)
        {
            if (devType == CAMERA) Console.Write("Please input the camera index: ");
            else Console.Write("Please input the interface index: ");
            int inputIndex;
            while (true)
            {
                inputIndex = 0;
                try
                {
                    inputIndex = Convert.ToInt32(Console.ReadLine()) - 1;
                }
                catch
                {
                    if (devType == INTERFACE)
                    {
                        Console.Write("Invalid Input!Please input the interface index: ");
                        continue;
                    }
                    else
                    {
                        Console.Write("Invalid Input!Please input the camera index: ");
                        continue;
                    }
                }
                if ((inputIndex > interfaceCnt - 1 || inputIndex < 0) && devType == INTERFACE)
                {
                    Console.Write("Input Error!Please input the interface index: ");
                    continue;
                }
                else if ((inputIndex > cameraCnt - 1 || inputIndex < 0) && devType == CAMERA)
                {
                    Console.Write("Input Error!Please input the camera index: ");
                    continue;
                }
                break;
            }

            return (uint) inputIndex;
        }

         #region 展示设备列表

        private static void displayDeviceInfo(IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST interfaceInfoList,
            IMVFGDefine.IMV_FG_DEVICE_INFO_LIST deviceInfoList)
        {
            // 打印Title行 
            // Print title line 
            Console.WriteLine(
                "\nIdx    Type            Vendor                  Model               S/N                   DeviceUserID                               ");
            Console.WriteLine(
                "--------------------------------------------------------------------------------------------------------------");
            for (int i = 0; i < interfaceInfoList.nInterfaceNum; i++)
            {
                string interfaceinfostr = string.Empty;
                IMVFGDefine.IMV_FG_INTERFACE_INFO interfaceinfo =
                    (IMVFGDefine.IMV_FG_INTERFACE_INFO)
                        Marshal.PtrToStructure(
                            interfaceInfoList.pInterfaceInfoList +
                            Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_INTERFACE_INFO))*i,
                            typeof (IMVFGDefine.IMV_FG_INTERFACE_INFO));
                // 设备列表的相机索引  最大表示字数：3
                // CamDev index in device list, display in 3 characters 
                interfaceinfostr += (i + 1).ToString();
                switch (interfaceinfo.nInterfaceType)
                {
                    case IMVFGDefine.IMV_FG_EInterfaceType.typeGigEInterface:
                        interfaceinfostr += "    GigE Card";
                        break;
                    case IMVFGDefine.IMV_FG_EInterfaceType.typeU3vInterface:
                        interfaceinfostr += "    U3V Card";
                        break;
                    case IMVFGDefine.IMV_FG_EInterfaceType.typeCLInterface:
                        interfaceinfostr += "    CL Card";
                        break;
                    case IMVFGDefine.IMV_FG_EInterfaceType.typeCXPInterface:
                        interfaceinfostr += "    CXP Card";
                        break;
                    default:
                        Console.WriteLine("    ");
                        break;
                }

                // 制造商信息  
                // CamDev vendor name 
                //interfaceinfostr += "           " + interfaceinfo.vendorName;
                interfaceinfostr += interfaceinfo.vendorName.PadLeft(interfaceinfo.vendorName.Length + 11);
                // 相机的型号信息  
                // CamDev model name 
                interfaceinfostr += interfaceinfo.modelName.PadLeft(interfaceinfo.modelName.Length + 9);

                // 相机的序列号 
                // CamDev serial number
                interfaceinfostr += interfaceinfo.serialNumber.PadLeft(interfaceinfo.serialNumber.Length + 14);

                // 自定义用户ID 
                // CamDev user id 
                interfaceinfostr += interfaceinfo.interfaceName.PadLeft(interfaceinfo.interfaceName.Length + 14);

                Console.WriteLine(interfaceinfostr);
                Console.Write("    |--");

                bool isFind = false;
                for (int j = 0; j < deviceInfoList.nDevNum; j++)
                {
                    string deviceinfostr = string.Empty;
                    IMVFGDefine.IMV_FG_DEVICE_INFO deviceinfo =
                        (IMVFGDefine.IMV_FG_DEVICE_INFO)
                            Marshal.PtrToStructure(
                                deviceInfoList.pDeviceInfoList +
                                Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_DEVICE_INFO))*j,
                                typeof (IMVFGDefine.IMV_FG_DEVICE_INFO));

                    if (deviceinfo.FGInterfaceInfo.interfaceKey.Equals(interfaceinfo.interfaceKey))
                    {
                        // 设备列表的相机索引  最大表示字数：3
                        // CamDev index in device list, display in 3 characters 
                        deviceinfostr += (j + 1).ToString();

                        // 制造商信息  
                        // CamDev vendor name 
                        deviceinfostr += deviceinfo.vendorName.PadLeft(deviceinfo.vendorName.Length + 15);

                        // 相机的型号信息  
                        // CamDev model name 
                        deviceinfostr += deviceinfo.modelName.PadLeft(deviceinfo.modelName.Length + 9);

                        // 相机的序列号 
                        // CamDev serial number
                        deviceinfostr += deviceinfo.serialNumber.PadLeft(deviceinfo.serialNumber.Length + 8);

                        // 自定义用户ID 
                        // CamDev user id 
                        deviceinfostr += deviceinfo.cameraName.PadLeft(deviceinfo.cameraName.Length + 8);

                        Console.WriteLine(deviceinfostr);
                        isFind = true;
                        break;
                    }

                }
                if (!isFind) Console.WriteLine();
            }
        }

        #endregion

     
    }
}
