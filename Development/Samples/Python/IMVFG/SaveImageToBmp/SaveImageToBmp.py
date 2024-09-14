# -- coding: utf-8 --

import sys
import time
import numpy
import cv2

sys.path.append("../MVSDK")
from IMVFGApi import *

g_isRunThread = True

class BitmapRGBQuad(Structure):
    _fields_ = [
        ('rgbBlue', c_char),
        ('rgbGreen', c_char),
        ('rgbRed', c_char),
        ('rgbReserved', c_char)
    ]

g_pConvertBuf = None
g_colorTable = (BitmapRGBQuad * 256)()

# 保存bmp图像
def saveImageToBmp(cam, frame):
    stPixelConvertParam = IMV_FG_PixelConvertParam()

    """ // mono8和BGR8裸数据不需要转码
	// mono8 and BGR8 raw data is not need to convert """
    if ((frame.frameInfo.pixelFormat != IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8)
            & (frame.frameInfo.pixelFormat != IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8)):
        if g_pConvertBuf == None:
            print("g_pConvertBuf is NULL")
            return False

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
        stPixelConvertParam.eBayerDemosaic = IMV_FG_EBayerDemosaic.IMV_FG_DEMOSAIC_NEAREST_NEIGHBOR
        stPixelConvertParam.eDstPixelFormat = IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8
        stPixelConvertParam.pDstBuf = g_pConvertBuf
        stPixelConvertParam.nDstBufSize = frame.frameInfo.width * frame.frameInfo.height * 3

        ret = cam.IMV_FG_PixelConvert(stPixelConvertParam)
        if IMV_FG_OK != ret:
            print("image convert to BGR failed! ErrorCode[%d]" %  ret)
            return False
        pImageData = g_pConvertBuf
        pixelFormat = IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8
    else:
        pImageData = frame.pData
        pixelFormat = frame.frameInfo.pixelFormat

    userBuff = c_buffer(b'\0', frame.frameInfo.size)
    memmove(userBuff, pImageData, frame.frameInfo.size)
    if pixelFormat == IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8:
        imageSize = frame.frameInfo.width * frame.frameInfo.height
        numpy_image = numpy.frombuffer(userBuff, dtype=numpy.ubyte, count=imageSize). \
            reshape(frame.frameInfo.height, frame.frameInfo.width)
    else:
        imageSize = frame.frameInfo.width * frame.frameInfo.height * 3
        numpy_image = numpy.frombuffer(userBuff, dtype=numpy.ubyte, count=imageSize). \
            reshape(frame.frameInfo.height, frame.frameInfo.width, 3)

    cv2.imwrite("test.bmp",numpy_image)
    return True

winfun_ctype = WINFUNCTYPE
pFrame = POINTER(IMV_FG_Frame)
FrameInfoCallBack = winfun_ctype(None, pFrame, c_void_p)

def onGetFrame(pFrame, pUSer):

    if pFrame == None:
        print("pFrame is None!")
        return
    Frame = cast(pFrame, POINTER(IMV_FG_Frame)).contents
    print("Get frame blockID = [%d]"%  Frame.frameInfo.blockId)
    if Frame.frameInfo.blockId == 1:
        print("Save image to bmp start...")
        if saveImageToBmp(pUSer,Frame):
            print("Save image to bmp successfully!")
        else:
            print("Save image to bmp failed!")
    return

CALL_BACK_FUN = FrameInfoCallBack(onGetFrame)

# 检查是否需要申请存放转码数据的内存
def mallocConvertBuffer(cam):
    pixelFormatVal = c_uint64(0)
    widthVal = c_int64(0)
    heightVal = c_int64(0)
    nRet = cam.IMV_FG_GetEnumFeatureValue("PixelFormat", pixelFormatVal)
    if IMV_FG_OK != nRet:
        print("Get PixelFormat feature value failed! ErrorCode[%d]", nRet)
        return nRet
    for i in range(0, 256):
        g_colorTable[i].rgbRed = c_char(i)
        g_colorTable[i].rgbBlue = c_char(i)
        g_colorTable[i].rgbGreen = c_char(i)
        g_colorTable[i].rgbReserved = c_char(0)
    if (IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8 == pixelFormatVal) | (IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8 == pixelFormatVal):
        """ // mono8和BGR8裸数据不需要转码 """
        return IMV_FG_OK
    nRet = card.IMV_FG_GetIntFeatureValue("Width", widthVal)
    if IMV_FG_OK != nRet:
        print("Get Width feature value failed! ErrorCode[%d]" % nRet)
        return nRet
    nRet = card.IMV_FG_GetIntFeatureValue("Height", heightVal)
    if IMV_FG_OK != nRet:
        print("Get Height feature value failed! ErrorCode[%d]"% nRet)
        return nRet
    global g_pConvertBuf
    nSize = widthVal.value * heightVal.value * 3
    g_pConvertBuf = (c_ubyte * nSize)()
    return IMV_FG_OK

