using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MVSDK_Net;

namespace ClearFrameBuffer
{
    internal class Program
    {
        private static MyCamera cam = new MyCamera();

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

                // 连续取流模式
                // continuous mode
                res = setGrabMode(true);
                if (res != IMVDefine.IMV_OK) break;

                // 开始拉流 
                // Start grabbing 
                res = cam.IMV_StartGrabbing();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Start grabbing failed! ErrorCode:[{0}]", res);
                    break;
                }

                // 连续取流模式切换成触发模式进行清除帧数据缓存
                // Clear frame buffer from continuous mode to trigger mode
                res = contiousToTriggerClearFrameBuffer();
                if (res != IMVDefine.IMV_OK) break;

                // 触发模式下清除帧数据缓存
                // Clear frame buffer on trigger mode
                res = triggerModeClearFrameBuffer();
                if (res != IMVDefine.IMV_OK) break;

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

        static int setGrabMode(bool isContinous)
        {
            int res = IMVDefine.IMV_OK;

            // 设置触发器 
            // Set trigger selector to FrameStart
            res = cam.IMV_SetEnumFeatureSymbol("TriggerSelector", "FrameStart");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set triggerSelector value failed! ErrorCode[{0}]", res);
                return res;
            }

            if (isContinous)
            {
                // 关闭触发模式 
                // Set trigger mode to Off 
                res = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "Off");
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Set triggerMode value failed! ErrorCode[{0}]", res);
                    return res;
                }
            }
            else
            {
                // 设置触发源为软触发 
                // Set trigger source to Software
                res = cam.IMV_SetEnumFeatureSymbol("TriggerSource", "Software");
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Set triggerSource value failed! ErrorCode[{0}]", res);
                    return res;
                }

                // 设置触发模式 
                // Set trigger mode to On 
                res = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "On");
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Set triggerMode value failed! ErrorCode[{0}]", res);
                    return res;
                }
            }
            return res;

        }

        static int contiousToTriggerClearFrameBuffer()
        {
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_Frame frame=new IMVDefine.IMV_Frame();

            // 取图1秒
            // get frame 1 seconds
            Thread.Sleep(1000);

            // 设置软触发配置 
            // Set software trigger config 
            res = setGrabMode(false);
            if (res != IMVDefine.IMV_OK)
            {
                return res;
            }

            // 连续取流模式需要完全接收网络中残留的帧数据
            // Continuous mode need to fully receive the ramaining frame data in the network
            Thread.Sleep(500);

            // 清除帧数据缓存
            // Clear frame buffer
            res = cam.IMV_ClearFrameBuffer();
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Clear frame buffer failed! ErrorCode[{0}]", res);
                return res;
            }

            // 执行软触发
            // Execute soft trigger 
            res = cam.IMV_ExecuteCommandFeature("TriggerSoftware");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Execute TriggerSoftware failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取一帧图像
            // Get a frame image
            res = cam.IMV_GetFrame(ref frame, 1000);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get frame failed! ErrorCode[{0}]", res);
                return res;
            }

            Console.WriteLine("Get frame blockId = {0}",frame.frameInfo.blockId);

            // 释放图像缓存
            // Free image buffer
            res = cam.IMV_ReleaseFrame(ref frame);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Release frame failed! ErrorCode[{0}]", res);
                return res;
            }
            return res;
        }

        static int triggerModeClearFrameBuffer()
        {
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_Frame frame=new IMVDefine.IMV_Frame();

            // 清除帧数据缓存
            // Clear frame buffer
            res = cam.IMV_ClearFrameBuffer();
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Clear frame buffer failed! ErrorCode[{0}]", res);
                return res;
            }

            // 执行软触发
            // Execute soft trigger 
            res = cam.IMV_ExecuteCommandFeature("TriggerSoftware");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Execute TriggerSoftware failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取一帧图像
            // Get a frame image
            res = cam.IMV_GetFrame(ref frame, 1000);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get frame failed! ErrorCode[{0}]", res);
                return res;
            }

            Console.WriteLine("Get frame blockId = {0}", frame.frameInfo.blockId);

            // 释放图像缓存
            // Free image buffer
            res = cam.IMV_ReleaseFrame(ref frame);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Release frame failed! ErrorCode[{0}]", res);
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
