# -- coding: utf-8 --

import sys
import numpy
import cv2

sys.path.append("../MVSDK")
from IMVApi import *

""" // 保存bmp图像
// save image to bmp """
def saveImageToBmp(cam,frame):
    stPixelConvertParam=IMV_PixelConvertParam()

    """ // mono8和BGR8裸数据不需要转码
	// mono8 and BGR8 raw data is not need to convert """
    if ((frame.frameInfo.pixelFormat != IMV_EPixelType.gvspPixelMono8)
        & (frame.frameInfo.pixelFormat != IMV_EPixelType.gvspPixelBGR8)):
        nSize = frame.frameInfo.width * frame.frameInfo.height * 3
        g_pConvertBuf = (c_ubyte * nSize)()
        """ // 图像转换成BGR8
		// convert image to BGR8 """
        memset(byref(stPixelConvertParam), 0, sizeof(stPixelConvertParam))
        stPixelConvertParam.nWidth = frame.frameInfo.width
        stPixelConvertParam.nHeight = frame.frameInfo.height
        stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat
        stPixelConvertParam.pSrcData = frame.pData
        stPixelConvertParam.nSrcDataLen = frame.frameInfo.size
        stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX
        stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY
        stPixelConvertParam.eBayerDemosaic = IMV_EBayerDemosaic.demosaicNearestNeighbor
        stPixelConvertParam.eDstPixelFormat = IMV_EPixelType.gvspPixelBGR8
        stPixelConvertParam.pDstBuf = g_pConvertBuf
        stPixelConvertParam.nDstBufSize = frame.frameInfo.width * frame.frameInfo.height * 3
        ret = cam.IMV_PixelConvert(stPixelConvertParam)
        if IMV_OK != ret:
            print("image convert to BGR failed! ErrorCode[%d]", ret)
            return False
        pImageData = g_pConvertBuf
        pixelFormat = IMV_EPixelType.gvspPixelBGR8
    else:
        pImageData = frame.pData
        pixelFormat = frame.frameInfo.pixelFormat

    if pixelFormat == IMV_EPixelType.gvspPixelMono8:
        imageSize = frame.frameInfo.width * frame.frameInfo.height
    else:
        imageSize = frame.frameInfo.width * frame.frameInfo.height * 3

    userBuff = c_buffer(b'\0', imageSize)
    memmove(userBuff, pImageData, imageSize)
    if pixelFormat == IMV_EPixelType.gvspPixelMono8:
        numpy_image = numpy.frombuffer(userBuff, dtype=numpy.ubyte, count=imageSize). \
            reshape(frame.frameInfo.height, frame.frameInfo.width)
    else:
        numpy_image = numpy.frombuffer(userBuff, dtype=numpy.ubyte, count=imageSize). \
            reshape(frame.frameInfo.height, frame.frameInfo.width, 3)

    cv2.imwrite('test.bmp',numpy_image)
    return True


""" // ***********开始： 这部分处理与SDK操作相机无关，用于显示设备列表 ***********
// ***********BEGIN: These functions are not related to API call and used to display device info*********** """
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
    frame=IMV_Frame()
    
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

    # 开始拉流
    nRet=cam.IMV_StartGrabbing()
    if IMV_OK != nRet:
        print("Start grabbing failed! ErrorCode",nRet)
        sys.exit()
    
    # 取一帧
    nRet=cam.IMV_GetFrame(frame,500)
    if IMV_OK!=nRet:
        print("Get frame failed!ErrorCode[%d]" % nRet) 
        sys.exit()

    if saveImageToBmp(cam,frame):
        print("Save image to bmp successfully!")
    else:
        print("Save image to bmp failed!")
        sys.exit()
    
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
