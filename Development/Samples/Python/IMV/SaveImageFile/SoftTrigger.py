# -- coding: utf-8 --

import sys

sys.path.append("../MVSDK")
from IMVApi import *
import time
import threading



#软触发线程
def executeSoftTriggerProc(cam):
    frame = IMV_Frame()

    ret = cam.IMV_ExecuteCommandFeature("TriggerSoftware")
    time.sleep(1)
    if ret != IMV_OK:
        print("Execute TriggerSoftware failed! ErrorCode:",ret)
        sys.exit()
    nRet = cam.IMV_GetFrame(frame, 1000)
    if IMV_OK != nRet:
        print("Get frame failed!ErrorCode[%d]" % nRet)
        sys.exit()

    if frame.frameInfo.pixelFormat == IMV_EPixelType.gvspPixelMono8:
        imagesize = frame.frameInfo.width * frame.frameInfo.height
        # # 畸变矫正
        print("Start... Calib image.")
        CalibParam = IMV_ImageCalibParam()
        CalibParam.CalibFilePath = b"./calib_result.Calib"
        ret = cam.IMV_ImageCalib(frame, CalibParam)

        frameFFC = IMV_Frame()
        frameFFC.frameHandle = frame.frameHandle
        frameFFC.frameInfo = frame.frameInfo
        frameFFC.nReserved = frame.nReserved
        frameFFC.pData = None

        if ret == IMV_OK:
            print("Calib image successfully")
            frameFFC.pData = CalibParam.pDstBuf
            # memmove(frameFFC.pData, CalibParam.pDstBuf, CalibParam.nDstBufSize)
        else:
            print("IMV_ImageCalib failed! ErrorCode[%d]" % ret)
            sys.exit()

        # 平场矫正
        print("Start... CalibFFC image.")
        CalibFFCParam = IMV_ImageCalibFFCParam()
        CalibFFCParam.FFCBgroundFilePath = b"./Mono.bmp"
        ret = cam.IMV_ImageCalibFFC(frameFFC, CalibFFCParam)

        print("End...Calib FFC image.")

        print("size1=[%d],size2=[%d] ,size3=[%d] "
              % (CalibFFCParam.nDstBufSize, imagesize, CalibParam.nDstBufSize))

        pDstBuf = c_buffer(b'\0', imagesize)
        memset(pDstBuf, 0, imagesize)
        if ret == IMV_OK:
            print("CalibFFC image successfully")
            userBuff = CalibFFCParam.pDstBuf
            # memmove(pDstBuf, CalibFFCParam.pDstBuf, CalibFFCParam.nDstBufSize)
        else:
            memmove(pDstBuf, frameFFC.pData, CalibParam.nDstBufSize)
            print("IMV_ImageCalibFFC failed! ErrorCode[%d]" % ret)

        cvImage = numpy.frombuffer(pDstBuf, dtype=numpy.ubyte, count=imagesize). \
            reshape(frameFFC.frameInfo.height, frameFFC.frameInfo.width)

    else:
        imageSize = frame.frameInfo.width * frame.frameInfo.height * 3
        # 畸变矫正，输入标定文件
        print("Start... Calib image.")
        CalibParam = IMV_ImageCalibParam()
        CalibParam.CalibFilePath = b"./calib_result.Calib"
        ret = cam.IMV_ImageCalib(frame, CalibParam)
        print("End...Calib image.")

        pDstBuf = c_buffer(b'\0', imageSize)
        memset(pDstBuf, 0, imageSize)
        memmove(pDstBuf, frame.pData, imageSize)
        # dstData = CalibParam.pDstBuf
        # if ret == IMV_OK:
        #     print("Calib image successfully")
        #     memmove(pDstBuf, CalibParam.pDstBuf, CalibParam.nDstBufSize)
        # else:
        #     print("IMV_ImageCalib failed! ErrorCode[%d]" % ret)
        cvImage = numpy.frombuffer(pDstBuf, dtype=numpy.ubyte, count=imageSize). \
            reshape(frame.frameInfo.height, frame.frameInfo.width, 3)

    print("Get frame blockId = [%d]" % frame.frameInfo.blockId)
    nRet = cam.IMV_ReleaseFrame(frame)
    if IMV_OK != nRet:
        print("Release frame failed! ErrorCode[%d]" % nRet)
        sys.exit()

    return 0


