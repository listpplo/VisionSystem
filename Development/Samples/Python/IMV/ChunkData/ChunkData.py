# -- coding: utf-8 --

import threading
import sys
import time

sys.path.append("../MVSDK")
from IMVApi import *

pFrame = POINTER(IMV_Frame)
FrameInfoCallBack = eval('CFUNCTYPE')(None, pFrame, c_void_p)

g_bExit = False

# 为线程定义一个函数
def work_thread(cam):
    devHandle=cam.handle
    if None==devHandle:
        return
    while False==g_bExit:
        nRet=cam.IMV_ExecuteCommandFeature("TriggerSoftware")
        if IMV_OK!=nRet:
            print("Execute TriggerSoftware failed! ErrorCode[%d]" % nRet)
            continue
        time.sleep(0.05)

def image_callback(pFrame, pUser):
    chunkDataInfo=IMV_ChunkDataInfo()
    devHandle=pUser
    if pFrame==None:
        print ("pFrame is NULL")
        return 
    Frame=cast(pFrame,POINTER(IMV_Frame)).contents
    print ("Get frame blockId = [%d]" % Frame.frameInfo.blockId)
    if devHandle==None:
        print("devHandle is NULL!")
        return
    
    for i in range(0,Frame.frameInfo.chunkCount):
        nRet=cam.IMV_GetChunkDataByIndex(Frame,i,chunkDataInfo)
        if IMV_OK!=nRet:
            print("Get ChunkData failed! ErrorCode", nRet)
            continue

        print("chunkID=",chunkDataInfo.chunkID)
        for j in range(0,chunkDataInfo.nParamCnt):
            print("paramName = ", chunkDataInfo.pParamNameList[j].str.decode('utf-8'))
    return

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

def setSoftTriggerConf(cam):
    nRet = cam.IMV_SetEnumFeatureSymbol("TriggerSource", "Software")
    if IMV_OK != nRet:
        print("Set triggerSource value failed! ErrorCode[%d]" % nRet)
        return nRet

    nRet = cam.IMV_SetEnumFeatureSymbol("TriggerSelector", "FrameStart")
    if IMV_OK != nRet:
        print("Set triggerSelector value failed! ErrorCode[%d]" % nRet)
        return nRet

    nRet = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "On")
    if IMV_OK != nRet:
        print("Set triggerMode value failed! ErrorCode[%d]" % nRet)
        return nRet

    return nRet

def setChunkDataConf(cam):
    deviceInfo=IMV_DeviceInfo()
    nRet=cam.IMV_GetDeviceInfo(deviceInfo)
    if IMV_OK != nRet:
        print("Get device info failed! ErrorCode[%d]" % nRet)
        return nRet

    if ((typeGigeCamera != deviceInfo.nCameraType) & (typeU3vCamera != deviceInfo.nCameraType)):
        print("CameraType [%d]", deviceInfo.nCameraType)
        return IMV_NOT_SUPPORT

    # 设置CounterSelector
    nRet = cam.IMV_SetEnumFeatureSymbol("CounterSelector", "Counter0")
    if IMV_OK != nRet:
        print("Set CounterSelector value failed! ErrorCode[%d]" % nRet)
        return nRet

    # 设置CounterResetSource
    nRet = cam.IMV_SetEnumFeatureSymbol("CounterResetSource", "SoftwareSignal0")
    if IMV_OK != nRet:
        print("Set CounterSelector value failed! ErrorCode[%d]" % nRet)
        return nRet

    # 执行CounterReset
    nRet = cam.IMV_ExecuteCommandFeature("CounterReset")
    if IMV_OK != nRet:
        print("Execute CounterReset failed! ErrorCode[%d]" % nRet)
        return nRet

    # 设置CounterResetSource
    nRet = cam.IMV_SetEnumFeatureSymbol("CounterResetSource", "SoftwareSignal0")
    if IMV_OK != nRet:
        print("Set CounterResetSource value failed! ErrorCode[%d]\n" % nRet)
        return nRet

    # 设置CounterResetSource
    nRet = cam.IMV_SetEnumFeatureSymbol("CounterResetSource", "Off")
    if IMV_OK != nRet:
        print("Set CounterResetSource value failed! ErrorCode[%d]\n" % nRet)
        return nRet

    if typeGigeCamera == deviceInfo.nCameraType:
        # 设置GevGVSPExtendedIDMode
        nRet = cam.IMV_SetEnumFeatureSymbol("GevGVSPExtendedIDMode", "On");
        if IMV_OK != nRet:
            print("Set GevGVSPExtendedIDMode value failed! ErrorCode[%d]" % nRet)
            return nRet

    # 设置ChunkModeActive
    nRet = cam.IMV_SetBoolFeatureValue("ChunkModeActive", True);
    if IMV_OK != nRet:
        print("Set ChunkModeActive value failed! ErrorCode[%d]" % nRet);
        return nRet

    # 设置ChunkSelector
    nRet = cam.IMV_SetEnumFeatureSymbol("ChunkSelector", "Counter0Value")
    if IMV_OK != nRet:
        print("Set ChunkSelector value failed! ErrorCode[%d]" % nRet)
        return nRet

    # 设置ChunkEnable
    nRet = cam.IMV_SetBoolFeatureValue("ChunkEnable",True)
    if IMV_OK != nRet:
        print("Set ChunkEnable value failed! ErrorCode[%d]"% nRet)
        return nRet

    return nRet

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
	
    # 设置软触发配置
    nRet=setSoftTriggerConf(cam)
    if IMV_OK != nRet:
        print("setSoftTriggerConf failed!")
        sys.exit()

    # 设置ChunkData配置
    nRet=setChunkDataConf(cam)
    if IMV_OK != nRet:
        print("setChunkDataConf failed!")
        sys.exit()

    # 注册数据帧回调函数
    nRet = cam.IMV_AttachGrabbing(CALL_BACK_FUN,cam.handle)
    if IMV_OK != nRet:
        print("Attach grabbing failed! ErrorCode",nRet)
        sys.exit()
      
    # 开始拉流
    nRet=cam.IMV_StartGrabbing()
    if IMV_OK != nRet:
        print("Start grabbing failed! ErrorCode",nRet)
        sys.exit()
    
    try:
        hThreadHandle = threading.Thread(target=work_thread, args=(cam,))
        hThreadHandle.start()
    except:
        print ("error: unable to start thread")
    
    time.sleep(2)

    g_bExit=True
    hThreadHandle.join()

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


    
        




    