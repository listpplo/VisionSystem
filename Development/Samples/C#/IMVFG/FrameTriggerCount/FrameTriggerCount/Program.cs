using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using CaptureCard_Net;

namespace FrameTriggerCount
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

            Console.WriteLine("Enum capture board interface info.");
            //枚举采集卡设备
            // Discover capture board device
            int res = IMVFGDefine.IMV_FG_OK;
            IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST interfaceList = new IMVFGDefine.IMV_FG_INTERFACE_INFO_LIST();
            IMVFGDefine.IMV_FG_EInterfaceType interfaceTp = IMVFGDefine.IMV_FG_EInterfaceType.typeCLInterface;
            res = CardDev.IMV_FG_EnumInterface((uint) interfaceTp, ref interfaceList);
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
            res = CamDev.IMV_FG_EnumDevices((uint) interfaceTp, ref camListPtr);
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
            cameraIndex = (int) selectDevice(interfaceList.nInterfaceNum, camListPtr.nDevNum, CAMERA);

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
                // Connect to camera 
                res = cam.IMV_FG_OpenDevice(IMVFGDefine.IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_INDEX, cameraIndex);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Open camera failed!errorCode:[{0}]", res);
                    break;
                }
                setSoftTriggerConf();
                // 注册数据帧回调函数
                // Register data frame callback function
                res = card.IMV_FG_AttachGrabbing(onGetFrame, IntPtr.Zero);
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Attach grabbing failed! errorCode:[{0}]", res);
                    break;
                }

                // 开始拉流 
                // Start grabbing 
                res = card.IMV_FG_StartGrabbing();
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Start grabbing failed! errorCode:[{0}]", res);
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

                // 停止拉流 
                // Stop grabbing 
                res = card.IMV_FG_StopGrabbing();
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Stop grabbing failed! errorCode:[{0}]", res);
                    break;
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

        private static bool g_isExitThread = false;

        private static void executeSoftTriggerProc()
        {
            int res = 0;
            while (!g_isExitThread)
            {

                // 执行软触发
                // Execute soft trigger 
                res = cam.IMV_FG_ExecuteCommandFeature("TriggerSoftware");
                if (res != IMVFGDefine.IMV_FG_OK)
                {
                    Console.WriteLine("Execute TriggerSoftware failed! errorCode:[{0}]", res);
                    continue;
                }

                // 通过睡眠时间来调节帧率
                // Adjust the frame rate by sleep time
                Thread.Sleep(200);
            }

        }

        private static void onGetFrame(ref IMVFGDefine.IMV_FG_Frame frame, IntPtr pUser)
        {
            if (frame.frameHandle == IntPtr.Zero)
            {
                Console.WriteLine("frame is NULL");
                return;
            }

            // 获取帧触发数 
            // Get frame trigger count
            long frameTriggerCount = 0;
            int res = cam.IMV_FG_GetIntFeatureValue("FrameTriggerCount", ref frameTriggerCount);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get FrameTriggerCount value failed! errorCode:[{0}]", res);
                return;
            }

            // 获取帧触发丢失数 
            // Get frame trigger lost count
            long frameTriggerLostCount = 0;
            res = cam.IMV_FG_GetIntFeatureValue("FrameTriggerLostCount", ref frameTriggerLostCount);
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Get FrameTriggerLostCount value failed! errorCode:[{0}]", res);
                return;
            }
            Console.WriteLine("Frame blockId = {0}, FrameTriggerCount = {1}, FrameTriggerLostCount = {2}",
                frame.frameInfo.blockId,
                frameTriggerCount, frameTriggerLostCount);
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

            return (uint) inputIndex;
        }

        private static int setSoftTriggerConf()
        {
            int res = IMVFGDefine.IMV_FG_OK;

            // 设置触发源为软触发 
            // Set trigger source to Software 
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerSource", "Software");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set triggerSource value failed! errorCode:[{0}]", res);
                return res;
            }

            // 设置触发器 
            // Set trigger selector to FrameStart 
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerSelector", "FrameStart");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set triggerSelector value failed! errorCode:[{0}]", res);
                return res;
            }

            // 设置触发模式 
            // Set trigger mode to On 
            res = cam.IMV_FG_SetEnumFeatureSymbol("TriggerMode", "On");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Set triggerMode value failed! errorCode:[{0}]", res);
                return res;
            }

            // 触发计数清0 
            // Clear trigger count
            res = cam.IMV_FG_ExecuteCommandFeature("FrameTriggerCountReset");
            if (res != IMVFGDefine.IMV_FG_OK)
            {
                Console.WriteLine("Execute FrameTriggerCountReset value failed! errorCode:[{0}]", res);
                return res;
            }
            return res;
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
