# -- coding: utf-8 --

import sys

sys.path.append("../MVSDK")
from IMVFGApi import *

g_isRunThread = True

# 为线程定义一个函数
def frameGrabbingProc(pUserData):
    devHandle = pUserData
    frame = IMV_FG_Frame()
    if None == devHandle:
        return

    while g_isRunThread:
        nRet = card.IMV_FG_GetFrame(frame, 500)
        if IMV_FG_OK != nRet:
            print("Get frame failed!ErrorCode[%d]" % nRet)
            continue
        print("Get frame blockId = [%d]"% frame.frameInfo.blockId)
        nRet = card.IMV_FG_ReleaseFrame(frame)
        if IMV_FG_OK != nRet:
            print("Release frame failed! ErrorCode[%d]" % nRet)
    return

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

def selectConvertFormat():
    print("--------------------------------------------")
    print("\t0.Convert to mono8")
    print("\t1.Convert to RGB24")
    print("\t2.Convert to BGR24")
    print("\t3.Convert to BGRA32")
    print("--------------------------------------------")
    inputIndex = input("Please input convert pixelformat: ")
    while(int(inputIndex) > 3):
        inputIndex = input("Input invalid! Please input convert pixelformat: ")
    inputIndex = int(inputIndex)
    if 0 == inputIndex:
        convertFormat = IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8
    elif 1 == inputIndex:
        convertFormat = IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_RGB8
    elif 2 == inputIndex:
        convertFormat = IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8
    elif 3 == inputIndex:
        convertFormat = IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGRA8
    else:
        convertFormat = IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8
    return convertFormat

def imageConvert(card, frame, convertFormat):

    stPixelConvertParam = IMV_FG_PixelConvertParam()
    if IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8 == convertFormat:
        nDstBufSize = frame.frameInfo.width * frame.frameInfo.height * 3
        FileName = "convertMono8.bin"
        pConvertFormatStr = "Mono8"
    elif IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_RGB8 == convertFormat:
        nDstBufSize = frame.frameInfo.width * frame.frameInfo.height * 3
        FileName = "convertRGB8.bin"
        pConvertFormatStr = "RGB8"
    elif IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGR8 == convertFormat:
        nDstBufSize = frame.frameInfo.width * frame.frameInfo.height * 3
        FileName = "convertBGR8.bin"
        pConvertFormatStr = "BGR8"
    elif IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_BGRA8 == convertFormat:
        nDstBufSize = frame.frameInfo.width * frame.frameInfo.height * 4
        FileName = "convertBGRA8.bin"
        pConvertFormatStr = "BGRA8"
    else:
        nDstBufSize = frame.frameInfo.width * frame.frameInfo.height
        FileName = "convertMono8.bin"
        pConvertFormatStr = "Mono8"

    pDstBuf = (c_ubyte * nDstBufSize)()
    memset(byref(stPixelConvertParam), 0, sizeof(stPixelConvertParam))
    stPixelConvertParam.nWidth = frame.frameInfo.width
    stPixelConvertParam.nHeight = frame.frameInfo.height
    stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat
    stPixelConvertParam.pSrcData = frame.pData
    stPixelConvertParam.nSrcDataLen = frame.frameInfo.size
    stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX
    stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY
    stPixelConvertParam.eBayerDemosaic = IMV_FG_EBayerDemosaic.IMV_FG_DEMOSAIC_NEAREST_NEIGHBOR
    stPixelConvertParam.eDstPixelFormat = convertFormat
    stPixelConvertParam.pDstBuf = pDstBuf
    stPixelConvertParam.nDstBufSize = nDstBufSize

    nRet = card.IMV_FG_PixelConvert(stPixelConvertParam)
    if IMV_FG_OK == nRet:
        print("image convert to %s successfully! nDstDataLen (%d)" % (
        pConvertFormatStr, stPixelConvertParam.nDstBufSize))
        hFile = open(FileName.encode('ascii'), "wb+")
        try:
            img_buff = (c_ubyte * stPixelConvertParam.nDstBufSize)()
            cdll.msvcrt.memcpy(byref(img_buff), stPixelConvertParam.pDstBuf, stPixelConvertParam.nDstBufSize)
            hFile.write(img_buff)
        except:
            print("save file executed failed")
        finally:
            hFile.close()
    else:
        print("image convert to %s failed! ErrorCode[%d]" % (pConvertFormatStr, nRet))
        del pDstBuf
        sys.exit()

    if pDstBuf != None:
        del pDstBuf

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

    # 开始拉流
    nRet = card.IMV_FG_StartGrabbing()
    if IMV_FG_OK != nRet:
        print("Start grabbing failed! ErrorCode", nRet)
        sys.exit()
    frame = IMV_FG_Frame()
    convert = IMV_FG_EPixelType.IMV_FG_PIXEL_TYPE_Mono8

    # 获取一帧图像
    nRet = card.IMV_FG_GetFrame(frame, 500)
    if nRet != IMV_FG_OK:
        print("Get frame failed! ErrorCode:", nRet)
        sys.exit()

    # 选择图像转换目标格式
    convertFormat = selectConvertFormat()
    print("BlockId ", frame.frameInfo.blockId, "pixelFormat ", frame.frameInfo.pixelFormat, "start image convert...")
    imageConvert(card, frame, convertFormat)

    nRet = card.IMV_FG_ReleaseFrame(frame)
    if nRet != IMV_FG_OK:
        print("Release frame failed! ErrorCode ", nRet)
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