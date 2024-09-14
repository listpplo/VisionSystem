using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MVSDK_Net;
using System.Runtime.InteropServices;

namespace CommPropAccess
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

                // 修改相机曝光时间，通用double型属性访问实例 
                // set CamDev's ExposureTime, an example of double value property access 
                if (modifyCamDevExposureTime() != IMVDefine.IMV_OK) break;

                // 修改相机像素宽度，通用int型属性访问实例 
                // set CamDev's Width, an example of integer value property access 
                if (modifyCamDevWidth() != IMVDefine.IMV_OK) break;

                // 修改相机ReverseX，通用bool型属性访问实例 
                // set camera's ReverseX, an example of boolean value property access
                if (modifyCamDevReverseX() != IMVDefine.IMV_OK) break;

                // 修改相机DeviceUserID，通用string型属性访问实例 
                // set camera's DeviceUserID, an example of string value property access 
                if (modifyDeviceUserID() != IMVDefine.IMV_OK) break;

                // 修改相机TriggerSelector，通用enum型属性访问实例 
                // set camera's TriggerSelector, an example of enumeration value property access
                if (modifyCamDevTriggerSelector() != IMVDefine.IMV_OK) break;

                // 修改相机TriggerMode，通用enum型属性访问实例 
                // set camera's TriggerMode, an example of enumeration value property access
                if (modifyCamDevTriggerMode() != IMVDefine.IMV_OK) break;

                // 修改相机TriggerSource，通用enum型属性访问实例 
                // set camera's TriggerSource, an example of enumeration value property access
                if (modifyCamDevTriggerSource() != IMVDefine.IMV_OK) break;

                // 执行相机TriggerSoftware，通用command型属性访问实例 
                // execute camera's TriggerSoftware, an example of command type property access 
                if (executeTriggerSoftware() != IMVDefine.IMV_OK) break;

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

        #region 修改相机属性

        //double
        private static int modifyCamDevExposureTime()
        {
            int res = IMVDefine.IMV_OK;
            double exposureTimeValue = 0.0;
            double exposureMinValue = 0;
            double exposureMaxValue = 0;

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetDoubleFeatureValue("ExposureTime", ref exposureTimeValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,exposureTime is {0:F}", exposureTimeValue);

            // 获取属性可设的最小值
            // Get property's settable minimum value
            res = cam.IMV_GetDoubleFeatureMin("ExposureTime", ref exposureMinValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature minimum value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("exposureTime settable minimum value is {0:F}", exposureMinValue);

            // 获取属性可设的最大值
            // Get property's settable maximum value
            res = cam.IMV_GetDoubleFeatureMax("ExposureTime", ref exposureMaxValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature maximum value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("exposureTime settable maximum value is {0:F}", exposureMaxValue);

            if (exposureTimeValue < exposureMinValue + 2.0)
                exposureTimeValue += 2.0;
            else
                exposureTimeValue -= 2.0;

            // 设置属性值
            // Set feature value 
            res = cam.IMV_SetDoubleFeatureValue("ExposureTime", exposureTimeValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetDoubleFeatureValue("ExposureTime", ref exposureTimeValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            Console.WriteLine("After change ,exposureTime is {0:F}", exposureTimeValue);
            return res;
        }

        //init
        private static int modifyCamDevWidth()
        {
            int res = IMVDefine.IMV_OK;
            Int64 widthValue = 0;
            Int64 widthMinValue = 0;
            Int64 widthMaxValue = 0;
            Int64 incrementValue = 0;

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetIntFeatureValue("Width", ref widthValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,Width is {0}", widthValue);

            // 获取属性可设的最小值
            // Get property's settable minimum value
            res = cam.IMV_GetIntFeatureMin("Width", ref widthMinValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get Width minimum value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Width settable minimum value is {0}", widthMinValue);

            // 获取属性可设的最大值
            // Get property's settable maximum value
            res = cam.IMV_GetIntFeatureMax("Width", ref widthMaxValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get Width maximum value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Width settable maximum value is {0}", widthMaxValue);

            // 获取属性步长
            // Get feature increment
            res = cam.IMV_GetIntFeatureInc("Width", ref incrementValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get Width increment value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("width increment value is {0}", incrementValue);

            if (widthValue < widthMinValue + incrementValue)
                widthValue += incrementValue;
            else
                widthValue -= incrementValue;

            // 设置属性值
            // Set feature value 
            res = cam.IMV_SetIntFeatureValue("Width", widthValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set Width feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetIntFeatureValue("Width", ref widthValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get Width feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            Console.WriteLine("After change ,Width is {0}", widthValue);
            return res;
        }

        //bool
        private static int modifyCamDevReverseX()
        {
            int res = IMVDefine.IMV_OK;
            bool reverseXValue = false;

            if (!cam.IMV_FeatureIsValid("ReverseX") || !cam.IMV_FeatureIsAvailable("ReverseX") ||
                !cam.IMV_FeatureIsReadable("ReverseX") || !cam.IMV_FeatureIsWriteable("ReverseX"))
            {
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetBoolFeatureValue("ReverseX", ref reverseXValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,reverseX is {0}", reverseXValue);

            // 设置属性值
            // Set feature value 
            res = cam.IMV_SetBoolFeatureValue("ReverseX", !reverseXValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetBoolFeatureValue("ReverseX", ref reverseXValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            Console.WriteLine("After change ,reverseX is {0}", reverseXValue);
            return res;
        }

        //string
        private static int modifyDeviceUserID()
        {
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_String stringValue = new IMVDefine.IMV_String();

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetStringFeatureValue("DeviceUserID", ref stringValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,DeviceUserID is {0}", stringValue.str);

            // 设置属性值
            // Set feature value 
            res = cam.IMV_SetStringFeatureValue("DeviceUserID", "CamDev");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetStringFeatureValue("DeviceUserID", ref stringValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("After change ,DeviceUserID is {0}", stringValue.str);
            return res;
        }

        //enum
        private static int modifyCamDevTriggerSelector()
        {
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_String enumSymbolValue = new IMVDefine.IMV_String();

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetEnumFeatureSymbol("TriggerSelector", ref enumSymbolValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,TriggerSelector is {0}", enumSymbolValue.str);

            // 设置属性值
            // Set feature value 
            res = cam.IMV_SetEnumFeatureSymbol("TriggerSelector", "FrameStart");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetEnumFeatureSymbol("TriggerSelector", ref enumSymbolValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("After change ,TriggerSelector is {0}", enumSymbolValue.str);
            return res;
        }

        //enum
        private static int modifyCamDevTriggerSource()
        {
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_EnumEntryList enumEntryList = new IMVDefine.IMV_EnumEntryList();
            int index = 0;
            UInt64 enumValue = 0;
            uint nEntryNum = 0;

            // 获取枚举属性的可设枚举值的个数
            //  Get the number of enumeration property settable enumeration
            res = cam.IMV_GetEnumFeatureEntryNum("TriggerSource", ref nEntryNum);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get settable enumeration number failed! ErrorCode[{0}]", res);
                return res;
            }

            IMVDefine.IMV_EnumEntryInfo[] info = new IMVDefine.IMV_EnumEntryInfo[nEntryNum];
            enumEntryList.nEnumEntryBufferSize = (uint) Marshal.SizeOf(typeof (IMVDefine.IMV_EnumEntryInfo))*nEntryNum;
            enumEntryList.pEnumEntryInfo = Marshal.AllocHGlobal((int) enumEntryList.nEnumEntryBufferSize);

            if (enumEntryList.pEnumEntryInfo == IntPtr.Zero)
            {
                Console.WriteLine("pEnumEntryInfo is NULL");
                return res;
            }

            // 获取枚举属性的可设枚举值列表
            // Get enumeration property's settable enumeration value list
            res = cam.IMV_GetEnumFeatureEntrys("TriggerSource", ref enumEntryList);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get settable enumeration value failed! ErrorCode[{0}]", res);
                return res;
            }

            for (index = 0; index < nEntryNum; index++)
            {
                info[index] =
                    (IMVDefine.IMV_EnumEntryInfo)
                        Marshal.PtrToStructure(
                            enumEntryList.pEnumEntryInfo + Marshal.SizeOf(typeof (IMVDefine.IMV_EnumEntryInfo))*index,
                            typeof (IMVDefine.IMV_EnumEntryInfo));
                Console.WriteLine("Enum Entry Name[{0}] <-> Enum Entry Value[{1}]", info[index].name, info[index].value);
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetEnumFeatureValue("TriggerSource", ref enumValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            for (index = 0; index < nEntryNum; index++)
            {
                if (info[index].value == enumValue)
                {
                    Console.WriteLine("Before change,triggerSource is {0}", info[index].name);
                    break;
                }

            }

            for (index = 0; index < nEntryNum; index++)
            {
                if (string.Equals(info[index].name, "Software"))
                {
                    enumValue = info[index].value;
                    break;
                }
            }

            // 设置属性值
            // Set feature value 
            res = cam.IMV_SetEnumFeatureValue("TriggerSource", enumValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetEnumFeatureValue("TriggerSource", ref enumValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            for (index = 0; index < nEntryNum; index++)
            {
                if (info[index].value == enumValue)
                {
                    Console.WriteLine("After change ,triggerSource is {0}", info[index].name);
                    break;
                }
            }

            if (enumEntryList.pEnumEntryInfo != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(enumEntryList.pEnumEntryInfo);
                enumEntryList.pEnumEntryInfo = IntPtr.Zero;
            }

            return res;
        }

        private static int modifyCamDevTriggerMode()
        {
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_String enumSymbolValue = new IMVDefine.IMV_String();

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetEnumFeatureSymbol("TriggerMode", ref enumSymbolValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,TriggerMode is {0}", enumSymbolValue.str);

            // 设置属性值
            // Set feature value 
            res = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "Off");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_GetEnumFeatureSymbol("TriggerMode", ref enumSymbolValue);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("After change ,TriggerMode is {0}", enumSymbolValue.str);
            return res;
        }

        //command
        private static int executeTriggerSoftware()
        {
            int res = IMVDefine.IMV_OK;
            // 执行命令属性
            // Execute command property
            res = cam.IMV_ExecuteCommandFeature("TriggerSoftware");
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Execute TriggerSoftware failed! ErrorCode[{0}]", res);
                return res;
            }

            Console.WriteLine("Execute TriggerSoftware success.");
            return res;
        }


        #endregion

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
