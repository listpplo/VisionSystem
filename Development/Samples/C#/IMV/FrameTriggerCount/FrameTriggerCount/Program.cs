using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MVSDK_Net;

namespace FrameTriggerCount
{
    internal class Program
    {
        private static MyCamera cam = new MyCamera();
        private static IMVDefine.IMV_FrameCallBack frameCallBack;
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

                setSoftTriggerConf();

                // 注册数据帧回调函数
                // Register data frame callback function
                frameCallBack = onGetFrame;
                res = cam.IMV_AttachGrabbing(frameCallBack, IntPtr.Zero);
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

                // 创建软触发线程 
                // Create soft trigger thread
                Thread threadHandle = new Thread(new ThreadStart(executeSoftTriggerProc));
                threadHandle.Start();

                // 取图2秒
                // get frame 2 seconds
                Thread.Sleep(2000);

                // 退出软触发线程 
                // Stop soft trigger thread 
                g_isExitThread = true;
                Thread.Sleep(500);

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

        private static bool g_isExitThread = false;

        private static void executeSoftTriggerProc()
        {
            int res = IMVDefine.IMV_OK;
            while (!g_isExitThread)
            {

                // 执行软触发
                // Execute soft trigger 
                res = cam.IMV_ExecuteCommandFeature("TriggerSoftware");
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Execute TriggerSoftware failed! ErrorCode:[{0}]", res);
                    continue;
                }

                // 通过睡眠时间来调节帧率
                // Adjust the frame rate by sleep time
                Thread.Sleep(200);
            }

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

            // 获取帧触发数 
            // Get frame trigger count
            long frameTriggerCount = 0;
            int res = cam.IMV_GetIntFeatureValue("FrameTriggerCount", ref frameTriggerCount);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get FrameTriggerCount value failed! ErrorCode:[{0}]", res);
                return;
            }

            // 获取帧触发丢失数 
            // Get frame trigger lost count
            long frameTriggerLostCount = 0;
            res = cam.IMV_GetIntFeatureValue("FrameTriggerLostCount", ref frameTriggerLostCount);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get FrameTriggerLostCount value failed! ErrorCode:[{0}]", res);
                return;
            }
            Console.WriteLine("Frame blockId = {0}, FrameTriggerCount = {1}, FrameTriggerLostCount = {2}",
                frame.frameInfo.blockId,
                frameTriggerCount, frameTriggerLostCount);
        }

        private static int setSoftTriggerConf()
        {
            int res = IMVDefine.IMV_OK;

            // 设置触发源为软触发 
            // Set trigger source to Software 
            res = cam.IMV_SetEnumFeatureSymbol("TriggerSource", "Software");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set triggerSource value failed! ErrorCode:[{0}]", res);
                return res;
            }

            // 设置触发器 
            // Set trigger selector to FrameStart 
            res = cam.IMV_SetEnumFeatureSymbol("TriggerSelector", "FrameStart");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set triggerSelector value failed! ErrorCode:[{0}]", res);
                return res;
            }

            // 设置触发模式 
            // Set trigger mode to On 
            res = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "On");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set triggerMode value failed! ErrorCode:[{0}]", res);
                return res;
            }

            // 触发计数清0 
            // Clear trigger count
            res = cam.IMV_ExecuteCommandFeature("FrameTriggerCountReset");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Execute FrameTriggerCountReset value failed! ErrorCode:[{0}]", res);
                return res;
            }
            return res;
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
