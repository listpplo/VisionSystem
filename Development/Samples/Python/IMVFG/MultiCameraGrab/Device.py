# -- coding: utf-8 --

import sys
import time
import threading

sys.path.append("../MVSDK")
from IMVFGApi import *

def displayDeviceInfo(interfaceList,deviceInfoList):
    print("Idx  Type   Vendor              Model           S/N                 DeviceUserID    IP Address")
    print("------------------------------------------------------------------------------------------------")
    for index in range(0,interfaceList.nInterfaceNum):
        interfaceInfo = interfaceList.pInterfaceInfoList[index]
        strType = ""
        strVendorName = interfaceInfo.vendorName.decode("ascii")
        strModeName = interfaceInfo.modelName.decode("ascii")
        serialNumber = interfaceInfo.serialNumber.decode("ascii")
        interfaceName = interfaceInfo.interfaceName.decode("ascii")
        if interfaceInfo.nInterfaceType == IMV_FG_EInterfaceType.typeGigEInterface:
            strType = "Gige Card"
        elif interfaceInfo.nInterfaceType == IMV_FG_EInterfaceType.typeU3vInterface:
            strType = "U3V Card"
        elif interfaceInfo.nInterfaceType == IMV_FG_EInterfaceType.typeCLInterface:
            strType = "CL Card"
        elif interfaceInfo.nInterfaceType == IMV_FG_EInterfaceType.typeCXPInterface:
            strType = "CXP Card"
        print("[%d]  %s   %s    %s        %s            %s " % (
            index+1, strType, strVendorName, strModeName, serialNumber, interfaceName))

        for i in range(0, deviceInfoList.nDevNum):
            pDeviceInfo = deviceInfoList.pDeviceInfoList[i]
            if pDeviceInfo.FGInterfaceInfo.interfaceKey == interfaceInfo.interfaceKey:
                strType = ""
                strIpAdress = ""
                strVendorName = pDeviceInfo.vendorName.decode("ascii")
                strModeName = pDeviceInfo.modelName.decode("ascii")
                strSerialNumber = pDeviceInfo.serialNumber.decode("ascii")
                strCameraname = pDeviceInfo.cameraName.decode("ascii")
                if pDeviceInfo.nDeviceType == IMV_FG_EDeviceType.IMV_FG_TYPE_GIGE_DEVICE:
                    strType = "Gige"
                elif pDeviceInfo.nDeviceType == IMV_FG_EDeviceType.IMV_FG_TYPE_U3V_DEVICE:
                    strType = "U3V"
                elif pDeviceInfo.nDeviceType == IMV_FG_EDeviceType.IMV_FG_TYPE_CL_DEVICE:
                    strType = "CL"
                elif pDeviceInfo.nDeviceType == IMV_FG_EDeviceType.IMV_FG_TYPE_CXP_DEVICE:
                    strType = "CXP"
                print("  |-%d  %s   %s    %s      %s     %s           %s" % (
                 int(i)+1,  strType, strVendorName, strModeName, strSerialNumber, strCameraname, strIpAdress))
    return