#设置软触发配置
def setSoftTriggerConf(cam):

    #设置触发源为软触发
    ret = cam.IMV_SetEnumFeatureSymbol("TriggerSource","Software")
    if ret != IMV_OK:
        print("Set triggerSource value failed! ErrorCode:",ret)
        return ret

    #设置触发器
    ret = cam.IMV_SetEnumFeatureSymbol("TriggerSelector","FrameStart")
    if ret != IMV_OK:
        print("Set triggerSelector value failed! ErrorCode:",ret)
        return ret

    #设置触发模式
    ret = cam.IMV_SetEnumFeatureSymbol("TriggerMode","On")
    if ret != IMV_OK:
        print("Set triggerMode value failed! ErrorCode:",ret)
        return ret

    return ret

#打印设备信息
def displayDeviceInfo(deviceInfoList):  
    print("Idx  Type   Vendor              Model           S/N                 DeviceUserID    IP Address")
    print("------------------------------------------------------------------------------------------------")
    for i in range(0,deviceInfoList.nDevNum):
        pDeviceInfo=deviceInfoList.pDevInfo[i]
        strType=""
        strVendorName=""
        strModeName = ""
        strSerialNumber=""
        strCameraname=""
        strIpAdress=""
        for str in pDeviceInfo.vendorName:
            strVendorName = strVendorName + chr(str)
        for str in pDeviceInfo.modelName:
            strModeName = strModeName + chr(str)
        for str in pDeviceInfo.serialNumber:
            strSerialNumber = strSerialNumber + chr(str)
        for str in pDeviceInfo.cameraName:
            strCameraname = strCameraname + chr(str)
        for str in pDeviceInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress:
                strIpAdress = strIpAdress + chr(str)
        if pDeviceInfo.nCameraType == IMV_ECameraType.typeGigeCamera:
            strType="Gige"
        elif pDeviceInfo.nCameraType == IMV_ECameraType.typeU3vCamera:
            strType="U3V"
        elif pDeviceInfo.nCameraType == IMV_ECameraType.typeCLCamera:
            strType="CL"
        elif pDeviceInfo.nCameraType == IMV_ECameraType.typePCIeCamera:
            strType="PCIe"
        print ("[%d]  %s   %s    %s      %s     %s           %s" % (i+1, strType,strVendorName,strModeName,strSerialNumber,strCameraname,strIpAdress))



if __name__ == "__main__":

    deviceList = IMV_DeviceList()
    interfaceType = IMV_EInterfaceType.interfaceTypeAll
    # 枚举设备
    nRet = MvCamera.IMV_EnumDevices(deviceList, interfaceType)
    print("deviceList type is", type(deviceList))
    if IMV_OK != nRet:
        print("Enumeration devices failed! ErrorCode", nRet)
        sys.exit()
    if deviceList.nDevNum == 0:
        print("find no device!")
        sys.exit()

    displayDeviceInfo(deviceList)

    nConnectionNum = input("Please input the camera index: ")

    if int(nConnectionNum) > deviceList.nDevNum:
        print("intput error!")
        sys.exit()

    cam = MvCamera()
    # 创建设备句柄
    nRet = cam.IMV_CreateHandle(IMV_ECreateHandleMode.modeByIndex, byref(c_void_p(int(nConnectionNum)-1)))
    if IMV_OK != nRet:
        print("Create devHandle failed! ErrorCode", nRet)
        sys.exit()

    # 打开相机
    nRet = cam.IMV_Open()
    if IMV_OK != nRet:
        print("Open devHandle failed! ErrorCode", nRet)
        sys.exit()
    
    #设置软触发配置
    # nRet = setSoftTriggerConf(cam)

    nRet = cam.IMV_SetBufferCount(1)

    # 开始拉流
    nRet = cam.IMV_StartGrabbing()

    if IMV_OK != nRet:
        print("Start grabbing failed! ErrorCode", nRet)
        sys.exit()

    while (1):
        threading.Thread(target=executeSoftTriggerProc, args=(cam,)).start()
        time.sleep(3)


    # 停止拉流
    cam.IMV_StopGrabbing()
    if IMV_OK != nRet:
        print("Stop grabbing failed! ErrorCode", nRet)
        sys.exit()

    # 关闭相机
    nRet = cam.IMV_Close()
    if IMV_OK != nRet:
        print("Close camera failed! ErrorCode", nRet)
        sys.exit()

    # 销毁句柄
    if (cam.handle):
        nRet = cam.IMV_DestroyHandle()

    print("---Process exit---")
    sys.exit()