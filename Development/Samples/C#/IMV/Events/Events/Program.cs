using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Permissions;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using MVSDK_Net;

namespace Events
{
    internal class Program
    {
        private static MyCamera cam = new MyCamera();
        private static IMVDefine.IMV_ParamUpdateCallBack paramUpdateCallBack;
        private static IMVDefine.IMV_FrameCallBack pFrameCallBack;
        private static IMVDefine.IMV_MsgChannelCallBack pMsgChannelCallBack;

        private static void Main(string[] args)
        {
            Console.WriteLine("SDK Version:{0}", MyCamera.IMV_GetVersion());
            Console.WriteLine("Enum camera device.");

            // 发现GigE相机
            // discover GigE camera 
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_DeviceList deviceList = new IMVDefine.IMV_DeviceList();
            IMVDefine.IMV_EInterfaceType interfaceTp = IMVDefine.IMV_EInterfaceType.interfaceTypeAll;
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

                // 打开相机
                // Open camera
                res = cam.IMV_Open();
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Open camera failed! ErrorCode:[{0}]", res);
                    break;
                }

                res = setMessageEventConf();
                if (res != IMVDefine.IMV_OK) break;

                // 消息事件回调注册
                // Register message channel event callback function
                pMsgChannelCallBack = onMessageChannelEvent;
                res = cam.IMV_SubscribeMsgChannelArg(pMsgChannelCallBack, IntPtr.Zero);
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Register message channel event failed! ErrorCode:[{0}]", res);
                    break;
                }

                // 参数更新回调注册
                // Register parameters update event callback function
                paramUpdateCallBack = onParameterUpdateEvent;
                res = cam.IMV_SubscribeParamUpdateArg(paramUpdateCallBack, IntPtr.Zero);
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Register parameters update event failed! ErrorCode:[{0}]", res);
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

        // 参数更新事件回调函数
        // Updating parameters event callback function
        private static void onParameterUpdateEvent(ref IMVDefine.IMV_SParamUpdateArg pParamUpdateArg, IntPtr pUser)
        {
            uint index = 0;

            Console.WriteLine("isPoll = {0}", pParamUpdateArg.isPoll);

            for (index = 0; index < pParamUpdateArg.nParamCnt; index++)
            {
                IMVDefine.IMV_String paramUpdateStr =
                    (IMVDefine.IMV_String)
                        Marshal.PtrToStructure(
                            pParamUpdateArg.pParamNameList + Marshal.SizeOf(typeof (IMVDefine.IMV_String))*(int) index,
                            typeof (IMVDefine.IMV_String));
                Console.WriteLine("paramName = {0}", paramUpdateStr.str);
            }
            Console.WriteLine();
        }

        // 消息通道事件回调函数
        // Message channel event callback function
        private static void onMessageChannelEvent(ref IMVDefine.IMV_SMsgChannelArg pMsgChannelArg, IntPtr pUser)
        {
            uint index = 0;
            string pEventIdStr = "";

            switch (pMsgChannelArg.eventId)
            {
                case IMVDefine.IMV_MSG_EVENT_ID_EXPOSURE_END:
                    pEventIdStr = "ExposureEnd";
                    break;
                case IMVDefine.IMV_MSG_EVENT_ID_FRAME_TRIGGER:
                    pEventIdStr = "FrameTrigger";
                    break;
                case IMVDefine.IMV_MSG_EVENT_ID_FRAME_START:
                    pEventIdStr = "FrameStart";
                    break;
                case IMVDefine.IMV_MSG_EVENT_ID_ACQ_START:
                    pEventIdStr = "AcquisitionStart";
                    break;
                case IMVDefine.IMV_MSG_EVENT_ID_ACQ_TRIGGER:
                    pEventIdStr = "AcquisitionTrigger";
                    break;
                case IMVDefine.IMV_MSG_EVENT_ID_DATA_READ_OUT:
                    pEventIdStr = "ReadOut";
                    break;
                default:
                    pEventIdStr = "Unknow";
                    break;
            }
            Console.WriteLine("eventId = [0x{0:x} : {1}]", pMsgChannelArg.eventId,pEventIdStr);
            Console.WriteLine("channelId = {0}", pMsgChannelArg.channelId);
            Console.WriteLine("blockId = {0}", pMsgChannelArg.blockId);
            Console.WriteLine("timestamp = {0}", pMsgChannelArg.timeStamp);

            for (index = 0; index < pMsgChannelArg.nParamCnt; index++)
            {
                IMVDefine.IMV_String messageChannelStr =
                    (IMVDefine.IMV_String)
                        Marshal.PtrToStructure(
                            pMsgChannelArg.pParamNameList + Marshal.SizeOf(typeof (IMVDefine.IMV_String))*(int) index,
                            typeof (IMVDefine.IMV_String));
                Console.WriteLine("paramName = {0}", messageChannelStr.str);
            }
            Console.WriteLine();
        }

        private static int setMessageEventConf()
        {
            int res = IMVDefine.IMV_OK;
            IMVDefine.IMV_DeviceInfo deviceInfo=new IMVDefine.IMV_DeviceInfo();

            // 获取设备信息
            // Get device information
            res = cam.IMV_GetDeviceInfo(ref deviceInfo);
            if (res != IMVDefine.IMV_OK)
            {
                Console.WriteLine("Get device info failed! ErrorCode[{0}]", res);
                return res;
            }

            // USB设备没有消息事件
            // USB device has nomessage event
            if (deviceInfo.nCameraType == IMVDefine.IMV_ECameraType.typeGigeCamera)
            {
                // 设置EventSelector
                // Set EventSelector
                res = cam.IMV_SetEnumFeatureSymbol("EventSelector", "FrameStart");
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Set EventSelector value failed! ErrorCode[{0}]", res);
                    return res;
                }

                // 设置EventNotification
                // Set EventNotification
                res = cam.IMV_SetEnumFeatureSymbol("EventNotification", "On");
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Set EventNotification value failed! ErrorCode[{0}]", res);
                    return res;
                }

                // 设置EventSelector
                // Set EventSelector
                res = cam.IMV_SetEnumFeatureSymbol("EventSelector", "ReadOut");
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Set EventSelector value failed! ErrorCode[{0}]", res);
                    return res;
                }

                // 设置EventNotification
                // Set EventNotification
                res = cam.IMV_SetEnumFeatureSymbol("EventNotification", "On");
                if (res != IMVDefine.IMV_OK)
                {
                    Console.WriteLine("Set EventNotification value failed! ErrorCode[{0}]", res);
                    return res;
                }
              
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
