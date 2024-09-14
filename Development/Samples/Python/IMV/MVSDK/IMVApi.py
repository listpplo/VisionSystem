# -- coding: utf-8 --

import sys
import ctypes
import platform

sys.path.append("Development\Samples\Python\IMV\MVSDK")
from IMVDefines import *


# 加载SDK动态库
# load SDK library
if sys.platform == 'win32':
    print("Windows")
    bits, linkage = platform.architecture()
    if bits == '64bit':
        MVSDKdll = WinDLL("Runtime/x64/MVSDKmd.dll")
    else:
        MVSDKdll = WinDLL("../../../../../Runtime/Win32/MVSDKmd.dll")
else:
    print("Linux")
    MVSDKdll = CDLL("../../../lib/libMVSDK.so")


class MvCamera():

    def __init__(self):
        self._handle = c_void_p()  # 记录当前连接设备的句柄
        self.handle = pointer(self._handle)  # 创建句柄指针

    @staticmethod
    # ch:获取SDK版本号 | en:Get SDK Version
    def IMV_GetVersion():
        MVSDKdll.IMV_GetVersion.restype = c_char_p
        # C原型：const char* IMV_CALL IMV_GetVersion(void);
        return MVSDKdll.IMV_GetVersion()

    @staticmethod
    def IMV_EnumDevices(pDeviceList, interfaceType):
        MVSDKdll.IMV_EnumDevices.argtype = (c_void_p, c_uint)
        MVSDKdll.IMV_EnumDevices.restype = c_int
        # C原型:int IMV_CALL IMV_EnumDevices(OUT IMV_DeviceList *pDeviceList, IN unsigned int interfaceType);
        return MVSDKdll.IMV_EnumDevices(byref(pDeviceList), c_uint(interfaceType))

    @staticmethod
    def IMV_EnumDevicesByUnicast(pDeviceList, pIpAddress):
        MVSDKdll.IMV_EnumDevicesByUnicast.argtype = (c_void_p, c_char_p)
        MVSDKdll.IMV_EnumDevicesByUnicast.restype = c_int
        # C原型:int IMV_CALL IMV_EnumDevicesByUnicast(OUT IMV_DeviceList *pDeviceList, IN const char* pIpAddress);
        return MVSDKdll.IMV_EnumDevicesByUnicast(byref(pDeviceList), pIpAddress.encode('ascii'))

    # ch:创建设备句柄
    def IMV_CreateHandle(self, mode, pIdentifier):
        MVSDKdll.IMV_CreateHandle.argtype = (c_void_p, c_int, c_void_p)
        MVSDKdll.IMV_CreateHandle.restype = c_int
        # C原型:int IMV_CALL IMV_CreateHandle(OUT IMV_HANDLE* handle, IN IMV_ECreateHandleMode mode, IN void* pIdentifier);
        return MVSDKdll.IMV_CreateHandle(byref(self.handle), c_int(mode), pIdentifier)

    # ch:销毁设备句柄
    def IMV_DestroyHandle(self):
        MVSDKdll.IMV_DestroyHandle.argtype = c_void_p
        MVSDKdll.IMV_DestroyHandle.restype = c_int
        # C原型:int IMV_CALL IMV_DestroyHandle(IN IMV_HANDLE handle);
        return MVSDKdll.IMV_DestroyHandle(self.handle)

    # ch:获取设备信息
    def IMV_GetDeviceInfo(self, pDevInfo):
        MVSDKdll.IMV_GetDeviceInfo.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_GetDeviceInfo.restype = c_int
        # C原型:int IMV_CALL IMV_GetDeviceInfo(IN IMV_HANDLE handle, OUT IMV_DeviceInfo *pDevInfo);
        return MVSDKdll.IMV_GetDeviceInfo(self.handle, byref(pDevInfo))

    # ch:打开设备
    def IMV_Open(self):
        MVSDKdll.IMV_Open.argtype = c_void_p
        MVSDKdll.IMV_Open.restype = c_int
        # C原型:int IMV_CALL IMV_Open(IN IMV_HANDLE handle);
        return MVSDKdll.IMV_Open(self.handle)

    # ch:打开设备
    def IMV_OpenEx(self, accessPermission):
        MVSDKdll.IMV_OpenEx.argtype = (c_void_p, c_int)
        MVSDKdll.IMV_OpenEx.restype = c_int
        # C原型:int IMV_CALL IMV_OpenEx(IN IMV_HANDLE handle, IN IMV_ECameraAccessPermission accessPermission);
        return MVSDKdll.IMV_OpenEx(self.handle, c_int(accessPermission))

    # ch:判断设备是否已打开
    def IMV_IsOpen(self):
        MVSDKdll.IMV_IsOpen.argtype = c_void_p
        MVSDKdll.IMV_IsOpen.restype = c_bool
        # C原型:bool IMV_CALL IMV_IsOpen(IN IMV_HANDLE handle);
        return MVSDKdll.IMV_IsOpen(self.handle)

    # ch:关闭设备
    def IMV_Close(self):
        MVSDKdll.IMV_Close.argtype = c_void_p
        MVSDKdll.IMV_Close.restype = c_int
        # C原型:int IMV_CALL IMV_Close(IN IMV_HANDLE handle);
        return MVSDKdll.IMV_Close(self.handle)

    # ch:修改设备IP, 仅限Gige设备使用
    def IMV_GIGE_ForceIpAddress(self, pIpAddress, pSubnetMask, pGateway):
        MVSDKdll.IMV_GIGE_ForceIpAddress.argtype = (c_void_p, c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GIGE_ForceIpAddress.restype = c_int
        # C原型:int IMV_CALL IMV_GIGE_ForceIpAddress(IN IMV_HANDLE handle, IN const char* pIpAddress, IN const char* pSubnetMask, IN const char* pGateway);
        return MVSDKdll.IMV_GIGE_ForceIpAddress(self.handle, pIpAddress.encode('ascii'), pSubnetMask.encode('ascii'),
                                                pGateway.encode('ascii'))

    # ch:设置设备对sdk命令的响应超时时间,仅限Gige设备使用
    def IMV_GIGE_SetAnswerTimeout(self, timeout):
        MVSDKdll.IMV_GIGE_SetAnswerTimeout.argtype = (c_void_p, c_uint)
        MVSDKdll.IMV_GIGE_SetAnswerTimeout.restype = c_int
        # C原型:int IMV_CALL IMV_GIGE_SetAnswerTimeout(IN IMV_HANDLE handle, IN unsigned int timeout);
        return MVSDKdll.IMV_GIGE_SetAnswerTimeout(self.handle, c_uint(timeout))

    # ch:获取设备的当前访问权限, 仅限Gige设备使用
    def IMV_GIGE_GetAccessPermission(self, pAccessPermission):
        MVSDKdll.IMV_GIGE_SetAnswerTimeout.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_GIGE_SetAnswerTimeout.restype = c_int
        # C原型:int IMV_CALL IMV_GIGE_GetAccessPermission(IN IMV_HANDLE handle, OUT IMV_ECameraAccessPermission* pAccessPermission);
        return MVSDKdll.IMV_GIGE_GetAccessPermission(self.handle, byref(pAccessPermission))

    # ch:下载设备描述XML文件，并保存到指定路径，如：D:\\xml.zip
    def IMV_DownLoadGenICamXML(self, pFullFileName):
        MVSDKdll.IMV_DownLoadGenICamXML.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_DownLoadGenICamXML.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_DownLoadGenICamXML(IN IMV_HANDLE handle, IN const char* pFullFileName);
        return MVSDKdll.IMV_DownLoadGenICamXML(self.handle, pFullFileName.encode('ascii'))

    # ch:保存设备配置到指定的位置。同名文件已存在时，覆盖。
    def IMV_SaveDeviceCfg(self, pFullFileName):
        MVSDKdll.IMV_SaveDeviceCfg.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_SaveDeviceCfg.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SaveDeviceCfg(IN IMV_HANDLE handle, IN const char* pFullFileName);
        return MVSDKdll.IMV_SaveDeviceCfg(self.handle, pFullFileName.encode('ascii'))

    # ch:保存设备配置到指定的位置。同名文件已存在时，覆盖。
    def IMV_LoadDeviceCfg(self, pFullFileName, pErrorList):
        MVSDKdll.IMV_SaveDeviceCfg.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_SaveDeviceCfg.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_LoadDeviceCfg(IN IMV_HANDLE handle, IN const char* pFullFileName, OUT IMV_ErrorList* pErrorList);
        return MVSDKdll.IMV_LoadDeviceCfg(self.handle, pFullFileName.encode('ascii'), byref(pErrorList))

    # ch:写用户自定义数据。相机内部保留32768字节用于用户存储自定义数据(此功能针对本品牌相机，其它品牌相机无此功能)
    def IMV_WriteUserPrivateData(self, pBuffer, pLength):
        MVSDKdll.IMV_WriteUserPrivateData.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_WriteUserPrivateData.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_WriteUserPrivateData(IN IMV_HANDLE handle, IN void* pBuffer, IN_OUT unsigned int* pLength);
        return MVSDKdll.IMV_WriteUserPrivateData(self.handle, byref(pBuffer), byref(pLength))

    # ch:读用户自定义数据。相机内部保留32768字节用于用户存储自定义数据(此功能针对本品牌相机，其它品牌相机无此功能)
    def IMV_ReadUserPrivateData(self, pBuffer, pLength):
        MVSDKdll.IMV_ReadUserPrivateData.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_ReadUserPrivateData.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_ReadUserPrivateData(IN IMV_HANDLE handle, OUT void* pBuffer, IN_OUT unsigned int* pLength);
        return MVSDKdll.IMV_ReadUserPrivateData(self.handle, byref(pBuffer), byref(pLength))

    # ch:往相机串口寄存器写数据，每次写会清除掉上次的数据(此功能只支持包含串口功能的本品牌相机)
    def IMV_WriteUARTData(self, pBuffer, pLength):
        MVSDKdll.IMV_WriteUARTData.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_WriteUARTData.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_WriteUARTData(IN IMV_HANDLE handle, IN void* pBuffer, IN_OUT unsigned int* pLength);
        return MVSDKdll.IMV_WriteUARTData(self.handle, byref(pBuffer), byref(pLength))

    # ch:从相机串口寄存器读取串口数据(此功能只支持包含串口功能的本品牌相机)
    def IMV_ReadUARTData(self, pBuffer, pLength):
        MVSDKdll.IMV_ReadUARTData.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_ReadUARTData.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_ReadUARTData(IN IMV_HANDLE handle, IN void* pBuffer, IN_OUT unsigned int* pLength);
        return MVSDKdll.IMV_ReadUARTData(self.handle, byref(pBuffer), byref(pLength))

    # ch:设备连接状态事件回调注册
    def IMV_SubscribeConnectArg(self, proc, pUser):
        MVSDKdll.IMV_SubscribeConnectArg.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_SubscribeConnectArg.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SubscribeConnectArg(IN IMV_HANDLE handle, IN IMV_ConnectCallBack proc, IN void* pUser);
        return MVSDKdll.IMV_SubscribeConnectArg(self.handle, proc, pUser)

    # ch:参数更新事件回调注册
    def IMV_SubscribeParamUpdateArg(self, proc, pUser):
        MVSDKdll.IMV_SubscribeParamUpdateArg.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_SubscribeParamUpdateArg.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SubscribeParamUpdateArg(IN IMV_HANDLE handle, IN IMV_ParamUpdateCallBack proc, IN void* pUser);
        return MVSDKdll.IMV_SubscribeParamUpdateArg(self.handle, proc, pUser)

    # ch:流通道事件回调注册
    def IMV_SubscribeStreamArg(self, proc, pUser):
        MVSDKdll.IMV_SubscribeStreamArg.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_SubscribeStreamArg.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SubscribeStreamArg(IN IMV_HANDLE handle, IN IMV_StreamCallBack proc, IN void* pUser);
        return MVSDKdll.IMV_SubscribeStreamArg(self.handle, proc, pUser)

    # ch:消息通道事件回调注册
    def IMV_SubscribeMsgChannelArg(self, proc, pUser):
        MVSDKdll.IMV_SubscribeMsgChannelArg.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_SubscribeMsgChannelArg.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SubscribeMsgChannelArg(IN IMV_HANDLE handle, IN IMV_MsgChannelCallBack proc, IN void* pUser);
        return MVSDKdll.IMV_SubscribeMsgChannelArg(self.handle, proc, pUser)

    # ch:设置帧数据缓存个数
    def IMV_SetBufferCount(self, nSize):
        MVSDKdll.IMV_SetBufferCount.argtype = (c_void_p, c_uint)
        MVSDKdll.IMV_SetBufferCount.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SetBufferCount(IN IMV_HANDLE handle, IN unsigned int nSize);
        return MVSDKdll.IMV_SetBufferCount(self.handle, c_uint(nSize))

    # ch:清除帧数据缓存个数
    def IMV_ClearFrameBuffer(self):
        MVSDKdll.IMV_ClearFrameBuffer.argtype = c_void_p
        MVSDKdll.IMV_ClearFrameBuffer.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_ClearFrameBuffer(IN IMV_HANDLE handle);
        return MVSDKdll.IMV_ClearFrameBuffer(self.handle)

    # ch:设置驱动包间隔时间(MS),仅对Gige设备有效
    def IMV_GIGE_SetInterPacketTimeout(self, nTimeout):
        MVSDKdll.IMV_GIGE_SetInterPacketTimeout.argtype = (c_void_p, c_uint)
        MVSDKdll.IMV_GIGE_SetInterPacketTimeout.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GIGE_SetInterPacketTimeout(IN IMV_HANDLE handle, IN unsigned int nTimeout);
        return MVSDKdll.IMV_GIGE_SetInterPacketTimeout(self.handle, c_uint(nTimeout))

    # ch:设置单次重传最大包个数, 仅对GigE设备有效
    def IMV_GIGE_SetSingleResendMaxPacketNum(self, maxPacketNum):
        MVSDKdll.IMV_GIGE_SetSingleResendMaxPacketNum.argtype = (c_void_p, c_uint)
        MVSDKdll.IMV_GIGE_SetSingleResendMaxPacketNum.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GIGE_SetSingleResendMaxPacketNum(IN IMV_HANDLE handle, IN unsigned int maxPacketNum);
        return MVSDKdll.IMV_GIGE_SetSingleResendMaxPacketNum(self.handle, c_uint(maxPacketNum))

    # ch:设置同一帧最大丢包的数量,仅对GigE设备有效
    def IMV_GIGE_SetMaxLostPacketNum(self, maxLostPacketNum):
        MVSDKdll.IMV_GIGE_SetMaxLostPacketNum.argtype = (c_void_p, c_uint)
        MVSDKdll.IMV_GIGE_SetMaxLostPacketNum.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GIGE_SetMaxLostPacketNum(IN IMV_HANDLE handle, IN unsigned int maxLostPacketNum);
        return MVSDKdll.IMV_GIGE_SetMaxLostPacketNum(self.handle, c_uint(maxLostPacketNum))

    # ch:设置U3V设备的传输数据块的数量和大小,仅对USB设备有效
    def IMV_USB_SetUrbTransfer(self, nNum, nSize):
        MVSDKdll.IMV_USB_SetUrbTransfer.argtype = (c_void_p, c_uint, c_uint)
        MVSDKdll.IMV_USB_SetUrbTransfer.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_USB_SetUrbTransfer(IN IMV_HANDLE handle, IN unsigned int nNum, IN unsigned int nSize);
        return MVSDKdll.IMV_USB_SetUrbTransfer(self.handle, c_uint(nNum), c_uint(nSize))

    # ch:开始取流
    def IMV_StartGrabbing(self):
        MVSDKdll.IMV_StartGrabbing.argtype = c_void_p
        MVSDKdll.IMV_StartGrabbing.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_StartGrabbing(IN IMV_HANDLE handle);
        return MVSDKdll.IMV_StartGrabbing(self.handle)

    # ch:开始取流
    def IMV_StartGrabbingEx(self, maxImagesGrabbed, strategy):
        MVSDKdll.IMV_StartGrabbingEx.argtype = (c_void_p, c_uint64, c_uint)
        MVSDKdll.IMV_StartGrabbingEx.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_StartGrabbingEx(IN IMV_HANDLE handle, IN uint64_t maxImagesGrabbed, IN IMV_EGrabStrategy strategy);
        return MVSDKdll.IMV_StartGrabbingEx(self.handle, c_uint64(maxImagesGrabbed), c_uint(strategy))

    # ch:判断设备是否正在取流
    def IMV_IsGrabbing(self):
        MVSDKdll.IMV_IsGrabbing.argtype = c_void_p
        MVSDKdll.IMV_IsGrabbing.restype = c_bool
        # C原型:IMV_API int IMV_CALL IMV_StartGrabbing(IN IMV_HANDLE handle);
        return MVSDKdll.IMV_IsGrabbing(self.handle)

    # ch:停止取流
    def IMV_StopGrabbing(self):
        MVSDKdll.IMV_StopGrabbing.argtype = c_void_p
        MVSDKdll.IMV_StopGrabbing.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_StopGrabbing(IN IMV_HANDLE handle);
        return MVSDKdll.IMV_StopGrabbing(self.handle)

    # ch:注册帧数据回调函数(异步获取帧数据机制)
    def IMV_AttachGrabbing(self, proc, pUser):
        MVSDKdll.IMV_AttachGrabbing.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_AttachGrabbing.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_AttachGrabbing(IN IMV_HANDLE handle, IN IMV_FrameCallBack proc, IN void* pUser);
        return MVSDKdll.IMV_AttachGrabbing(self.handle, proc, pUser)

    # ch:获取一帧图像(同步获取帧数据机制)
    def IMV_GetFrame(self, pFrame, timeoutMS):
        MVSDKdll.IMV_GetFrame.argtype = (c_void_p, c_void_p, c_uint)
        MVSDKdll.IMV_GetFrame.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetFrame(IN IMV_HANDLE handle, OUT IMV_Frame* pFrame, IN unsigned int timeoutMS);
        return MVSDKdll.IMV_GetFrame(self.handle, byref(pFrame), c_uint(timeoutMS))

    # ch:释放图像缓存
    def IMV_ReleaseFrame(self, pFrame):
        MVSDKdll.IMV_ReleaseFrame.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_ReleaseFrame.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_ReleaseFrame(IN IMV_HANDLE handle, IN IMV_Frame* pFrame);
        return MVSDKdll.IMV_ReleaseFrame(self.handle, byref(pFrame))

    # ch:释放图像缓存
    def IMV_CloneFrame(self, pFrame, pCloneFrame):
        MVSDKdll.IMV_CloneFrame.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_CloneFrame.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_CloneFrame(IN IMV_HANDLE handle, IN IMV_Frame* pFrame, OUT IMV_Frame* pCloneFrame);
        return MVSDKdll.IMV_CloneFrame(self.handle, byref(pFrame), byref(pCloneFrame))

    # ch:获取Chunk数据(仅对GigE/Usb相机有效)
    def IMV_GetChunkDataByIndex(self, pFrame, index, pChunkDataInfo):
        MVSDKdll.IMV_GetChunkDataByIndex.argtype = (c_void_p, c_void_p, c_uint, c_void_p)
        MVSDKdll.IMV_GetChunkDataByIndex.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetChunkDataByIndex(IN IMV_HANDLE handle, IN IMV_Frame* pFrame, IN unsigned int index, OUT IMV_ChunkDataInfo *pChunkDataInfo);
        return MVSDKdll.IMV_GetChunkDataByIndex(self.handle, byref(pFrame), c_uint(index), byref(pChunkDataInfo))

    # ch:获取流统计信息(IMV_StartGrabbing / IMV_StartGrabbing执行后调用)
    def IMV_GetStatisticsInfo(self, pStreamStatsInfo):
        MVSDKdll.IMV_GetStatisticsInfo.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_GetStatisticsInfo.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetStatisticsInfo(IN IMV_HANDLE handle, OUT IMV_StreamStatisticsInfo* pStreamStatsInfo);
        return MVSDKdll.IMV_GetStatisticsInfo(self.handle, byref(pStreamStatsInfo))

    # ch:重置流统计信息(IMV_StartGrabbing / IMV_StartGrabbing执行后调用)
    def IMV_ResetStatisticsInfo(self):
        MVSDKdll.IMV_ResetStatisticsInfo.argtype = c_void_p
        MVSDKdll.IMV_ResetStatisticsInfo.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_ResetStatisticsInfo(IN IMV_HANDLE handle);
        return MVSDKdll.IMV_ResetStatisticsInfo(self.handle)

    # ch:判断属性是否可用
    def IMV_FeatureIsAvailable(self, pFeatureName):
        MVSDKdll.IMV_FeatureIsAvailable.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_FeatureIsAvailable.restype = c_bool
        # C原型:IMV_API bool IMV_CALL IMV_FeatureIsAvailable(IN IMV_HANDLE handle, IN const char* pFeatureName);
        return MVSDKdll.IMV_FeatureIsAvailable(self.handle, pFeatureName.encode('ascii'))

    # ch:判断属性是否可读
    def IMV_FeatureIsReadable(self, pFeatureName):
        MVSDKdll.IMV_FeatureIsReadable.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_FeatureIsReadable.restype = c_bool
        # C原型:IMV_API bool IMV_CALL IMV_FeatureIsReadable(IN IMV_HANDLE handle, IN const char* pFeatureName);
        return MVSDKdll.IMV_FeatureIsReadable(self.handle, pFeatureName.encode('ascii'))

    # ch:判断属性是否可写
    def IMV_FeatureIsWriteable(self, pFeatureName):
        MVSDKdll.IMV_FeatureIsWriteable.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_FeatureIsWriteable.restype = c_bool
        # C原型:IMV_API bool IMV_CALL IMV_FeatureIsWriteable(IN IMV_HANDLE handle, IN const char* pFeatureName);
        return MVSDKdll.IMV_FeatureIsWriteable(self.handle, pFeatureName.encode('ascii'))

    # ch:判断属性是否可流
    def IMV_FeatureIsStreamable(self, pFeatureName):
        MVSDKdll.IMV_FeatureIsStreamable.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_FeatureIsStreamable.restype = c_bool
        # C原型:IMV_API bool IMV_CALL IMV_FeatureIsStreamable(IN IMV_HANDLE handle, IN const char* pFeatureName);
        return MVSDKdll.IMV_FeatureIsStreamable(self.handle, pFeatureName.encode('ascii'))

    # ch:判断属性是否有效
    def IMV_FeatureIsValid(self, pFeatureName):
        MVSDKdll.IMV_FeatureIsValid.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_FeatureIsValid.restype = c_bool
        # C原型:IMV_API bool IMV_CALL IMV_FeatureIsValid(IN IMV_HANDLE handle, IN const char* pFeatureName);
        return MVSDKdll.IMV_FeatureIsValid(self.handle, pFeatureName.encode('ascii'))

    # ch:获取属性类型
    def IMV_GetFeatureType(self, pFeatureName, pPropertyType):
        MVSDKdll.IMV_GetFeatureType.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetFeatureType.restype = c_bool
        # C原型:IMV_API bool IMV_CALL IMV_GetFeatureType(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT IMV_EFeatureType* pPropertyType);
        return MVSDKdll.IMV_GetFeatureType(self.handle, pFeatureName.encode('ascii'), byref(pPropertyType))

    # ch:获取整型属性值
    def IMV_GetIntFeatureValue(self, pFeatureName, pIntValue):
        MVSDKdll.IMV_GetIntFeatureValue.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetIntFeatureValue.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetIntFeatureValue(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);
        return MVSDKdll.IMV_GetIntFeatureValue(self.handle, pFeatureName.encode('ascii'), byref(pIntValue))

    # ch:获取整型属性可设的最小值
    def IMV_GetIntFeatureMin(self, pFeatureName, pIntValue):
        MVSDKdll.IMV_GetIntFeatureMin.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetIntFeatureMin.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetIntFeatureMin(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);
        return MVSDKdll.IMV_GetIntFeatureMin(self.handle, pFeatureName.encode('ascii'), byref(pIntValue))

    # ch:获取整型属性可设的最大值
    def IMV_GetIntFeatureMax(self, pFeatureName, pIntValue):
        MVSDKdll.IMV_GetIntFeatureMax.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetIntFeatureMax.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetIntFeatureMax(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);
        return MVSDKdll.IMV_GetIntFeatureMax(self.handle, pFeatureName.encode('ascii'), byref(pIntValue))

    # ch:获取整型属性步长
    def IMV_GetIntFeatureInc(self, pFeatureName, pIntValue):
        MVSDKdll.IMV_GetIntFeatureInc.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetIntFeatureInc.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetIntFeatureInc(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);
        return MVSDKdll.IMV_GetIntFeatureInc(self.handle, pFeatureName.encode('ascii'), byref(pIntValue))

    # ch:设置整型属性值
    def IMV_SetIntFeatureValue(self, pFeatureName, pIntValue):
        MVSDKdll.IMV_SetIntFeatureValue.argtype = (c_void_p, c_void_p, c_int)
        MVSDKdll.IMV_SetIntFeatureValue.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SetIntFeatureValue(IN IMV_HANDLE handle, IN const char* pFeatureName, IN int64_t intValue);
        return MVSDKdll.IMV_SetIntFeatureValue(self.handle, pFeatureName.encode('ascii'), c_int64(pIntValue))

    # ch:获取浮点属性值
    def IMV_GetDoubleFeatureValue(self, pFeatureName, pDoubleValue):
        MVSDKdll.IMV_GetDoubleFeatureValue.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetDoubleFeatureValue.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetDoubleFeatureValue(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT double* pDoubleValue);
        return MVSDKdll.IMV_GetDoubleFeatureValue(self.handle, pFeatureName.encode('ascii'), byref(pDoubleValue))

    # ch:获取浮点属性可设的最小值
    def IMV_GetDoubleFeatureMin(self, pFeatureName, pDoubleValue):
        MVSDKdll.IMV_GetDoubleFeatureMin.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetDoubleFeatureMin.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetDoubleFeatureMin(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT double* pDoubleValue);
        return MVSDKdll.IMV_GetDoubleFeatureMin(self.handle, pFeatureName.encode('ascii'), byref(pDoubleValue))

    # ch:获取浮点属性可设的最大值
    def IMV_GetDoubleFeatureMax(self, pFeatureName, pDoubleValue):
        MVSDKdll.IMV_GetDoubleFeatureMax.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetDoubleFeatureMax.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetDoubleFeatureMax(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT double* pDoubleValue);
        return MVSDKdll.IMV_GetDoubleFeatureMax(self.handle, pFeatureName.encode('ascii'), byref(pDoubleValue))

    # ch:设置浮点属性值
    def IMV_SetDoubleFeatureValue(self, pFeatureName, doubleValue):
        MVSDKdll.IMV_SetDoubleFeatureValue.argtype = (c_void_p, c_void_p, c_double)
        MVSDKdll.IMV_SetDoubleFeatureValue.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SetDoubleFeatureValue(IN IMV_HANDLE handle, IN const char* pFeatureName, IN double doubleValue);
        return MVSDKdll.IMV_SetDoubleFeatureValue(self.handle, pFeatureName.encode('ascii'), c_double(doubleValue))

    # ch:获取布尔属性值
    def IMV_GetBoolFeatureValue(self, pFeatureName, pBoolValue):
        MVSDKdll.IMV_GetBoolFeatureValue.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetBoolFeatureValue.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetBoolFeatureValue(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT bool* pBoolValue);
        return MVSDKdll.IMV_GetBoolFeatureValue(self.handle, pFeatureName.encode('ascii'), byref(pBoolValue))

    # ch:设置布尔属性值
    def IMV_SetBoolFeatureValue(self, pFeatureName, boolValue):
        MVSDKdll.IMV_SetBoolFeatureValue.argtype = (c_void_p, c_void_p, c_bool)
        MVSDKdll.IMV_SetBoolFeatureValue.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SetBoolFeatureValue(IN IMV_HANDLE handle, IN const char* pFeatureName, IN bool boolValue);
        return MVSDKdll.IMV_SetBoolFeatureValue(self.handle, pFeatureName.encode('ascii'), c_bool(boolValue))

    # ch:获取枚举属性值
    def IMV_GetEnumFeatureValue(self, pFeatureName, pEnumValue):
        MVSDKdll.IMV_GetEnumFeatureValue.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetEnumFeatureValue.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetEnumFeatureValue(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT uint64_t* pEnumValue);
        return MVSDKdll.IMV_GetEnumFeatureValue(self.handle, pFeatureName.encode('ascii'), byref(pEnumValue))

    # ch:设置枚举属性值
    def IMV_SetEnumFeatureValue(self, pFeatureName, enumValue):
        MVSDKdll.IMV_SetEnumFeatureValue.argtype = (c_void_p, c_void_p, c_uint64)
        MVSDKdll.IMV_SetEnumFeatureValue.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SetEnumFeatureValue(IN IMV_HANDLE handle, IN const char* pFeatureName, IN uint64_t enumValue);
        return MVSDKdll.IMV_SetEnumFeatureValue(self.handle, pFeatureName.encode('ascii'), c_int64(enumValue))

    # ch:获取枚举属性symbol值
    def IMV_GetEnumFeatureSymbol(self, pFeatureName, pEnumSymbol):
        MVSDKdll.IMV_GetEnumFeatureSymbol.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetEnumFeatureSymbol.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetEnumFeatureSymbol(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT IMV_String* pEnumSymbol);
        return MVSDKdll.IMV_GetEnumFeatureSymbol(self.handle, pFeatureName.encode('ascii'), byref(pEnumSymbol))

    # ch:设置枚举属性symbol值
    def IMV_SetEnumFeatureSymbol(self, pFeatureName, pEnumSymbol):
        MVSDKdll.IMV_SetEnumFeatureSymbol.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_SetEnumFeatureSymbol.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SetEnumFeatureSymbol(IN IMV_HANDLE handle, IN const char* pFeatureName, IN const char* pEnumSymbol);
        return MVSDKdll.IMV_SetEnumFeatureSymbol(self.handle, pFeatureName.encode('ascii'), pEnumSymbol.encode('ascii'))

    # ch:获取枚举属性的可设枚举值的个数
    def IMV_GetEnumFeatureEntryNum(self, pFeatureName, pEntryNum):
        MVSDKdll.IMV_GetEnumFeatureEntryNum.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetEnumFeatureEntryNum.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetEnumFeatureEntryNum(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT unsigned int* pEntryNum);
        return MVSDKdll.IMV_GetEnumFeatureEntryNum(self.handle, pFeatureName.encode('ascii'), byref(pEntryNum))

    # ch:获取枚举属性的可设枚举值列表
    def IMV_GetEnumFeatureEntrys(self, pFeatureName, pEnumEntryList):
        MVSDKdll.IMV_GetEnumFeatureEntrys.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetEnumFeatureEntrys.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetEnumFeatureEntrys(IN IMV_HANDLE handle, IN const char* pFeatureName, IN_OUT IMV_EnumEntryList* pEnumEntryList);
        return MVSDKdll.IMV_GetEnumFeatureEntrys(self.handle, pFeatureName.encode('ascii'), byref(pEnumEntryList))

    # ch:获取字符串属性值
    def IMV_GetStringFeatureValue(self, pFeatureName, pStringValue):
        MVSDKdll.IMV_GetStringFeatureValue.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_GetStringFeatureValue.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_GetStringFeatureValue(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT IMV_String* pStringValue);
        return MVSDKdll.IMV_GetStringFeatureValue(self.handle, pFeatureName.encode('ascii'), byref(pStringValue))

    # ch:设置字符串属性值
    def IMV_SetStringFeatureValue(self, pFeatureName, pStringValue):
        MVSDKdll.IMV_SetStringFeatureValue.argtype = (c_void_p, c_void_p, c_void_p)
        MVSDKdll.IMV_SetStringFeatureValue.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_SetStringFeatureValue(IN IMV_HANDLE handle, IN const char* pFeatureName, IN const char* pStringValue);
        return MVSDKdll.IMV_SetStringFeatureValue(self.handle, pFeatureName.encode('ascii'),
                                                  pStringValue.encode('ascii'))

    # ch:执行命令属性
    def IMV_ExecuteCommandFeature(self, pFeatureName):
        MVSDKdll.IMV_ExecuteCommandFeature.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_ExecuteCommandFeature.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_ExecuteCommandFeature(IN IMV_HANDLE handle, IN const char* pFeatureName);
        return MVSDKdll.IMV_ExecuteCommandFeature(self.handle, pFeatureName.encode('ascii'))

    # ch:像素格式转换
    def IMV_PixelConvert(self, pstPixelConvertParam):
        MVSDKdll.IMV_PixelConvert.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_PixelConvert.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_PixelConvert(IN IMV_HANDLE handle, IN_OUT IMV_PixelConvertParam* pstPixelConvertParam);
        return MVSDKdll.IMV_PixelConvert(self.handle, byref(pstPixelConvertParam))

    # ch:图像翻转
    def IMV_FlipImage(self, pstFlipImageParam):
        MVSDKdll.IMV_FlipImage.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_FlipImage.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_FlipImage(IN IMV_HANDLE handle, IN_OUT IMV_FlipImageParam* pstFlipImageParam);
        return MVSDKdll.IMV_FlipImage(self.handle, byref(pstFlipImageParam))

    # ch:图像顺时针旋转
    def IMV_RotateImage(self, pstRotateImageParam):
        MVSDKdll.IMV_RotateImage.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_RotateImage.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_RotateImage(IN IMV_HANDLE handle, IN_OUT IMV_RotateImageParam* pstRotateImageParam);
        return MVSDKdll.IMV_RotateImage(self.handle, byref(pstRotateImageParam))


    # ch:图像顺时针旋转
    def IMV_InternalWriteReg(self, regAddress, regValue, pLength):
        MVSDKdll.IMV_InternalWriteReg.argtype = (c_void_p, c_uint64, c_uint64, c_void_p)
        MVSDKdll.IMV_InternalWriteReg.restype = c_int
        # C原型:IMV_API int IMV_CALL IMV_InternalWriteReg(IN IMV_HANDLE handle, IN uint64_t regAddress, IN uint64_t regValue, IN_OUT unsigned int* pLength);
        return MVSDKdll.IMV_InternalWriteReg(self.handle, regAddress, regValue, byref(pLength))

    # ch:保存图像
    def IMV_SaveImageToFile(self, pstSaveFileParam):
        MVSDKdll.IMV_SaveImageToFile.argtype = (c_void_p, c_void_p)
        MVSDKdll.IMV_SaveImageToFile.restype = c_int
        #IMV_API int IMV_CALL IMV_SaveImageToFile(IN IMV_HANDLE handle, IN IMV_SaveImageToFileParam * pstSaveFileParam);
        return MVSDKdll.IMV_SaveImageToFile(self.handle, byref(pstSaveFileParam))
