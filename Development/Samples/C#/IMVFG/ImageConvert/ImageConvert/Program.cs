using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using CaptureCard_Net;

namespace ImageConvert
{
    internal class Program
    {
        private static CamDev cam = new CamDev();
        private static CardDev card = new CardDev();
        private const int INTERFACE = 1;
        private const int CAMERA = 0;

        private static void Main(string[] args)
        {

            uint boardIndex = 0;
            int cameraIndex = 0;
            Console.WriteLine("SDK Version:{0}", CardDev.IMV_FG_GetVersion());
            Console.WriteLine("Enum capture board interface info.");
            //枚举采集卡设备
            // Discover capture board device
            int res = IMVFGDefine.IMV_FG_OK;
            IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST interfaceList = new IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST();
            IMVFGDefine.IMV_FG_EInterfaceType interfaceTp = IMVFGDefine.IMV_FG_EInterfaceType.typeCLInterface;
            IMVFGDefine.IMV_FG_Frame frame=new IMVFGDefine.IMV_FG_Frame();

            res = CardDev.IMV_FG_EnumInterface((uint)interfaceTp, ref interfaceList);
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
            res = CamDev.IMV_FG_EnumDevices((uint)interfaceTp, ref camListPtr);
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
            cameraIndex = (int)selectDevice(interfaceList.nInterfaceNum, camListPtr.nDevNum, CAMERA);
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
                res = cam.IMV_FG_OpenDevice(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_INDEX, cameraIndex);
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
                res = card.IMV_FG_ReleaseFrame(ref frame);
                if (res != 0)
                {
                    Console.WriteLine("Release frame failed! ErrorCode:[{0}]", res);
                    break;
                }

                // 停止拉流 
                // Stop grabbing 
                res = card.IMV_FG_StopGrabbing();
                if (res != 0)
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


        private static void imageConvert(IMVFGDefine.IMV_FG_Frame frame, int format)
        {
            IMVFGDefine.IMV_FG_PixelConvertParam stPixelConvertParam = new IMVFGDefine.IMV_FG_PixelConvertParam();
            uint nDstBufSize = 0;
            string pConvertFormatStr = "";
            string pFileName = "";
            IMVFGDefine.IMV_FG_EPixelType convertFormat = IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8;
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
                    convertFormat = IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_RGB8;
                    break;
                case 2:
                    nDstBufSize = frame.frameInfo.width*frame.frameInfo.height*3;
                    pConvertFormatStr = "BGR8";
                    pFileName = "convertBGR8.bin";
                    convertFormat = IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8;
                    break;
                case 3:
                    nDstBufSize = frame.frameInfo.width*frame.frameInfo.height*4;
                    pConvertFormatStr = "BGRA8";
                    pFileName = "convertBGRA8.bin";
                    convertFormat=IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGRA8;
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
            stPixelConvertParam.eBayerDemosaic = IMVFGDefine.IMV_FG_EBayerDemosaic.IMV_FG_DEMOSAIC_NEAREST_NEIGHBOR;
            stPixelConvertParam.eDstPixelFormat = convertFormat;
            stPixelConvertParam.pDstBuf = pDstBuf;
            stPixelConvertParam.nDstBufSize = nDstBufSize;

            int res = card.IMV_FG_PixelConvert(ref stPixelConvertParam);
            if (res == IMVFGDefine.IMV_FG_OK)
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

            return (uint)inputIndex;
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
