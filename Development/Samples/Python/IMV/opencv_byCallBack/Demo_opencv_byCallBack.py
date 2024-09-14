# -- coding: utf-8 --

import sys
from ctypes import *
import datetime
import numpy
import cv2
import gc
import time

sys.path.append("Development\Samples\Python\IMV\MVSDK")
from IMVApi import *

pFrame = POINTER(IMV_Frame)
FrameInfoCallBack = eval('CFUNCTYPE')(None, pFrame, c_void_p)

def image_callback(pFrame, pUser):
    if pFrame==None:
        print ("pFrame is NULL")
        return 
    stPixelConvertParam=IMV_PixelConvertParam()
    frame=cast(pFrame,POINTER(IMV_Frame)).contents
    print("getFrame success BlockId = [" + str(frame.frameInfo.blockId) + "], get frame time: " + str(datetime.datetime.now()))
    
    # 给转码所需的参数赋值

    if IMV_EPixelType.gvspPixelMono8==frame.frameInfo.pixelFormat:
        nDstBufSize=frame.frameInfo.width * frame.frameInfo.height
    else:
        nDstBufSize=frame.frameInfo.width * frame.frameInfo.height*3
    
    pDstBuf=(c_ubyte*nDstBufSize)()
    memset(byref(stPixelConvertParam), 0, sizeof(stPixelConvertParam))
    
    stPixelConvertParam.nWidth = frame.frameInfo.width
    stPixelConvertParam.nHeight = frame.frameInfo.height
    stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat
    stPixelConvertParam.pSrcData = frame.pData
    stPixelConvertParam.nSrcDataLen = frame.frameInfo.size
    stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX
    stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY
    stPixelConvertParam.eBayerDemosaic = IMV_EBayerDemosaic.demosaicNearestNeighbor
    stPixelConvertParam.eDstPixelFormat = frame.frameInfo.pixelFormat
    stPixelConvertParam.pDstBuf = pDstBuf
    stPixelConvertParam.nDstBufSize = nDstBufSize
    
    # 释放驱动图像缓存
    # release frame resource at the end of use
    
    nRet = cam.IMV_ReleaseFrame(frame)
    if IMV_OK != nRet:
        print("Release frame failed! ErrorCode[%d]"% nRet)
        sys.exit()
    
    # 如果图像格式是 Mono8 直接使用
    # no format conversion required for Mono8
    if stPixelConvertParam.ePixelFormat == IMV_EPixelType.gvspPixelMono8:
        imageBuff=stPixelConvertParam.pSrcData
        userBuff = c_buffer(b'\0', stPixelConvertParam.nDstBufSize)
    
        memmove(userBuff,imageBuff,stPixelConvertParam.nDstBufSize)
        grayByteArray = bytearray(userBuff)
        
        cvImage = numpy.array(grayByteArray).reshape(stPixelConvertParam.nHeight, stPixelConvertParam.nWidth)
        
    else:
        # 转码 => BGR24
        stPixelConvertParam.eDstPixelFormat=IMV_EPixelType.gvspPixelBGR8
        stPixelConvertParam.nDstBufSize=nDstBufSize

        nRet=cam.IMV_PixelConvert(stPixelConvertParam)
        if IMV_OK!=nRet:
            print("image convert to failed! ErrorCode[%d]" % nRet)
            del pDstBuf
            sys.exit()
        rgbBuff = c_buffer(b'\0', stPixelConvertParam.nDstBufSize)
        memmove(rgbBuff,stPixelConvertParam.pDstBuf,stPixelConvertParam.nDstBufSize)
        colorByteArray = bytearray(rgbBuff)
        cvImage = numpy.array(colorByteArray).reshape(stPixelConvertParam.nHeight, stPixelConvertParam.nWidth, 3)
        if None!=pDstBuf:
            del pDstBuf


    # --- end if ---
    cvImage = cv2.resize(cvImage, (720, 480))
    cv2.imshow('myWindow', cvImage)
    gc.collect()

    if (cv2.waitKey(1) >= 0):
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

if __name__ == "__main__":
    deviceList=IMV_DeviceList()
    interfaceType=IMV_EInterfaceType.interfaceTypeAll
    nWidth=c_uint()
    nHeight=c_uint()

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
    
   # 通用属性设置:设置触发模式为off
    nRet=cam.IMV_SetEnumFeatureSymbol("TriggerSource","Software")
    if IMV_OK != nRet:
        print("Set triggerSource value failed! ErrorCode[%d]" % nRet)
        sys.exit()
        
    nRet=cam.IMV_SetEnumFeatureSymbol("TriggerSelector","FrameStart")
    if IMV_OK != nRet:
        print("Set triggerSelector value failed! ErrorCode[%d]" % nRet)
        sys.exit()

    nRet=cam.IMV_SetEnumFeatureSymbol("TriggerMode","Off")
    if IMV_OK != nRet:
        print("Set triggerMode value failed! ErrorCode[%d]" % nRet)
        sys.exit()
    
    nRet = cam.IMV_AttachGrabbing(CALL_BACK_FUN,None)
    
    if IMV_OK != nRet:
        print("Attach grabbing failed! ErrorCode",nRet)
        sys.exit()

    # 开始拉流
    nRet=cam.IMV_StartGrabbing()
    if IMV_OK != nRet:
        print("Start grabbing failed! ErrorCode",nRet)
        sys.exit()

    # 取流10s
    time.sleep(30)
    
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