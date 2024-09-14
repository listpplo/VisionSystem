using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using CaptureCard_Net;

namespace ConnectSpecCamera
{
    internal class Program
    {
        private static readonly int specifiedIndex = 0;
        private static readonly int specifiedDeviceUserID = 1;
        private static readonly int specifiedCameraKey = 2;
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
            IMVFGDefine.IMV_FG_EInterfaceType interfaceTp = IMVFGDefine.IMV_FG_EInterfaceType.typeCLInterface | IMVFGDefine.IMV_FG_EInterfaceType.typeCXPInterface;

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

            int inputIndex = 0;
            if (!specifiedDevice((int)interfaceList.nInterfaceNum, out inputIndex)) return;
            do
            {
                // 获取指定序号的采集卡信息
                IMVFGDefine.IMV_FG_INTERFACE_INFO interfaceinfo =
                   (IMVFGDefine.IMV_FG_INTERFACE_INFO)
                       Marshal.PtrToStructure(
                           interfaceList.pInterfaceInfoList +
                           Marshal.SizeOf(typeof(IMVFGDefine.IMV_FG_INTERFACE_INFO)) * (int)boardIndex,
                           typeof(IMVFGDefine.IMV_FG_INTERFACE_INFO));

                // 获取指定序号的相机信息
                IMVFGDefine.IMV_FG_DEVICE_INFO deviceinfo =
                        (IMVFGDefine.IMV_FG_DEVICE_INFO)
                            Marshal.PtrToStructure(
                                camListPtr.pDeviceInfoList +
                                Marshal.SizeOf(typeof(IMVFGDefine.IMV_FG_DEVICE_INFO)) * cameraIndex,
                                typeof(IMVFGDefine.IMV_FG_DEVICE_INFO));

                if (inputIndex == specifiedIndex)
                {
                    Console.WriteLine("Open device by index.");
                    res = card.IMV_FG_OpenInterfaceEx(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_INDEX, (int)boardIndex);
                    if (res != IMVFGDefine.IMV_FG_OK)
                    {
                        Console.WriteLine("Open capture device failed! ErrorCode[{0}]", res);
                        break;
                    }
                    res = cam.IMV_FG_OpenDevice(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_INDEX, cameraIndex);
                    if (res != IMVFGDefine.IMV_FG_OK)
                    {
                        Console.WriteLine("Open camera failed! ErrorCode[{0}]", res);
                        break;
                    }
                }
                else if (inputIndex == specifiedDeviceUserID)
                {
                    Console.WriteLine("Open device by deviceUserID.");
                    // 多个设备UserID不能相同，不然该方式无法打开指定设备
                    res = card.IMV_FG_OpenInterfaceEx(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_DEVICE_USERID, cameraStr: interfaceinfo.interfaceName);
                    if (res != IMVFGDefine.IMV_FG_OK)
                    {
                        Console.WriteLine("Open capture device failed! ErrorCode[{0}]", res);
                        break;
                    }
                    res = cam.IMV_FG_OpenDevice(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_DEVICE_USERID, cameraStr: deviceinfo.cameraName);
                    if (res != IMVFGDefine.IMV_FG_OK)
                    {
                        Console.WriteLine("Open camera failed! ErrorCode[{0}]", res);
                        break;
                    }
                }
                else if (inputIndex == specifiedCameraKey)
                {
                    Console.WriteLine("Open device by cameraKey.");
                    res = card.IMV_FG_OpenInterfaceEx(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_CAMERAKEY, cameraStr: interfaceinfo.interfaceKey);
                    if (res != IMVFGDefine.IMV_FG_OK)
                    {
                        Console.WriteLine("Open capture device failed! ErrorCode[{0}]", res);
                        break;
                    }
                    res = cam.IMV_FG_OpenDevice(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_CAMERAKEY, cameraStr: deviceinfo.cameraKey);
                    if (res != IMVFGDefine.IMV_FG_OK)
                    {
                        Console.WriteLine("Open camera failed! ErrorCode[{0}]", res);
                        break;
                    }
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
     
        private static bool specifiedDevice(int num, out int inputIndex)
        {
            Console.WriteLine("\n--------------------------------------------");
            Console.WriteLine("\t0.Specified Index");
            Console.WriteLine("\t1.Specified DeviceUserID");
            Console.WriteLine("\t2.Specified CameraKey");
            Console.WriteLine("--------------------------------------------");
            Console.WriteLine("\nPlease select the specified camera type: [0-2]");

            inputIndex = 0;
            try
            {
                inputIndex = Convert.ToInt32(Console.ReadLine());
            }
            catch
            {
                Console.WriteLine("Invalid Input!");
                Console.Read();
                return false;
            }
            if (inputIndex > 2 || inputIndex < 0)
            {
                Console.WriteLine("Input Error!");
                Console.Read();
                return false;
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
                            Marshal.SizeOf(typeof(IMVFGDefine.IMV_FG_INTERFACE_INFO)) * i,
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
				
                // 采集卡的型号信息  
                // CamDev model name 
                interfaceinfostr += interfaceinfo.modelName.PadLeft(interfaceinfo.modelName.Length + 9);

                // 采集卡的序列号 
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
                                Marshal.SizeOf(typeof(IMVFGDefine.IMV_FG_DEVICE_INFO)) * j,
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
