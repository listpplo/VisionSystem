# -- coding: utf-8 --

import sys
import time

sys.path.append("../MVSDK")
from IMVApi import *

winfun_ctype = WINFUNCTYPE

pFrame = POINTER(IMV_Frame)
FrameInfoCallBack = winfun_ctype(None, pFrame, c_void_p)

def onGetFrame(pFrame ,pUSer):
    if pFrame == None:
        print("pFrame is NULL")
        return
    Frame = cast(pFrame, POINTER(IMV_Frame)).contents
    print("Get frame blockId = [%d]" % Frame.frameInfo.blockId)
    return

CALL_BACK_FUN = FrameInfoCallBack(onGetFrame)

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

def strtok(string, delimiters):
    string = string.strip(delimiters)
    s = string.split(delimiters)
    return s[0]

def autoSetXameraIP(self):

    subnetStr = [0]*5
    subnetStrIndex = 0
    ipValue = 253
    devInfo = IMV_DeviceInfo()
    #获取设备信息
    nRet = cam.IMV_GetDeviceInfo(devInfo)
    if nRet != IMV_OK:
        print("get device info failed! ErrorCode",nRet)
        sys.exit()

    #判断设备和主机IP的网段是否匹配
    if devInfo.DeviceSpecificInfo.gigeDeviceInfo.ipConfiguration.decode("ascii") == 'Valid':
        return IMV_OK
    
    print("Device ip address (before):",devInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress.decode("ascii"))
    print("Device subnetMask (before):",devInfo.DeviceSpecificInfo.gigeDeviceInfo.subnetMask.decode("ascii"))
    print("Device gateway (before):",devInfo.DeviceSpecificInfo.gigeDeviceInfo.defaultGateWay.decode("ascii"))
    print("Device macAddress (before):",devInfo.DeviceSpecificInfo.gigeDeviceInfo.macAddress.decode("ascii"))

    print(" ")
    print("Interface ip address:",devInfo.InterfaceInfo.gigeInterfaceInfo.ipAddress.decode("ascii"))
    print("Interface subnetMask:",devInfo.InterfaceInfo.gigeInterfaceInfo.subnetMask.decode("ascii"))
    print("Interface gateway:",devInfo.InterfaceInfo.gigeInterfaceInfo.defaultGateWay.decode("ascii"))
    print("Interface macAddress:",devInfo.InterfaceInfo.gigeInterfaceInfo.macAddress.decode("ascii"))
    print(" ")

    ipAddress = devInfo.InterfaceInfo.gigeInterfaceInfo.ipAddress.decode("ascii")

    while True:
        subnetStr[subnetStrIndex] = strtok(ipAddress, '.')
        if not subnetStr[subnetStrIndex]:
            break
        ipAddress = ipAddress[len(str(subnetStr[subnetStrIndex])) + 1:]
        subnetStrIndex = subnetStrIndex + 1

    while ipValue:
        if ipValue is not subnetStr[3]:
            break
        ipValue = ipValue - 1

    newIPStr = '{}.{}.{}.{}'.format(str(subnetStr[0]),str(subnetStr[1]),str(subnetStr[2]),str(ipValue))

    print("New device ip address:",newIPStr)
    print(" ")
    
    #修改设备临时IP
    nRet = cam.IMV_GIGE_ForceIpAddress(str(newIPStr),
    devInfo.InterfaceInfo.gigeInterfaceInfo.subnetMask.decode("ascii"),
    devInfo.InterfaceInfo.gigeInterfaceInfo.defaultGateWay.decode("ascii"))
    if nRet != IMV_OK:
        print("Set device ip failed! ErrorCode:",nRet)
        return nRet
    
    nRet = cam.IMV_GetDeviceInfo(devInfo)
    if nRet != IMV_OK:
        print("Get device info failed! ErrorCode:",nRet)
        return nRet
    
    print("Device ip address (after):",devInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress.decode("ascii"))
    print("Device subnetMask (after):",devInfo.DeviceSpecificInfo.gigeDeviceInfo.subnetMask.decode("ascii"))
    print("Device gateway (after):",devInfo.DeviceSpecificInfo.gigeDeviceInfo.defaultGateWay.decode("ascii"))
    print("Device macAddress (after):",devInfo.DeviceSpecificInfo.gigeDeviceInfo.macAddress.decode("ascii"))
    print(" ")

    #打开相机（返回错误码为IMV_ERROR_INVALID_IP，表示设备与主机网段不匹配）
    nRet = cam.IMV_Open()
    if nRet != IMV_OK:
        print("Open camera failed! ErrorCode:",nRet)
        return nRet

    nRet = cam.IMV_SetBoolFeatureValue("GevCurrentIPConfigurationPersistentIP",True)
    if nRet != IMV_OK:
        print("Set GevCurrentIPConfigurationPersistentIP failed! ErrorCode:",nRet)
        return nRet

    nRet = cam.IMV_SetStringFeatureValue("GevPersistentIPAddress",
    devInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress.decode("ascii"))
    if nRet != IMV_OK:
        print("Set GevPersistentIPAddress failed! ErrorCode:",nRet)
        return nRet

    nRet = cam.IMV_SetStringFeatureValue("GevPersistentSubnetMask",
    devInfo.DeviceSpecificInfo.gigeDeviceInfo.subnetMask.decode("ascii"))
    if nRet != IMV_OK:
        print("Set GevPersistentSubnetMask failed! ErrorCode:",nRet)
        return nRet

    nRet = cam.IMV_SetStringFeatureValue("GevPersistentDefaultGateway",
    devInfo.DeviceSpecificInfo.gigeDeviceInfo.defaultGateWay.decode("ascii"))
    if nRet != IMV_OK:
        print("Set GevPersistentDefaultGateway failed! ErrorCode:",nRet)
        return nRet
    
    return nRet

