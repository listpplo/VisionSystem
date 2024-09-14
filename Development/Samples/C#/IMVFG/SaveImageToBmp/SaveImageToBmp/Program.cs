﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;
using CaptureCard_Net;

namespace SaveImageToBmp
{
    internal class Program
    {
        private static CamDev cam = new CamDev();
        private static CardDev card = new CardDev();
        private static string m_binSavePath = System.Environment.CurrentDirectory + @"\Bins";
        private static string m_bitMapSavePath = System.Environment.CurrentDirectory + @"\BitMaps";
        private const int INTERFACE = 1;
        private const int CAMERA = 0;

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
            uint boardIndex = 0;
            int cameraIndex = 0;
            Console.WriteLine("SDK Version:{0}", CardDev.IMV_FG_GetVersion());
            Console.WriteLine("Enum capture board interface info.");
            //枚举采集卡设备
            // Discover capture board device
            int res = IMVFGDefine.IMV_FG_OK;
            IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST interfaceList = new IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST();
            IMVFGDefine.IMV_FG_EInterfaceType interfaceTp = IMVFGDefine.IMV_FG_EInterfaceType.typeCLInterface;

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
                    Console.WriteLine("Start grabbing failed! errorCode:[{0}]", res);
                    break;
                }

                //取图
                //get frame 
                IMVFGDefine.IMV_FG_Frame frame = new IMVFGDefine.IMV_FG_Frame();
                res = card.IMV_FG_GetFrame(ref frame, 1000);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Get frame failed! errorCode:[{0}]", res);
                    break;
                }

                //保存bin文件
                //save bin file
                if (!SaveToBin(ref frame))
                {
                    Console.WriteLine("Save Bin failed");
                    break;
                }

                //保存bitmap文件
                //save to bitmap
                if (!SaveToBitmap(ref frame))
                {
                    Console.WriteLine("Save Bitmap failed");
                }

                res = card.IMV_FG_ReleaseFrame(ref frame);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Release frame failed! errorCode:[{0}]", res);
                    break;
                }

                // 停止拉流 
                // Stop grabbing 
                res = card.IMV_FG_StopGrabbing();
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Stop grabbing failed! errorCode:[{0}]", res);
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

        //保存为bin文件
        //Save to Bin
        private static bool SaveToBin(ref IMVFGDefine.IMV_FG_Frame frame)
        {
            byte[] pBuffer = new byte[frame.frameInfo.size];
            Marshal.Copy(frame.pData, pBuffer, 0, (int)frame.frameInfo.size);
            if (!System.IO.Directory.Exists(m_binSavePath))
            {
                System.IO.Directory.CreateDirectory(m_binSavePath);
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
                Console.WriteLine(string.Format("Save bin Successfully!Save path is [{0}]", binPath));
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
        private static bool SaveToBitmap(ref IMVFGDefine.IMV_FG_Frame frame)
        {
            Bitmap bitmap = null;
            if (!ConvertToBitmap(ref frame, ref bitmap))
            {
                return false;
            }
            if (!System.IO.Directory.Exists(m_bitMapSavePath))
            {
                System.IO.Directory.CreateDirectory(m_bitMapSavePath);
            }
            try
            {
                string imageName = m_bitMapSavePath + @"\" + frame.frameInfo.blockId + ".bmp";
                bitmap.Save(imageName);
                bitmap.Dispose();
                Console.WriteLine(string.Format("Save bitmap Successfully!Save path is [{0}]", imageName));
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

        private static bool ConvertToBitmap(ref IMVFGDefine.IMV_FG_Frame frame, ref Bitmap bitmap)
        {
            IntPtr pDstRGB;
            BitmapData bmpData;
            Rectangle bitmapRect = new Rectangle();

            // mono8和BGR8裸数据不需要转码
            // mono8 and BGR8 raw data is not need to convert
            if (frame.frameInfo.pixelFormat != IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8
               && frame.frameInfo.pixelFormat != IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8)
            {
                IMVFGDefine.IMV_FG_PixelConvertParam stPixelConvertParam = new IMVFGDefine.IMV_FG_PixelConvertParam();
                int res = IMVFGDefine.IMV_FG_OK;

                //转目标内存 彩色
                var ImgSize = (int)frame.frameInfo.width * (int)frame.frameInfo.height * 3;

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
                stPixelConvertParam.eBayerDemosaic = IMVFGDefine.IMV_FG_EBayerDemosaic.IMV_FG_DEMOSAIC_NEAREST_NEIGHBOR;
                stPixelConvertParam.eDstPixelFormat = IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8;
                stPixelConvertParam.pDstBuf = pDstRGB;
                stPixelConvertParam.nDstBufSize = (uint)ImgSize;

                res = card.IMV_FG_PixelConvert(ref stPixelConvertParam);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("image convert to BGR failed!");
                    return false;
                }
            }
            else
            {
                pDstRGB = frame.pData;
            }

            if (frame.frameInfo.pixelFormat == IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8)
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
                if (frame.frameInfo.pixelFormat != IMVFGDefine.IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8)
                {
                    Marshal.FreeHGlobal(pDstRGB);
                }
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
                            Marshal.SizeOf(typeof(IMVFGDefine.IMV_FG_INTERFACE_INFO))*i,
                            typeof(IMVFGDefine.IMV_FG_INTERFACE_INFO));
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
                                Marshal.SizeOf(typeof(IMVFGDefine.IMV_FG_DEVICE_INFO))*j,
                                typeof(IMVFGDefine.IMV_FG_DEVICE_INFO));

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
