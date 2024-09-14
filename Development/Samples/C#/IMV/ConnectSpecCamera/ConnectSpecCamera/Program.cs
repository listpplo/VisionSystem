using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MVSDK_Net;

namespace ConnectSpecCamera
{
    internal class Program
    {
        private static MyCamera cam = new MyCamera();
        private static readonly int specifiedIndex = 0;
        private static readonly int specifiedDeviceUserID = 1;
        private static readonly int specifiedCameraKey = 2;
        private static readonly int specifiedCameraIP = 3;
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
            int camIndex = 0;
            string camStr = "";
            int inputIndex = 0;
            if (!specifiedDevice((int)deviceList.nDevNum, ref camIndex, ref camStr, out inputIndex)) return;
            do
            {
                // 创建设备句柄
                // Create Device Handle
                if (inputIndex == specifiedIndex)
                {
                    res = cam.IMV_CreateHandle(IMVDefine.IMV_ECreateHandleMode.modeByIndex, camIndex);
                }
                else if (inputIndex == specifiedDeviceUserID)
                {
                    res = cam.IMV_CreateHandle(IMVDefine.IMV_ECreateHandleMode.modeByDeviceUserID, cameraStr: camStr);
                }
                else if (inputIndex == specifiedCameraKey)
                {
                    res = cam.IMV_CreateHandle(IMVDefine.IMV_ECreateHandleMode.modeByCameraKey, cameraStr: camStr);
                }
                else if (inputIndex == specifiedCameraIP)
                {
                    res = cam.IMV_CreateHandle(IMVDefine.IMV_ECreateHandleMode.modeByIPAddress, cameraStr: camStr);
                }
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

                // 取图2秒
                // get frame 2 seconds
                Thread.Sleep(2000);

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

        private static void onGetFrame(ref IMVDefine.IMV_Frame frame, IntPtr pUser)
        {
            if (frame.frameHandle == IntPtr.Zero)
            {
                Console.WriteLine("frame is NULL");
                return;
            }
            Console.WriteLine("Get frame blockId = {0}", frame.frameInfo.blockId);
        }

        private static bool specifiedDevice(int num, ref int id, ref string str, out int inputIndex)
        {
            Console.WriteLine("\n--------------------------------------------");
            Console.WriteLine("\t0.Specified Index");
            Console.WriteLine("\t1.Specified DeviceUserID");
            Console.WriteLine("\t2.Specified CameraKey");
            Console.WriteLine("\t3.Specified CameraIP");
            Console.WriteLine("--------------------------------------------");
            Console.WriteLine("\nPlease select the specified camera type: [0-3]");
            int connectWay = 0;
            inputIndex = 0;
            try
            {
                connectWay = Convert.ToInt32(Console.ReadLine());
            }
            catch
            {
                Console.WriteLine("Invalid Input!");
                Console.Read();
                return false;
            }
            if (connectWay > 3 || connectWay < 0)
            {
                Console.WriteLine("Input Error!");
                Console.Read();
                return false;
            }
            try
            {
                switch (connectWay)
                {
                    case 0:
                        Console.WriteLine("Please input the index of specified camera: [0-{0:d}]", num - 1);
                        id = Convert.ToInt32(Console.ReadLine());
                        if (id < 0 || id > num - 1)
                        {
                            Console.WriteLine("Input Error!");
                            Console.Read();
                            return false;
                        }
                        break;
                    case 1:
                        Console.WriteLine("Please input the DeviceUserID of specified camera:");
                        str = Console.ReadLine();
                        inputIndex = 1;
                        break;
                    case 2:
                        Console.WriteLine("Please input the CameraKey of specified camera:");
                        str = Console.ReadLine();
                        inputIndex = 2;
                        break;
                    case 3:
                        Console.WriteLine("Please input the CameraIP of specified camera:");
                        str = Console.ReadLine();
                        inputIndex = 3;
                        break;
                }
            }
            catch (Exception)
            {
                Console.WriteLine("Invalid Input!");
                Console.Read();
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
                // 设备的自定义名称
                // DeviceUserID 
                Console.WriteLine("DeviceUserID : {0}", deviceInfo.cameraName);
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