if __name__ == "__main__":
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

    print("deviceList size is", deviceList.nDevNum)

    displayDeviceInfo(deviceList)

    nConnectionNum = input("Please input the camera index: ")

    if int(nConnectionNum) > deviceList.nDevNum:
        print("intput error!")
        sys.exit()

    cam = MvCamera()
    # 创建设备句柄
    nRet = cam.IMV_CreateHandle(IMV_ECreateHandleMode.modeByIndex, byref(c_void_p(int(nConnectionNum) - 1)))
    if IMV_OK != nRet:
        print("Create devHandle failed! ErrorCode", nRet)
        sys.exit()

    nRet = autoSetXameraIP(cam)
    if IMV_OK != nRet:
        print("Set camera IP failed!")
        sys.exit()

    #判断相机是否打开状态
    if not cam.IMV_IsOpen():
        #打开相机
        nRet = cam.IMV_Open()
        if nRet != IMV_OK:
            print("Open camera failed! ErrorCode:",nRet)
            sys.exit()

    #注册数据帧回调函数
    nRet = cam.IMV_AttachGrabbing(CALL_BACK_FUN,None)
    if nRet != IMV_OK:
        print("Attach grabbing failed! ErrorCode:",nRet)
        sys.exit()

    #开始拉流
    nRet = cam.IMV_StartGrabbing()
    if nRet != IMV_OK:
        print("Start grabbing failed! ErrorCode:",nRet)
        sys.exit()

    #取图2秒
    time.sleep(2)

    #停止拉流
    nRet = cam.IMV_StopGrabbing()
    if nRet != IMV_OK:
        print("Stop grabbing failed! ErrorCode:",nRet)
        sys.exit()

    # 关闭相机
    nRet = cam.IMV_Close()
    if IMV_OK != nRet:
        print("Close camera failed! ErrorCode", nRet)
        sys.exit()

    # 销毁句柄
    if (cam.handle):
        nRet = cam.IMV_DestroyHandle()

    print("---Demo end---")
    