class CameraDevice():
    def __init__(self):
        self.m_index = 0xff
        self.m_Key = ""
        self.m_userId = ""
        self.cam = MvCamera()
        self.hThreadHandle = None
        self.m_isExitThread = False

    def init(self,index,camInfo):
        self.m_index=index
        self.m_Key=camInfo.cameraKey
        self.m_userId=camInfo.cameraName
        return IMV_FG_OK

    def openDevice(self):
        return self.cam.IMV_FG_OpenDevice(IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_INDEX, self.m_index)

    def openDevicebyKey(self):
        return self.cam.IMV_FG_OpenDevice(IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_CAMERAKEY, self.m_Key)

    def openDevicebyUserId(self):
        return self.cam.IMV_FG_OpenDevice(IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_DEVICE_USERID, self.m_userId)

    def closeDevice(self):

        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE

        return self.cam.IMV_FG_CloseDevice()

    def setIntValue(self,pFeatureName,intValue):
        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE
        return self.cam.IMV_FG_SetIntFeatureValue(pFeatureName, intValue)

    def getIntValue(self,pFeatureName,pIntValue):
        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE
        return self.cam.IMV_FG_GetIntFeatureValue(pFeatureName, pIntValue)

    def setBoolValue(self,pFeatureName,boolValue):
        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE
        return self.cam.IMV_FG_SetBoolFeatureValue(pFeatureName, boolValue)

    def getBoolValue(self,pFeatureName,boolValue):
        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE
        return self.cam.IMV_FG_GetBoolFeatureValue(pFeatureName, boolValue)

    def setDoubleValue(self,pFeatureName,doubleValue):
        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE
        return self.cam.IMV_FG_SetDoubleFeatureValue(pFeatureName, doubleValue)

    def getDoubleValue(self,pFeatureName,doubleValue):
        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE
        return self.cam.IMV_FG_GetDoubleFeatureValue(pFeatureName, doubleValue)

    def setStringValue(self,pFeatureName,pStringValue):
        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE
        return self.cam.IMV_FG_SetStringFeatureValue(pFeatureName, pStringValue)

    def getStringValue(self,pFeatureName,pStringValue):
        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE
        return self.cam.IMV_FG_GetStringFeatureValue(pFeatureName, pStringValue)

    def setEnumSymbol(self,pFeatureName,pStringValue):
        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE
        return self.cam.IMV_FG_SetEnumFeatureSymbol(pFeatureName, pStringValue)

    def getEnumSymbol(self,pFeatureName,pStringValue):
        if not self.cam.handle:
            return IMV_FG_INVALID_HANDLE
        return self.cam.IMV_FG_GetEnumFeatureSymbol(pFeatureName, pStringValue)

class CaptureCardDevice():
    def __init__(self):
        self.m_index = 0xff
        self.m_Key = ""
        self.m_userId = ""
        self.m_card = Capture()
        self.m_isExitThread = None
        self.clCamera = CameraDevice()

    def init(self,index,interfaceInfo):
        self.m_index = index
        self.m_Key = interfaceInfo.interfaceKey
        self.m_userId = interfaceInfo.interfaceName
        return IMV_FG_OK

    def openDevice(self):
        return self.m_card.IMV_FG_OpenInterface(self.m_index)

    def openDevicebyKey(self):
        return self.m_card.IMV_FG_OpenInterfaceEx(IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_CAMERAKEY, self.m_Key)

    def openDevicebyUserId(self):
        return self.m_card.IMV_FG_OpenInterfaceEx(IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_DEVICE_USERID, self.m_userId)

    def closeDevice(self):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE

        return self.m_card.IMV_FG_CloseInterface()

    def setIntValue(self,pFeatureName,intValue):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        return self.m_card.IMV_FG_SetIntFeatureValue(pFeatureName, intValue)

    def getIntValue(self,pFeatureName,pIntValue):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        return self.m_card.IMV_FG_GetIntFeatureValue(pFeatureName, pIntValue)

    def setBoolValue(self,pFeatureName,boolValue):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        return self.m_card.IMV_FG_SetBoolFeatureValue(pFeatureName, boolValue)

    def getBoolValue(self,pFeatureName,boolValue):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        return self.m_card.IMV_FG_GetBoolFeatureValue(pFeatureName, boolValue)

    def setDoubleValue(self,pFeatureName,doubleValue):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        return self.m_card.IMV_FG_SetDoubleFeatureValue(pFeatureName, doubleValue)

    def getDoubleValue(self,pFeatureName,doubleValue):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        return self.m_card.IMV_FG_GetDoubleFeatureValue(pFeatureName, doubleValue)

    def setStringValue(self,pFeatureName,pStringValue):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        return self.m_card.IMV_FG_SetStringFeatureValue(pFeatureName, pStringValue)

    def getStringValue(self,pFeatureName,pStringValue):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        return self.m_card.IMV_FG_GetStringFeatureValue(pFeatureName, pStringValue)

    def setEnumSymbol(self,pFeatureName,pStringValue):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        return self.m_card.IMV_FG_SetEnumFeatureSymbol(pFeatureName, pStringValue)

    def getEnumSymbol(self,pFeatureName,pStringValue):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        return self.m_card.IMV_FG_GetEnumFeatureSymbol(pFeatureName, pStringValue)

    def getFrameThreadProc(self):
        frame = IMV_FG_Frame()
        if self.m_card.handle == None:
            return IMV_FG_NO_DATA

        while not self.m_isExitThread:
            nRet = self.m_card.IMV_FG_GetFrame(frame, 500)
            if IMV_FG_OK != nRet:
                continue

            print("Get frame blockId = [%d]" % frame.frameInfo.blockId)
            nRet = self.m_card.IMV_FG_ReleaseFrame(frame)
            if IMV_FG_OK != nRet:
                print("Release frame failed! ErrorCode[%d]" % nRet)
        return IMV_FG_OK

    def startGrabbing(self):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE
        self.m_isExitThread = False

        try:
            self.hThreadHandle = threading.Thread(target=self.getFrameThreadProc, args=())
            self.hThreadHandle.start()
        except:
            print("error: unable to start thread")

        return self.m_card.IMV_FG_StartGrabbing()

    def stopGrabbing(self):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE

        self.m_isExitThread = True
        self.hThreadHandle.join()

        return self.m_card.IMV_FG_StopGrabbing()

    def stopGrabbingCallback(self):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE

        return self.m_card.IMV_FG_StopGrabbing()

    def startGrabbingCallback(self):
        if not self.m_card.handle:
            return IMV_FG_INVALID_HANDLE

        ret = self.m_card.IMV_FG_AttachGrabbing(CALL_BACK_FUN,None)
        if ret != IMV_FG_OK:
            print("IMV_FG_AttachGrabbing failed. ret:%d"% ret)

        return self.m_card.IMV_FG_StartGrabbing()

