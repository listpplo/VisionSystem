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

namespace SaveImageToFile
{
    internal class Program
    {
        private static MyCamera cam = new MyCamera();
        private static object lockobj = new object();
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

                // 关闭触发模式 
                // Set TriggerMode off 
                res = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "Off");
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Set TriggerMode failed! ErrorCode:[{0}]", res);
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

                // 选择图像转换目标格式
                // Select target format for image convert
                Console.WriteLine("\n--------------------------------------------");
                Console.WriteLine("\t0.Save to BMP");
                Console.WriteLine("\t1.Save to Jpeg");
                Console.WriteLine("\t2.Save to Png");
                Console.WriteLine("\t3.Save to Tif");
                Console.WriteLine("--------------------------------------------");
                int nSaveFormat = 0;
                Console.Write("Please select the save format index: ");
                try
                {
                    nSaveFormat = Convert.ToInt32(Console.ReadLine());
                }
                catch
                {
                    Console.WriteLine("Invalid Input!Save to BMP");
                    nSaveFormat = 0;
                }
                if (nSaveFormat > 3 || nSaveFormat < 0)
                {
                    Console.WriteLine("Input Error!Save to BMP");
                    nSaveFormat = 0;
                }

                Console.WriteLine("BlockId ({0}) pixelFormat ({1}), Start image save...", frame.frameInfo.blockId, frame.frameInfo.pixelFormat);

                // 图片保存
                // Image save
                imageSave(ref frame, nSaveFormat);

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

        private static void imageSave(ref IMVDefine.IMV_Frame frame, int format)
        {
            lock (lockobj)
            {
                IMVDefine.IMV_SaveImageToFileParam stSaveImageToFileParam = new IMVDefine.IMV_SaveImageToFileParam();
                stSaveImageToFileParam.eImageType = (IMVDefine.IMV_ESaveFileType)format;
                stSaveImageToFileParam.nWidth = frame.frameInfo.width;
                stSaveImageToFileParam.nHeight = frame.frameInfo.height;
                stSaveImageToFileParam.ePixelFormat = frame.frameInfo.pixelFormat;
                stSaveImageToFileParam.pSrcData = frame.pData;
                stSaveImageToFileParam.nSrcDataLen = frame.frameInfo.size;
                stSaveImageToFileParam.eBayerDemosaic = IMVDefine.IMV_EBayerDemosaic.demosaicEdgeSensing;
                if (stSaveImageToFileParam.eImageType == IMVDefine.IMV_ESaveFileType.typeSaveBmp)
                {
                    stSaveImageToFileParam.pImagePath = "Image_w" + stSaveImageToFileParam.nWidth.ToString() + "_h" +
                                                        stSaveImageToFileParam.nHeight.ToString() + "_blockId" +
                                                        frame.frameInfo.blockId.ToString() + ".bmp";
                }
                else if (stSaveImageToFileParam.eImageType == IMVDefine.IMV_ESaveFileType.typeSaveJpeg)
                {
                    stSaveImageToFileParam.nQuality = 90;
                    stSaveImageToFileParam.pImagePath = "Image_w" + stSaveImageToFileParam.nWidth.ToString() + "_h" +
                                                        stSaveImageToFileParam.nHeight.ToString() + "_blockId" +
                                                        frame.frameInfo.blockId.ToString() + ".jpg";
                }
                else if (stSaveImageToFileParam.eImageType == IMVDefine.IMV_ESaveFileType.typeSavePng)
                {
                    stSaveImageToFileParam.nQuality = 8;
                    stSaveImageToFileParam.pImagePath = "Image_w" + stSaveImageToFileParam.nWidth.ToString() + "_h" +
                                                        stSaveImageToFileParam.nHeight.ToString() + "_blockId" +
                                                        frame.frameInfo.blockId.ToString() + ".png";
                }
                else if (stSaveImageToFileParam.eImageType == IMVDefine.IMV_ESaveFileType.typeSaveTiff)
                {
                    stSaveImageToFileParam.pImagePath = "Image_w" + stSaveImageToFileParam.nWidth.ToString() + "_h" +
                                                        stSaveImageToFileParam.nHeight.ToString() + "_blockId" +
                                                        frame.frameInfo.blockId.ToString() + ".tif";
                }

                int nRet = cam.IMV_SaveImageToFile(ref stSaveImageToFileParam);
                if (nRet != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Save image failed! ErrorCode[{0}]", nRet);
                }
            }           
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
