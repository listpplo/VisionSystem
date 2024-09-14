# -- coding: utf-8 --

import sys
import time
import threading

sys.path.append("../MVSDK")
from IMVApi import *

def displayDeviceInfo(deviceInfoList):
    print("Idx  Type   Vendor              Model           S/N                 DeviceUserID    IP Address")
    print("------------------------------------------------------------------------------------------------")
    for i in range(0,deviceInfoList.nDevNum):
        pDeviceInfo=deviceInfoList.pDevInfo[i]
        strType=""
        strVendorName = pDeviceInfo.vendorName.decode("ascii")
        strModeName = pDeviceInfo.modelName.decode("ascii")
        strSerialNumber = pDeviceInfo.serialNumber.decode("ascii")
        strCameraname = pDeviceInfo.cameraName.decode("ascii")
        strIpAdress = pDeviceInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress.decode("ascii")
        if pDeviceInfo.nCameraType == typeGigeCamera:
            strType="Gige"
        elif pDeviceInfo.nCameraType == typeU3vCamera:
            strType="U3V"
        print ("[%d]  %s   %s    %s      %s     %s           %s" % (i+1, strType,strVendorName,strModeName,strSerialNumber,strCameraname,strIpAdress))

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
        return IMV_OK

    def openDevice(self):
        cam = MvCamera()
        nRet = cam.IMV_CreateHandle(IMV_ECreateHandleMode.modeByIndex, byref(c_void_p(self.m_index)))
        if IMV_OK != nRet:
            print("Create devHandle failed! ErrorCode", nRet)
            sys.exit()

        # 打开相机
        nRet = cam.IMV_Open()
        if IMV_OK != nRet:
            print("Open devHandle failed! ErrorCode", nRet)
            cam.IMV_DestroyHandle()
            sys.exit()

    def openDevicebyKey(self):

        nRet = self.cam.IMV_CreateHandle(IMV_ECreateHandleMode.modeByCameraKey, self.m_Key)
        if IMV_OK != nRet:
            print("Create devHandle by CameraKey failed! Key is [%s], ErrorCode[%d]", self.m_Key, nRet)
            sys.exit()

        # 打开相机
        nRet = self.cam.IMV_Open()
        if IMV_OK != nRet:
            print("Open devHandle failed! ErrorCode", nRet)
            self.cam.IMV_DestroyHandle()
            sys.exit()

        return nRet

    def openDevicebyUserId(self):

        nRet = self.cam.IMV_CreateHandle(IMV_ECreateHandleMode.modeByDeviceUserID, self.m_userId.encode("ascii"))
        if IMV_OK != nRet:
            print("Create devHandle by device user id failed! User id is [%s], ErrorCode[%d]", self.m_userId, nRet)
            sys.exit()

        # 打开相机
        nRet = self.cam.IMV_Open()
        if IMV_OK != nRet:
            print("Open devHandle failed! ErrorCode", nRet)
            self.cam.IMV_DestroyHandle()
            sys.exit()

    def closeDevice(self):
        # 关闭相机
        nRet = self.cam.IMV_Close()
        if IMV_OK != nRet:
            print("Close camera failed! ErrorCode", nRet)
            sys.exit()

        # 销毁句柄
        if (self.cam.handle):
            self.cam.IMV_DestroyHandle()

    def setIntValue(self,pFeatureName,intValue):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        return self.cam.IMV_SetIntFeatureValue(pFeatureName, intValue)

    def getIntValue(self,pFeatureName,pIntValue):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        return self.cam.IMV_GetIntFeatureValue(pFeatureName, pIntValue)

    def setBoolValue(self,pFeatureName,boolValue):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        return self.cam.IMV_SetBoolFeatureValue(pFeatureName, boolValue)

    def getBoolValue(self,pFeatureName,boolValue):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        return self.cam.IMV_GetBoolFeatureValue(pFeatureName, boolValue)

    def setDoubleValue(self,pFeatureName,doubleValue):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        return self.cam.IMV_SetDoubleFeatureValue(pFeatureName, doubleValue)

    def getDoubleValue(self,pFeatureName,doubleValue):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        return self.cam.IMV_GetDoubleFeatureValue(pFeatureName, doubleValue)

    def setStringValue(self,pFeatureName,pStringValue):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        return self.cam.IMV_SetStringFeatureValue(pFeatureName, pStringValue)

    def getStringValue(self,pFeatureName,pStringValue):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        return self.cam.IMV_GetStringFeatureValue(pFeatureName, pStringValue)

    def setEnumSymbol(self,pFeatureName,pStringValue):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        return self.cam.IMV_SetEnumFeatureSymbol(pFeatureName, pStringValue)

    def getEnumSymbol(self,pFeatureName,pStringValue):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        return self.cam.IMV_GetEnumFeatureSymbol(pFeatureName, pStringValue)

    def getFrameThreadProc(self):
        frame = IMV_Frame()
        if self.cam.handle == None:
            return IMV_INVALID_HANDLE

        while not self.m_isExitThread:
            nRet = self.cam.IMV_GetFrame(frame, 500)
            if IMV_OK != nRet:
                continue

            print("Get frame blockId = [%d]" % frame.frameInfo.blockId)
            nRet = self.cam.IMV_ReleaseFrame(frame)
            if IMV_OK != nRet:
                print("Release frame failed! ErrorCode[%d]" % nRet)
        return IMV_OK

    def startGrabbing(self):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE
        self.m_isExitThread = False

        try:
            self.hThreadHandle = threading.Thread(target=self.getFrameThreadProc, args=())
            self.hThreadHandle.start()
        except:
            print("error: unable to start thread")

        return self.cam.IMV_StartGrabbing()

    def stopGrabbing(self):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE

        self.m_isExitThread = True
        self.hThreadHandle.join()

        return self.cam.IMV_StopGrabbing()

    def stopGrabbingCallback(self):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE

        return self.cam.IMV_StopGrabbing()

    def startGrabbingCallback(self):
        if not self.cam.handle:
            return IMV_INVALID_HANDLE

        ret = self.cam.IMV_AttachGrabbing(CALL_BACK_FUN,None)
        if ret != IMV_OK:
            print("IMV_AttachGrabbing failed. ret:%d"% ret)

        return self.cam.IMV_StartGrabbing()