pFrame = POINTER(IMV_FG_Frame)
FrameInfoCallBack = eval('CFUNCTYPE')(None, pFrame, c_void_p)

def onGetFrame(pFrame,pUser):
    if pFrame == None:
        print("pFrame is None!")
        return
    Frame = cast(pFrame, POINTER(IMV_FG_Frame)).contents

    print("Get frame blockID = ", Frame.frameInfo.blockId)
    return

CALL_BACK_FUN = FrameInfoCallBack(onGetFrame)

class DeviceSystem():

    def __init__(self):
        self.m_cardDevice = [CaptureCardDevice() for i in range(16)]
        self.m_cardDeviceNum = 0

    def initSystem(self):
        # 枚举采集卡设备
        interfaceList = IMV_FG_INTERFACE_INFO_LIST()
        interfaceTp = IMV_FG_EInterfaceType.typeCLInterface
        nRet = Capture.IMV_FG_EnumInterface(interfaceTp, interfaceList)
        if (IMV_FG_OK != nRet):
            print("Enumeration devices failed! errorCode:", nRet)

        print("find interface finished. interface num:%d"% interfaceList.nInterfaceNum)
        self.m_cardDeviceNum = interfaceList.nInterfaceNum
        # 枚举相机设备
        nInterfacetype = IMV_FG_EInterfaceType.typeCLInterface
        deviceList = IMV_FG_DEVICE_INFO_LIST()
        nRet = Capture.IMV_FG_EnumDevices(nInterfacetype, deviceList)
        if IMV_FG_OK != nRet:
            print("Enumeration devices failed! ErrorCode", nRet)

        print("find camera finished. camera num:%d."% deviceList.nDevNum)

        for i in range(0,interfaceList.nInterfaceNum):
            self.m_cardDevice[i].init(i, interfaceList.pInterfaceInfoList[i])

            for j in range(0, deviceList.nDevNum):
                if interfaceList.pInterfaceInfoList[i].interfaceKey == deviceList.pDeviceInfoList[j].FGInterfaceInfo.interfaceKey:
                    self.m_cardDevice[i].clCamera.init(j, deviceList.pDeviceInfoList[j])

        # 打印相机基本信息（序号,类型,制造商信息,型号,序列号,用户自定义ID)
        displayDeviceInfo(interfaceList, deviceList)

    def unInitSystem(self):
        self.m_cardDevice = [0 for i in range(16)]