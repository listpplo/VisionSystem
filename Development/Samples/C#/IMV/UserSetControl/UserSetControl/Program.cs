using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MVSDK_Net;

namespace UserSetControl
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

                // 恢复相机默认配置
                // Restore camera's default configuration
                res = restoreDefault();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Restore default failed! ErrorCode:[{0}]", res);
                    break;
                }

                Console.WriteLine("Restore default successfully...");

                // 保存相机当前配置到某配置集合，如userSet1
                // Save current configuration into certain userset, for example, userSet1
                res = saveUserConfiguration();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Save user set failed! ErrorCode:[{0}]", res);
                    break;
                }

                Console.WriteLine("Save configuration to userSet1 successfully...");

                // 加载相机当前配置
                // Load current camera configuration
                res = LoadUserConfiguration();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Load current set failed! ErrorCode:[{0}]", res);
                    break;
                }

                Console.WriteLine("Load configuration from userSet1 successfully...");

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

        private static int restoreDefault()
        {
            int res = IMVDefine.IMV_OK;

            //1、选择默认配置为当前配置
            //1、Select the default configuration as the current configuration
            res = cam.IMV_SetEnumFeatureSymbol("UserSetSelector", "Default");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set UserSetSelector feature value failed! ErrorCode[{0}]",res);
                return res;
            }

            //2、加载默认配置
            //2、Load the default configuration
            res = cam.IMV_ExecuteCommandFeature("UserSetLoad");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Excute UserSetLoad property failed! ErrorCode[{0}]", res);
                return res;
            }

            //3、设置默认配置为下次相机启动时使用的默认配置
            //3、Set the default configuration as the default configuration when the camera is started
            res = cam.IMV_SetEnumFeatureSymbol("UserSetDefault", "Default");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set UserSetDefault feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            return res;
        }

        private static int saveUserConfiguration()
        {
            int res = IMVDefine.IMV_OK;

            //1、选择当前配置为UserSet1
            //1、Select the UserSet1 configuration as the current configuration
            res = cam.IMV_SetEnumFeatureSymbol("UserSetSelector", "UserSet1");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set UserSetSelector feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            //2、保存配置到UserSet1
            //2、Save configuration to UserSet1
            res = cam.IMV_ExecuteCommandFeature("UserSetSave");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Excute UserSetSave property failed! ErrorCode[{0}]", res);
                return res;
            }

            return res;
        }

        private static int LoadUserConfiguration()
        {
            int res = IMVDefine.IMV_OK;

            //1、选择当前配置为UserSet1
            //1、Select the UserSet1 configuration as the current configuration
            res = cam.IMV_SetEnumFeatureSymbol("UserSetSelector", "UserSet1");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set UserSetSelector feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            //2、加载UserSet1配置
            //2、Load the UserSet1 configuration
            res = cam.IMV_ExecuteCommandFeature("UserSetLoad");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Excute UserSetLoad property failed! ErrorCode[{0}]", res);
                return res;
            }

            //3、设置UserSet1配置为下次相机启动时使用的默认配置
            //3、Set the UserSet1 configuration as the default configuration when the camera is started
            res = cam.IMV_SetEnumFeatureSymbol("UserSetDefault", "UserSet1");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set UserSetDefault feature value failed! ErrorCode[{0}]", res);
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