pFrame = POINTER(IMV_Frame)
FrameInfoCallBack = eval('CFUNCTYPE')(None, pFrame, c_void_p)

def onGetFrame(pFrame,pUser):
    if pFrame == None:
        print("pFrame is None!")
        return
    Frame = cast(pFrame, POINTER(IMV_Frame)).contents

    print("Get frame blockID = ", Frame.frameInfo.blockId)
    return

CALL_BACK_FUN = FrameInfoCallBack(onGetFrame)

class DeviceSystem():

    def __init__(self):
        self.m_Device = [CameraDevice() for i in range(16)]
        self.m_DeviceNum = 0

    def initSystem(self):
        deviceList = IMV_DeviceList()
        interfaceType = IMV_EInterfaceType.interfaceTypeAll

        # 枚举设备
        nRet = MvCamera.IMV_EnumDevices(deviceList, interfaceType)
        if IMV_OK != nRet:
            print("Enumeration devices failed! ErrorCode", nRet)
            sys.exit()
        if deviceList.nDevNum == 0:
            print("find no device!")
            sys.exit()

        self.m_DeviceNum = deviceList.nDevNum
        for i in range(0,deviceList.nDevNum):
            self.m_Device[i].init(i,deviceList.pDevInfo[i])

        print("deviceList size is", deviceList.nDevNum)
        displayDeviceInfo(deviceList)

    def unInitSystem(self):
        self.m_Device = [0 for i in range(16)]