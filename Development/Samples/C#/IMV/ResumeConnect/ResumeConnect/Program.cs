using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MVSDK_Net;

namespace ResumeConnect
{
    internal class Program
    {
        private static MyCamera cam = new MyCamera();
        private static IMVDefine.IMV_ConnectCallBack pConnectCallBack;
        private static IMVDefine.IMV_FrameCallBack pFrameCallBack;

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

                // 设备连接状态事件回调函数
                // Device connection status event callback function
                pConnectCallBack = onDeviceLinkNotify;
                res = cam.IMV_SubscribeConnectArg(pConnectCallBack, IntPtr.Zero);
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("SubscribeConnectArg failed! ErrorCode:[{0}]", res);
                    break;
                }

                // 注册数据帧回调函数
                // Register data frame callback function
                pFrameCallBack = onGetFrame;
                res = cam.IMV_AttachGrabbing(pFrameCallBack, IntPtr.Zero);
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Attach grabbing failed! ErrorCode:[{0}]", res);
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

                Console.WriteLine("Press enter to stop grabbing");
                Console.ReadKey();

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
            g_isMainThreadExit = true;
        }

        // 数据帧回调函数
        // Data frame callback function
        private static void onGetFrame(ref IMVDefine.IMV_Frame frame, IntPtr pUser)
        {
            if (frame.frameHandle == IntPtr.Zero)
            {
                Console.WriteLine("frame is NULL");
                return;
            }
            Console.WriteLine("Get frame blockId = {0}", frame.frameInfo.blockId);
        }

        // 断线通知处理
        // offLine notify processing
        private static void deviceOffLine()
        {
            // 停止拉流 
            // Stop grabbing 
            cam.IMV_StopGrabbing();
        }

        private static bool g_isMainThreadExit = false;

        // 上线通知处理
        // onLine notify processing
        private static void deviceOnLine()
        {
            int res = IMVDefine.IMV_OK;

            // 关闭相机
            // Close camera 
            cam.IMV_Close();

            do
            {
                if (g_isMainThreadExit) return;

                res = cam.IMV_Open();
                if (res != IMVDefine.IMV_OK) Console.WriteLine("Retry open camera failed! ErrorCode[{0}]", res);
                else
                {
                    Console.WriteLine("Retry open camera successfully!");
                    break;
                }

                Thread.Sleep(500);
            } while (true);

           
            // 重新设备连接状态事件回调函数
            // Device connection status event callback function again 
            pConnectCallBack = onDeviceLinkNotify;
            res = cam.IMV_SubscribeConnectArg(pConnectCallBack, IntPtr.Zero);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("SubscribeConnectArg failed! ErrorCode[{0}]", res);
            }

            
            // 重新注册数据帧回调函数
            // Register data frame callback function again
            pFrameCallBack = onGetFrame;
            res = cam.IMV_AttachGrabbing(pFrameCallBack, IntPtr.Zero);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Attach grabbing failed! ErrorCode[{0}]", res);
            }

            // 开始拉流 
            // Start grabbing 
            res = cam.IMV_StartGrabbing();
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Start grabbing failed! ErrorCode[{0}]", res);
            }
            else
            {
                Console.WriteLine("Start grabbing successfully");
            }
        }


        // 连接事件通知回调函数
        // Connect event notify callback function
        static void onDeviceLinkNotify(ref IMVDefine.IMV_SConnectArg pConnectArg, IntPtr pUser)
        {
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_DeviceInfo devInfo=new IMVDefine.IMV_DeviceInfo();

            res = cam.IMV_GetDeviceInfo(ref devInfo);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get device info failed! ErrorCode[{0}]", res);
                return;
            }

            // 断线通知
            // offLine notify 
            if (IMVDefine.IMV_EVType.offLine == pConnectArg.EvType)
            {
                Console.WriteLine("------cameraKey[{0}] : OffLine------",devInfo.cameraKey);
                deviceOffLine();
            }
            // 上线通知
            // onLine notify 
            else if (IMVDefine.IMV_EVType.onLine == pConnectArg.EvType)
            {
                Console.WriteLine("------cameraKey[{0}] : OnLine------", devInfo.cameraKey);
                deviceOnLine();
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