def displayDeviceInfo(interfaceList,deviceInfoList):
    print("Idx  Type   Vendor              Model           S/N                 DeviceUserID    IP Address")
    print("------------------------------------------------------------------------------------------------")
    for index in range(0,interfaceList.nInterfaceNum):
        interfaceInfo = interfaceList.pInterfaceInfoList[index]
        strType = ""
        strVendorName = ""
        strModeName = ""
        serialNumber = ""
        interfaceName = ""
        for str in interfaceInfo.vendorName:
            strVendorName = strVendorName + chr(str)
        for str in interfaceInfo.modelName:
            strModeName = strModeName + chr(str)
        for str in interfaceInfo.serialNumber:
            serialNumber = serialNumber + chr(str)
        for str in interfaceInfo.interfaceName:
            interfaceName = interfaceName + chr(str)
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
                strVendorName = ""
                strModeName = ""
                strSerialNumber = ""
                strCameraname = ""
                strIpAdress = ""
                for str in pDeviceInfo.vendorName:
                    strVendorName = strVendorName + chr(str)
                for str in pDeviceInfo.modelName:
                    strModeName = strModeName + chr(str)
                for str in pDeviceInfo.serialNumber:
                    strSerialNumber = strSerialNumber + chr(str)
                for str in pDeviceInfo.cameraName:
                    strCameraname = strCameraname + chr(str)
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

if __name__ == "__main__":

    print("SDK Version:", MvCamera.IMV_FG_GetVersion().decode("ascii"))
    card = Capture()
    cam = MvCamera()
    print("Enum capture board interface info.")

    # 枚举采集卡设备
    interfaceList = IMV_FG_INTERFACE_INFO_LIST()
    interfaceTp = IMV_FG_EInterfaceType.typeCLInterface
    nRet = card.IMV_FG_EnumInterface(interfaceTp, interfaceList)
    if (IMV_FG_OK != nRet):
        print("Enumeration devices failed! errorCode:", nRet)
        sys.exit()
    if (interfaceList.nInterfaceNum == 0):
        print("No board device find. board list size:", interfaceList.nInterfaceNum)
        sys.exit()
    print("Enum camera device.")

    # 枚举相机设备
    nInterfacetype = IMV_FG_EInterfaceType.typeCLInterface
    deviceList = IMV_FG_DEVICE_INFO_LIST()
    nRet = card.IMV_FG_EnumDevices(nInterfacetype, deviceList)
    if IMV_FG_OK != nRet:
        print("Enumeration devices failed! ErrorCode", nRet)
        sys.exit()
    if deviceList.nDevNum == 0:
        print("find no device!")
        sys.exit()

    # 打印相机基本信息（序号,类型,制造商信息,型号,序列号,用户自定义ID)
    displayDeviceInfo(interfaceList,deviceList)

    # 选择需要连接的采集卡
    boardIndex = input("Please input the capture index:")
    while (int(boardIndex) > interfaceList.nInterfaceNum):
        boardIndex = input("Input invalid! Please input the capture index:")
    print("Open capture device.")

    # 打开采集卡设备
    nRet = card.IMV_FG_OpenInterface(int(boardIndex) - 1)
    if (IMV_FG_OK != nRet):
        print("Open capture board device failed! errorCode:", nRet)
        sys.exit()

    # 选择需要连接的设备
    cameraIndex = input("Please input the camera index:")
    while (int(cameraIndex) > deviceList.nDevNum):
        cameraIndex = input("Please input the camera index:")
    print("Open camera device.")

    # 打开设备
    nRet = cam.IMV_FG_OpenDevice(IMV_FG_ECreateHandleMode.IMV_FG_MODE_BY_INDEX, byref(c_void_p(int(cameraIndex) - 1)))
    if IMV_FG_OK != nRet:
        print("Open devHandle failed! ErrorCode", nRet)
        sys.exit()

    # 检查是否需要申请存放转码数据的内存
    mallocConvertBuffer(cam)
    if (IMV_FG_OK != nRet):
        print("mallocConvertBuffer falied.")

    # 注册数据帧回调函数
    ret = card.IMV_FG_AttachGrabbing(CALL_BACK_FUN, None)
    if ret != IMV_FG_OK:
        print("Attach grabbing failed! ErrorCode:", ret)
        sys.exit()

    # 开始拉流
    nRet = card.IMV_FG_StartGrabbing()
    if IMV_FG_OK != nRet:
        print("Start grabbing failed! ErrorCode", nRet)
        sys.exit()

    # 拉流2s
    time.sleep(2)

    # 停止拉流
    nRet = card.IMV_FG_StopGrabbing()
    if IMV_FG_OK != nRet:
        print("Stop grabbing failed! ErrorCode", nRet)
        sys.exit()

    # 关闭相机
    if (cam.handle):
        nRet = cam.IMV_FG_CloseDevice()
        if IMV_FG_OK != nRet:
            print("Close camera failed! ErrorCode", nRet)
            sys.exit()

    # 关闭采集卡
    if (card.handle):
        nRet = card.IMV_FG_CloseInterface()
        if IMV_FG_OK != nRet:
            print("Close card failed! ErrorCode", nRet)
            sys.exit()

    print("---Demo end---")