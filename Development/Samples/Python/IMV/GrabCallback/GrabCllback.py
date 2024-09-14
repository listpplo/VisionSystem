# -- coding: utf-8 --

import sys
import time

from ctypes import *

sys.path.append("../MVSDK")
from IMVApi import *

pFrame = POINTER(IMV_Frame)
FrameInfoCallBack = eval('CFUNCTYPE')(None, pFrame, c_void_p)

def image_callback(pFrame, pUser):
    if pFrame==None:
        print ("pFrame is NULL")
        return 
    Frame=cast(pFrame,POINTER(IMV_Frame)).contents
    print ("Get frame blockId = [%d]" % Frame.frameInfo.blockId)

CALL_BACK_FUN = FrameInfoCallBack(image_callback)

def displayDeviceInfo(deviceInfoList):  
    print("Idx  Type   Vendor              Model           S/N                 DeviceUserID    IP Address")
    print("------------------------------------------------------------------------------------------------")
    for i in range(0, deviceInfoList.nDevNum):
        pDeviceInfo = deviceInfoList.pDevInfo[i]
        strType = ""
        strVendorName = pDeviceInfo.vendorName.decode("ascii")
        strModeName = pDeviceInfo.modelName.decode("ascii")
        strSerialNumber = pDeviceInfo.serialNumber.decode("ascii")
        strCameraname = pDeviceInfo.cameraName.decode("ascii")
        strIpAdress = pDeviceInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress.decode("ascii")
        if pDeviceInfo.nCameraType == typeGigeCamera:
            strType = "Gige"
        elif pDeviceInfo.nCameraType == typeU3vCamera:
            strType = "U3V"
        print ("[%d]  %s   %s    %s      %s     %s           %s" % (i+1, strType,strVendorName,strModeName,strSerialNumber,strCameraname,strIpAdress))

if __name__ == "__main__":
    deviceList=IMV_DeviceList()
    interfaceType=IMV_EInterfaceType.interfaceTypeAll
    
    # 枚举设备
    nRet=MvCamera.IMV_EnumDevices(deviceList,interfaceType)
    if IMV_OK != nRet:
        print("Enumeration devices failed! ErrorCode",nRet)
        sys.exit()
    if deviceList.nDevNum == 0:
        print ("find no device!")
        sys.exit()

    print("deviceList size is",deviceList.nDevNum)

    displayDeviceInfo(deviceList)

    nConnectionNum = input("Please input the camera index: ")

    if int(nConnectionNum) > deviceList.nDevNum:
        print ("intput error!")
        sys.exit()

    cam=MvCamera()
    # 创建设备句柄
    nRet=cam.IMV_CreateHandle(IMV_ECreateHandleMode.modeByIndex,byref(c_void_p(int(nConnectionNum)-1)))
    if IMV_OK != nRet:
        print("Create devHandle failed! ErrorCode",nRet)
        sys.exit()
        
    # 打开相机
    nRet=cam.IMV_Open()
    if IMV_OK != nRet:
        print("Open devHandle failed! ErrorCode",nRet)
        sys.exit()
            
    # 注册数据帧回调函数
    nRet = cam.IMV_AttachGrabbing(CALL_BACK_FUN,None)
    if IMV_OK != nRet:
        print("Attach grabbing failed! ErrorCode",nRet)
        sys.exit()
      
    # 开始拉流
    nRet=cam.IMV_StartGrabbing()
    if IMV_OK != nRet:
        print("Start grabbing failed! ErrorCode",nRet)
        sys.exit()
    
    #拉流2s
    time.sleep(2)

    # 停止拉流
    nRet=cam.IMV_StopGrabbing()
    if IMV_OK != nRet:
        print("Stop grabbing failed! ErrorCode",nRet)
        sys.exit()
    
    # 关闭相机
    nRet=cam.IMV_Close()
    if IMV_OK != nRet:
        print("Close camera failed! ErrorCode",nRet)
        sys.exit()
    
    # 销毁句柄
    if(cam.handle):
        nRet=cam.IMV_DestroyHandle()
    
    print("---Demo end---")