using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CaptureCard_Net;
using System.Runtime.InteropServices;

namespace CommPropAccess
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

                // 修改相机曝光时间，通用double型属性访问实例 
                // set CamDev's ExposureTime, an example of double value property access 
                if (modifyCamDevExposureTime() != IMVFGDefine.IMV_FG_OK) break;

                // 修改相机像素宽度，通用int型属性访问实例 
                // set CamDev's Width, an example of integer value property access 
                if (modifyCamDevWidth() != IMVFGDefine.IMV_FG_OK) break;

                // 修改相机ReverseX，通用bool型属性访问实例 
                // set camera's ReverseX, an example of boolean value property access
                if (modifyCamDevReverseX() != IMVFGDefine.IMV_FG_OK) break;

                // 修改相机DeviceUserID，通用string型属性访问实例 
                // set camera's DeviceUserID, an example of string value property access 
                if (modifyDeviceUserID() != IMVFGDefine.IMV_FG_OK) break;

                // 修改相机TriggerSelector，通用enum型属性访问实例 
                // set camera's TriggerSelector, an example of enumeration value property access
                if (modifyCamDevTriggerSelector() != IMVFGDefine.IMV_FG_OK) break;

                // 修改相机TriggerMode，通用enum型属性访问实例 
                // set camera's TriggerMode, an example of enumeration value property access
                if (modifyCamDevTriggerMode() != IMVFGDefine.IMV_FG_OK) break;

                // 修改相机TriggerSource，通用enum型属性访问实例 
                // set camera's TriggerSource, an example of enumeration value property access
                if (modifyCamDevTriggerSource() != IMVFGDefine.IMV_FG_OK) break;

                // 修改采集卡像素宽度，通用int型属性访问实例 
                // set CamDev's Width, an example of integer value property access 
                if (modifyCardDevWidth() != IMVFGDefine.IMV_FG_OK) break;

                // 修改采集卡StrobeSignalWidth，通用double型属性访问实例 
                // set CamDev's StrobeSignalWidth, an example of double value property access 
                if (modifyCardDevStrobeSignalWidth() != IMVFGDefine.IMV_FG_OK) break;

                // 修改采集卡ReverseY，通用bool型属性访问实例 
                // set camera's ReverseY, an example of boolean value property access
                if (modifyCardDevReverseY() != IMVFGDefine.IMV_FG_OK) break;

                // 修改采集卡PixelFormat，通用enum型属性访问实例 
                // set camera's TriggerSelector, an example of enumeration value property access
                if (modifyCardDevPixelFormat() != IMVFGDefine.IMV_FG_OK) break;

                // 修改采集卡DeviceUserID，通用string型属性访问实例 
                // set camera's DeviceUserID, an example of string value property access 
                if (modifyCardDeviceUserID() != IMVFGDefine.IMV_FG_OK) break;

                // 执行采集卡TriggerSoftware，通用command型属性访问实例 
                // execute capture baord's TriggerSoftware, an example of command type property access
                if (executeCardDevTriggerSoftware() != IMVFGDefine.IMV_FG_OK) break;

                // 执行相机TriggerSoftware，通用command型属性访问实例 
                // execute camera's TriggerSoftware, an example of command type property access 
                if (executeTriggerSoftware() != IMVFGDefine.IMV_FG_OK) break;

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

        #region 修改相机属性

        //double
        private static int modifyCamDevExposureTime()
        {
            int res = IMVFGDefine.IMV_FG_OK;
            double exposureTimeValue = 0.0;
            double exposureMinValue = 0;
            double exposureMaxValue = 0;

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetDoubleFeatureValue("ExposureTime", ref exposureTimeValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,exposureTime is {0:F}", exposureTimeValue);

            // 获取属性可设的最小值
            // Get property's settable minimum value
            res = cam.IMV_FG_GetDoubleFeatureMin("ExposureTime", ref exposureMinValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature minimum value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("exposureTime settable minimum value is {0:F}", exposureMinValue);

            // 获取属性可设的最大值
            // Get property's settable maximum value
            res = cam.IMV_FG_GetDoubleFeatureMax("ExposureTime", ref exposureMaxValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            res = cam.IMV_FG_SetDoubleFeatureValue("ExposureTime", exposureTimeValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetDoubleFeatureValue("ExposureTime", ref exposureTimeValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            int res = IMVFGDefine.IMV_FG_OK;
            Int64 widthValue = 0;
            Int64 widthMinValue = 0;
            Int64 widthMaxValue = 0;
            Int64 incrementValue = 0;

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetIntFeatureValue("Width", ref widthValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,Width is {0}", widthValue);

            // 获取属性可设的最小值
            // Get property's settable minimum value
            res = cam.IMV_FG_GetIntFeatureMin("Width", ref widthMinValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get Width minimum value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Width settable minimum value is {0}", widthMinValue);

            // 获取属性可设的最大值
            // Get property's settable maximum value
            res = cam.IMV_FG_GetIntFeatureMax("Width", ref widthMaxValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get Width maximum value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Width settable maximum value is {0}", widthMaxValue);

            // 获取属性步长
            // Get feature increment
            res = cam.IMV_FG_GetIntFeatureInc("Width", ref incrementValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            res = cam.IMV_FG_SetIntFeatureValue("Width", widthValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set Width feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetIntFeatureValue("Width", ref widthValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            int res = IMVFGDefine.IMV_FG_OK;
            bool reverseXValue = false;

            if (!cam.IMV_FG_FeatureIsValid("ReverseX") || !cam.IMV_FG_FeatureIsAvailable("ReverseX") ||
                !cam.IMV_FG_FeatureIsReadable("ReverseX") || !cam.IMV_FG_FeatureIsWriteable("ReverseX"))
            {
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetBoolFeatureValue("ReverseX", ref reverseXValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,reverseX is {0}", reverseXValue);

            // 设置属性值
            // Set feature value 
            res = cam.IMV_FG_SetBoolFeatureValue("ReverseX", !reverseXValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetBoolFeatureValue("ReverseX", ref reverseXValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            int res = IMVFGDefine.IMV_FG_OK;
            IMVFGDefine.IMV_FG_String stringValue = new IMVFGDefine.IMV_FG_String();

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetStringFeatureValue("DeviceUserID", ref stringValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,DeviceUserID is {0}", stringValue.str);

            // 设置属性值
            // Set feature value 
            res = cam.IMV_FG_SetStringFeatureValue("DeviceUserID", "CamDev");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetStringFeatureValue("DeviceUserID", ref stringValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            int res = IMVFGDefine.IMV_FG_OK;
            IMVFGDefine.IMV_FG_String enumSymbolValue = new IMVFGDefine.IMV_FG_String();

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetEnumFeatureSymbol("TriggerSelector", ref enumSymbolValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,TriggerSelector is {0}", enumSymbolValue.str);

            // 设置属性值
            // Set feature value 
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerSelector", "FrameStart");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetEnumFeatureSymbol("TriggerSelector", ref enumSymbolValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            int res = IMVFGDefine.IMV_FG_OK;
            IMVFGDefine.IMV_FG_EnumEntryList enumEntryList = new IMVFGDefine.IMV_FG_EnumEntryList();
            int index = 0;
            UInt64 enumValue = 0;
            uint nEntryNum = 0;

            // 获取枚举属性的可设枚举值的个数
            //  Get the number of enumeration property settable enumeration
            res = cam.IMV_FG_GetEnumFeatureEntryNum("TriggerSource", ref nEntryNum);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get settable enumeration number failed! ErrorCode[{0}]", res);
                return res;
            }

            IMVFGDefine.IMV_FG_EnumEntryInfo[] info = new IMVFGDefine.IMV_FG_EnumEntryInfo[nEntryNum];
            enumEntryList.nEnumEntryBufferSize = (uint) Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_EnumEntryInfo))*
                                                 nEntryNum;
            enumEntryList.pEnumEntryInfo = Marshal.AllocHGlobal((int) enumEntryList.nEnumEntryBufferSize);

            if (enumEntryList.pEnumEntryInfo == IntPtr.Zero)
            {
                Console.WriteLine("pEnumEntryInfo is NULL");
                return res;
            }

            // 获取枚举属性的可设枚举值列表
            // Get enumeration property's settable enumeration value list
            res = cam.IMV_FG_GetEnumFeatureEntrys("TriggerSource", ref enumEntryList);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get settable enumeration value failed! ErrorCode[{0}]", res);
                return res;
            }

            for (index = 0; index < nEntryNum; index++)
            {
                info[index] =
                    (IMVFGDefine.IMV_FG_EnumEntryInfo)
                        Marshal.PtrToStructure(
                            enumEntryList.pEnumEntryInfo +
                            Marshal.SizeOf(typeof (IMVFGDefine.IMV_FG_EnumEntryInfo))*index,
                            typeof (IMVFGDefine.IMV_FG_EnumEntryInfo));
                Console.WriteLine("Enum Entry Name[{0}] <-> Enum Entry Value[{1}]", info[index].name, info[index].value);
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetEnumFeatureValue("TriggerSource", ref enumValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            res = cam.IMV_FG_SetEnumFeatureValue("TriggerSource", enumValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetEnumFeatureValue("TriggerSource", ref enumValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            int res = IMVFGDefine.IMV_FG_OK;
            IMVFGDefine.IMV_FG_String enumSymbolValue = new IMVFGDefine.IMV_FG_String();

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetEnumFeatureSymbol("TriggerMode", ref enumSymbolValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,TriggerMode is {0}", enumSymbolValue.str);

            // 设置属性值
            // Set feature value 
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerMode", "Off");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = cam.IMV_FG_GetEnumFeatureSymbol("TriggerMode", ref enumSymbolValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            int res = IMVFGDefine.IMV_FG_OK;
            // 执行命令属性
            // Execute command property
            res = cam.IMV_FG_ExecuteCommandFeature("TriggerSoftware");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Execute TriggerSoftware failed! ErrorCode[{0}]", res);
                return res;
            }

            Console.WriteLine("Execute TriggerSoftware success.");
            return res;
        }

        private static int modifyCxpInterfaceProperty()
        {
            int res = IMVFGDefine.IMV_FG_OK;
            do
            {
                //判断属性是否有效可流可读可写
                //Check feature IsAvailable、IsReadable、IsWriteable、IsStreamable、IsValid
                if (cam.IMV_FG_FeatureIsValid("InterfaceID"))
                    Console.WriteLine("Feature InterfaceID Is Valid.");
                else
                    Console.WriteLine("Feature InterfaceID Not Valid.");

                if (cam.IMV_FG_FeatureIsAvailable("InterfaceID"))
                    Console.WriteLine("Feature InterfaceID Is Available.");
                else
                    Console.WriteLine("Feature InterfaceID Not Available.");

                if (cam.IMV_FG_FeatureIsReadable("InterfaceID"))
                    Console.WriteLine("Feature InterfaceID Is Readable.");
                else
                    Console.WriteLine("Feature InterfaceID Not Readable.");

                if (cam.IMV_FG_FeatureIsWriteable("InterfaceID"))
                    Console.WriteLine("Feature InterfaceID Is Writeable.");
                else
                    Console.WriteLine("Feature InterfaceID Not Writeable.");

                if (cam.IMV_FG_FeatureIsStreamable("InterfaceID"))
                    Console.WriteLine("Feature InterfaceID Is Streamable.");
                else
                    Console.WriteLine("Feature InterfaceID Not Streamable.");

                //获取属性类型
                //Get feature type
                IMVFGDefine.IMV_FG_EFeatureType valueType = IMVFGDefine.IMV_FG_EFeatureType.IMV_FG_FEATURE_UNDEFINED;
                cam.IMV_FG_GetFeatureType("InterfaceID", ref valueType);
                switch (valueType)
                {
                    case IMVFGDefine.IMV_FG_EFeatureType.IMV_FG_FEATURE_BOOL:
                        Console.WriteLine("Feature Type: featureBool.");
                        break;
                    case IMVFGDefine.IMV_FG_EFeatureType.IMV_FG_FEATURE_COMMAND:
                        Console.WriteLine("Feature Type: featureCommand.");
                        break;
                    case IMVFGDefine.IMV_FG_EFeatureType.IMV_FG_FEATURE_ENUM:
                        Console.WriteLine("Feature Type: featureEnum.");
                        break;
                    case IMVFGDefine.IMV_FG_EFeatureType.IMV_FG_FEATURE_FLOAT:
                        Console.WriteLine("Feature Type: featureFloat.");
                        break;
                    case IMVFGDefine.IMV_FG_EFeatureType.IMV_FG_FEATURE_GROUP:
                        Console.WriteLine("Feature Type: featureGroup.");
                        break;
                    case IMVFGDefine.IMV_FG_EFeatureType.IMV_FG_FEATURE_INT:
                        Console.WriteLine("Feature Type: featureInt.");
                        break;
                    case IMVFGDefine.IMV_FG_EFeatureType.IMV_FG_FEATURE_REG:
                        Console.WriteLine("Feature Type: featureReg.");
                        break;
                    case IMVFGDefine.IMV_FG_EFeatureType.IMV_FG_FEATURE_STRING:
                        Console.WriteLine("Feature Type: featureString.");
                        break;
                    default:
                        Console.WriteLine("Feature Type: featureUndefined.");
                        break;
                }

                IMVFGDefine.IMV_FG_String stringValue = new IMVFGDefine.IMV_FG_String();
                ;
                // 获取属性值
                // Get feature value 
                res = cam.IMV_FG_GetStringFeatureValue("InterfaceID", ref stringValue);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Get InterfaceID value failed! ErrorCode[{0}]", res);
                    break;
                }
                Console.WriteLine("interface id is {0}.", stringValue);
            } while (false);

            Console.WriteLine("end modifyCxpInterfaceProperty.");
            return res;
        }

        private static int modifyCxpDeviceProperty()
        {
            int res = IMVFGDefine.IMV_FG_OK;
            Console.WriteLine("begin modifyCxpDeviceProperty.");
            do
            {
                IMVFGDefine.IMV_FG_String stringValue = new IMVFGDefine.IMV_FG_String();
                // 获取属性值
                // Get feature value 
                res = cam.IMV_FG_GetStringFeatureValue("DeviceID", ref stringValue);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Get DeviceID value failed! ErrorCode[{0}]", res);
                    break;
                }
                Console.WriteLine("DeviceID is {0}.", stringValue);

                double doubleValue = 0.0;
                // 获取属性值
                // Get feature value 
                res = cam.IMV_FG_GetDoubleFeatureValue("DeviceExposureTime", ref doubleValue);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Get DeviceExposureTime value failed! ErrorCode[{0}]", res);
                    break;
                }

                Console.WriteLine("Before change ,DeviceExposureTime is {0}", doubleValue);

                long intValue = 0;
                // 获取属性值
                // Get feature value 
                res = cam.IMV_FG_GetIntFeatureValue("DevicePulseNumber", ref intValue);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Get DevicePulseNumber value failed! ErrorCode[{0}]", res);
                    break;
                }

                Console.WriteLine("Before change ,DevicePulseNumber is {0}", intValue);

                // 设置属性值
                // Set feature value 
                ulong enumValue = 0;
                IMVFGDefine.IMV_FG_String enumSymbolValue = new IMVFGDefine.IMV_FG_String();
                res = cam.IMV_FG_GetEnumFeatureValue("CardioInMask", ref enumValue);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Set CardioInMask value failed! ErrorCode[{0}]", res);
                    break;
                }
                Console.WriteLine("Before change ,CardioInMask is {0}", enumValue);
            } while (false);
            Console.WriteLine("end modifyCxpDeviceProperty.");
            return res;
        }

        #endregion

        #region 修改采集卡属性

        //int
        private static int modifyCardDevWidth()
        {
            int res = IMVFGDefine.IMV_FG_OK;
            Int64 widthValue = 0;
            Int64 widthMinValue = 0;
            Int64 widthMaxValue = 0;
            Int64 incrementValue = 0;

            // 获取属性值
            // Get feature value 
            res = card.IMV_FG_GetIntFeatureValue("Width", ref widthValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,Width is {0}", widthValue);

            // 获取属性可设的最小值
            // Get property's settable minimum value
            res = card.IMV_FG_GetIntFeatureMin("Width", ref widthMinValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get Width minimum value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Width settable minimum value is {0}", widthMinValue);

            // 获取属性可设的最大值
            // Get property's settable maximum value
            res = card.IMV_FG_GetIntFeatureMax("Width", ref widthMaxValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get Width maximum value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Width settable maximum value is {0}", widthMaxValue);

            // 获取属性步长
            // Get feature increment
            res = card.IMV_FG_GetIntFeatureInc("Width", ref incrementValue);
            if (res != IMVFGDefine.IMV_FG_OK)
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
            res = card.IMV_FG_SetIntFeatureValue("Width", widthValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set Width feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = card.IMV_FG_GetIntFeatureValue("Width", ref widthValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get Width feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            Console.WriteLine("After change ,Width is {0}", widthValue);
            return res;
        }

        //string
        private static int modifyCardDeviceUserID()
        {
            int res = IMVFGDefine.IMV_FG_OK;
            IMVFGDefine.IMV_FG_String stringValue = new IMVFGDefine.IMV_FG_String();

            // 获取属性值
            // Get feature value 
            res = card.IMV_FG_GetStringFeatureValue("DeviceUserID", ref stringValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,DeviceUserID is {0}", stringValue.str);

            // 设置属性值
            // Set feature value 
            res = card.IMV_FG_SetStringFeatureValue("DeviceUserID", "CamDev");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = card.IMV_FG_GetStringFeatureValue("DeviceUserID", ref stringValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("After change ,DeviceUserID is {0}", stringValue.str);
            return res;
        }

        //command
        private static int executeCardDevTriggerSoftware()
        {
            int res = IMVFGDefine.IMV_FG_OK;
            // 执行命令属性
            // Execute command property
            res = card.IMV_FG_ExecuteCommandFeature("TriggerSoftware");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Execute TriggerSoftware failed! ErrorCode[{0}]", res);
                return res;
            }

            Console.WriteLine("Execute TriggerSoftware success.");
            return res;
        }

        //bool
        private static int modifyCardDevReverseY()
        {
            int res = IMVFGDefine.IMV_FG_OK;
            bool reverseYValue = false;

            if (!cam.IMV_FG_FeatureIsValid("ReverseY") || !cam.IMV_FG_FeatureIsAvailable("ReverseY") ||
                !cam.IMV_FG_FeatureIsReadable("ReverseY") || !cam.IMV_FG_FeatureIsWriteable("ReverseY"))
            {
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = card.IMV_FG_GetBoolFeatureValue("ReverseY", ref reverseYValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            Console.WriteLine("Before change ,reverseY is {0}", reverseYValue);

            // 设置属性值
            // Set feature value 
            res = card.IMV_FG_SetBoolFeatureValue("ReverseY", !reverseYValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 获取属性值
            // Get feature value 
            res = card.IMV_FG_GetBoolFeatureValue("ReverseY", ref reverseYValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            Console.WriteLine("After change ,reverseY is {0}", reverseYValue);
            return res;
        }

        //enum
        private static int modifyCardDevPixelFormat()
        {
            UInt64 enumValue = 0;

            // 获取属性值
            // Get feature value 
            int res = card.IMV_FG_GetEnumFeatureValue("PixelFormat", ref enumValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 设置属性值
            // Set feature value 
            res = card.IMV_FG_SetEnumFeatureValue("PixelFormat", enumValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            return res;
        }

        //double
        private static int modifyCardDevStrobeSignalWidth()
        {
            double StrobeSignalWidthMinValue = 0.0;
            int res = IMVFGDefine.IMV_FG_OK;

            if (!cam.IMV_FG_FeatureIsValid("StrobeSignal1Width") || !cam.IMV_FG_FeatureIsAvailable("StrobeSignal1Width") ||
                !cam.IMV_FG_FeatureIsReadable("StrobeSignal1Width") ||
                !cam.IMV_FG_FeatureIsWriteable("StrobeSignal1Width"))
            {
                return res;
            }

            // 获取属性可设的最小值
            // Get property's settable minimum value
            res = card.IMV_FG_GetDoubleFeatureMin("StrobeSignal1Width", ref StrobeSignalWidthMinValue);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get feature minimum value failed! ErrorCode[{0}]", res);
                return res;
            }

            // 设置属性值
            // Set feature value 
            res = card.IMV_FG_SetDoubleFeatureValue("StrobeSignal1Width", StrobeSignalWidthMinValue + 1.0);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set feature value failed! ErrorCode[{0}]", res);
                return res;
            }
            return res;
        }

        #endregion

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
