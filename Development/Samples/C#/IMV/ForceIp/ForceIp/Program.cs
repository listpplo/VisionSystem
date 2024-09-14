using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.Versioning;
using System.Security.Permissions;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MVSDK_Net;

namespace ForceIp
{
    internal class Program
    {
        private static MyCamera cam = new MyCamera();
        private static IMVDefine.IMV_FrameCallBack frameCallBack;
        private static void Main(string[] args)
        {
            Console.WriteLine("SDK Version:{0}", MyCamera.IMV_GetVersion());
            Console.WriteLine("Enum camera device.");

            // 发现GigE相机
            // discover GigE camera 
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_DeviceList deviceList = new IMVDefine.IMV_DeviceList();
            IMVDefine.IMV_EInterfaceType interfaceTp = IMVDefine.IMV_EInterfaceType.interfaceTypeGige;
            res = MyCamera.IMV_EnumDevices(ref deviceList, (uint) interfaceTp);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Enumeration Gige devices failed! ErrorCode:[{0}]", res);
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

                // 检查和设置设备IP
                // Check and set the device IP
                res = autoSetCameraIP();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Set camera IP failed! ErrorCode[{0}]", res);
                    break;
                }

                // 判断相机是否打开状态
                // Check whether device is open
                if (!cam.IMV_IsOpen())
                {
                    // 打开相机(返回错误码为IMV_ERROR_INVALID_IP，表示设备与主机网段不匹配)
                    // Open camera( Return error code:IMV_ERROR_INVALID_IP,indicating that the device and PC subnet is mismatch)
                    res = cam.IMV_Open();
                    if (res != IMVDefine.IMV_OK)
                    {
                        Console.WriteLine("Open camera failed! ErrorCode:[{0}]", res);
                        break;
                    }
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

        private static int autoSetCameraIP()
        {
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_DeviceInfo deviceInfo = new IMVDefine.IMV_DeviceInfo();

            // 获取设备信息
            // Get device information
            res = cam.IMV_GetDeviceInfo(ref deviceInfo);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get device info failed! ErrorCode[{0}]", res);
                return res;
            }

            // 判断设备和主机IP的网段是否匹配。匹配(Valid) 不匹配(Invalid On This Interface)
            // Check whether match of device and PC subnet.match(Valid) mismatch(Invalid On This Interface)
            IMVDefine.IMV_GigEDeviceInfo gigEDeviceInfo =
                (IMVDefine.IMV_GigEDeviceInfo)
                    ByteToStruct(deviceInfo.deviceSpecificInfo.gigeDeviceInfo, typeof (IMVDefine.IMV_GigEDeviceInfo));
            if (gigEDeviceInfo.ipConfiguration.Equals("Valid")) return IMVDefine.IMV_OK;

            Console.WriteLine("Device ip address (before):[{0}]", gigEDeviceInfo.ipAddress);
            Console.WriteLine("Device subnetMask (before):[{0}]", gigEDeviceInfo.subnetMask);
            Console.WriteLine("Device defaultGateWay (before):[{0}]", gigEDeviceInfo.defaultGateWay);
            Console.WriteLine("Device macAddress (before):[{0}]", gigEDeviceInfo.macAddress);
            Console.WriteLine();

            IMVDefine.IMV_GigEInterfaceInfo gigEInterfaceInfo =
                (IMVDefine.IMV_GigEInterfaceInfo)
                    ByteToStruct(deviceInfo.interfaceInfo.gigeInterfaceInfo, typeof(IMVDefine.IMV_GigEInterfaceInfo));
            Console.WriteLine("Interface ip address :[{0}]", gigEInterfaceInfo.ipAddress);
            Console.WriteLine("Interface subnetMask :[{0}]", gigEInterfaceInfo.subnetMask);
            Console.WriteLine("Interface defaultGateWay :[{0}]", gigEInterfaceInfo.defaultGateWay);
            Console.WriteLine("Interface macAddress :[{0}]", gigEInterfaceInfo.macAddress);

            String[] subnetStr = gigEInterfaceInfo.ipAddress.Split('.');
            int ipValue = 253;
            while (ipValue != 0)
            {
                if (ipValue != Convert.ToUInt32(subnetStr[3]))
                {
                    break;
                }
                ipValue--;
            }
            string newIPStr = string.Format("{0}.{1}.{2}.{3}", subnetStr[0], subnetStr[1], subnetStr[2], ipValue);

            Console.WriteLine("\nNew device ip address:[{0}]\n", newIPStr);

            res = cam.IMV_GIGE_ForceIpAddress(newIPStr, gigEInterfaceInfo.subnetMask, gigEInterfaceInfo.defaultGateWay);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set device ip failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取设备信息
            // Get device information
            res = cam.IMV_GetDeviceInfo(ref deviceInfo);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get device info failed! ErrorCode[{0}]", res);
                return res;
            }
            gigEDeviceInfo =
                (IMVDefine.IMV_GigEDeviceInfo)
                    ByteToStruct(deviceInfo.deviceSpecificInfo.gigeDeviceInfo, typeof (IMVDefine.IMV_GigEDeviceInfo));
            Console.WriteLine("Device ip address (after):[{0}]", gigEDeviceInfo.ipAddress);
            Console.WriteLine("Device subnetMask (after):[{0}]", gigEDeviceInfo.subnetMask);
            Console.WriteLine("Device defaultGateWay (after):[{0}]", gigEDeviceInfo.defaultGateWay);
            Console.WriteLine("Device macAddress (after):[{0}]", gigEDeviceInfo.macAddress);

            // 打开相机(返回错误码为IMV_ERROR_INVALID_IP，表示设备与主机网段不匹配)
            // Open camera( Return error code:IMV_ERROR_INVALID_IP,indicating that the device and PC subnet is mismatch)
            res = cam.IMV_Open();
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Open camera failed! ErrorCode[{0}]", res);
                return res;
            }

            // 设置静态IP
            // Set persistent IP
            res = cam.IMV_SetBoolFeatureValue("GevCurrentIPConfigurationPersistentIP", true);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set GevCurrentIPConfigurationPersistentIP failed! ErrorCode[{0}]", res);
                return res;
            }

            res = cam.IMV_SetStringFeatureValue("GevPersistentIPAddress", gigEDeviceInfo.ipAddress);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set GevPersistentIPAddress failed! ErrorCode[{0}]", res);
                return res;
            }

            res = cam.IMV_SetStringFeatureValue("GevPersistentSubnetMask", gigEDeviceInfo.subnetMask);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set GevPersistentSubnetMask failed! ErrorCode[{0}]", res);
                return res;
            }

            res = cam.IMV_SetStringFeatureValue("GevPersistentDefaultGateway", gigEDeviceInfo.defaultGateWay);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set GevPersistentDefaultGateway failed! ErrorCode[{0}]", res);
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
