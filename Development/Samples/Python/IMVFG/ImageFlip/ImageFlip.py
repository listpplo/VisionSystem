# -- coding: utf-8 --

import sys

sys.path.append("../MVSDK")
from IMVFGApi import *

MONO_CHANNEL_NUM=1
RGB_CHANNEL_NUM=3
BGR_CHANNEL_NUM=3

def selectImageFlipType():
    imageFlipTypeCnt = 2
    print("--------------------------------------------")
    print("\t0.Image Vertical Flip")
    print("\t1.Image Horizontal Flip")
    print("--------------------------------------------")
    inputIndex = input("Please select the flip type index: ")

    if int(inputIndex) >= imageFlipTypeCnt | int(inputIndex) < 0:
        print("intput error!")
        return IMV_FG_INVALID_PARAM

    return int(inputIndex)

def imageFlip(card, frame, imageFlipType):
    stPixelConvertParam = IMV_FG_PixelConvertParam()
    stFlipImageParam = IMV_FG_FlipImageParam()
    pConvertBuf = None
    nChannelNum = 0

    memset(byref(stFlipImageParam), 0, sizeof(stFlipImageParam))
    if IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8 == frame.frameInfo.pixelFormat:
        stFlipImageParam.pSrcData = frame.pData
        stFlipImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * MONO_CHANNEL_NUM
        stFlipImageParam.ePixelFormat = frame.frameInfo.pixelFormat
        nChannelNum = MONO_CHANNEL_NUM
    elif IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8 == frame.frameInfo.pixelFormat:
        stFlipImageParam.pSrcData = frame.pData
        stFlipImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * BGR_CHANNEL_NUM
        stFlipImageParam.ePixelFormat = frame.frameInfo.pixelFormat
        nChannelNum = BGR_CHANNEL_NUM
    elif IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_RGB8 == frame.frameInfo.pixelFormat:
        stFlipImageParam.pSrcData = frame.pData
        stFlipImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * RGB_CHANNEL_NUM
        stFlipImageParam.ePixelFormat = frame.frameInfo.pixelFormat
        nChannelNum = RGB_CHANNEL_NUM

    # MONO8/RGB24/BGR24以外的格式都转化成BGR24
    else:
        nConvertBufSize = frame.frameInfo.width * frame.frameInfo.height * BGR_CHANNEL_NUM;
        pConvertBuf = (c_ubyte * nConvertBufSize)()
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
        stPixelConvertParam.pDstBuf = pConvertBuf
        stPixelConvertParam.nDstBufSize = nConvertBufSize

        nRet = card.IMV_FG_PixelConvert(stPixelConvertParam)
        if IMV_FG_OK == nRet:
            stFlipImageParam.pSrcData = pConvertBuf
            stFlipImageParam.nSrcDataLen = stPixelConvertParam.nDstDataLen
            stFlipImageParam.ePixelFormat = IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8
            nChannelNum = BGR_CHANNEL_NUM
        else:
            stFlipImageParam.pSrcData = None
            print("image convert to BGR8 failed! ErrorCode[%d]", nRet)
    bEnd = True
    while bEnd:
        if None == stFlipImageParam.pSrcData:
            print("stFlipImageParam pSrcData is NULL!")
            break
        nFlipBufSize = frame.frameInfo.width * frame.frameInfo.height * nChannelNum
        pFlipBuf = (c_ubyte * nFlipBufSize)()

        stFlipImageParam.nWidth = frame.frameInfo.width
        stFlipImageParam.nHeight = frame.frameInfo.height
        stFlipImageParam.eFlipType = imageFlipType
        stFlipImageParam.pDstBuf = pFlipBuf
        stFlipImageParam.nDstBufSize = nFlipBufSize

        nRet = card.IMV_FG_FlipImage(stFlipImageParam)

        if IMV_FG_OK == nRet:
            if IMV_FG_EFlipType.IMV_FG_TYPE_FLIP_VERTICAL == imageFlipType:
                print("Image vertical flip successfully!")
                FileName = "verticalFlip.bin"
                hFile = open(FileName.encode('ascii'), "wb")
            else:
                print("Image horizontal flip successfully!")
                FileName = "horizontalFlip.bin"
                hFile = open(FileName.encode('ascii'), "wb")

            try:
                img_buff = (c_ubyte * stFlipImageParam.nDstBufSize)()
                cdll.msvcrt.memcpy(byref(img_buff), stFlipImageParam.pDstBuf, stFlipImageParam.nDstBufSize)
                hFile.write(img_buff)
            except:
                print("save file executed failed")
            finally:
                hFile.close()

        else:
            if IMV_FG_EFlipType.IMV_FG_TYPE_FLIP_VERTICAL == imageFlipType:
                print("Image vertical flip failed! ErrorCode[%d]", nRet)
            else:
                print("Image horizontal flip failed! ErrorCode[%d]", nRet)
            if None != pConvertBuf:
                del pConvertBuf
                pConvertBuf = None
            if None != pFlipBuf:
                del pFlipBuf
        bEnd = False

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
    displayDeviceInfo(interfaceList, deviceList)

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

    # 开始拉流
    nRet = card.IMV_FG_StartGrabbing()
    if IMV_FG_OK != nRet:
        print("Start grabbing failed! ErrorCode", nRet)
        sys.exit()

    frame = IMV_FG_Frame()
    # 取一帧图像
    nRet=card.IMV_FG_GetFrame(frame,500)
    if IMV_FG_OK!=nRet:
        print("Get frame failed!ErrorCode[%d]" % nRet)
        sys.exit()

    # 选择图像翻转方式
    imageFlipType=selectImageFlipType()

    print("BlockId (%d) pixelFormat (%d), Start image flip..." % (frame.frameInfo.blockId,frame.frameInfo.pixelFormat))

    # 图片转化
    imageFlip(card,frame,imageFlipType)

    # 释放图像缓存
    nRet=card.IMV_FG_ReleaseFrame(frame)
    if IMV_FG_OK!=nRet:
        print("Release frame failed!Errorcode[%d]" % nRet)
        sys.exit()

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