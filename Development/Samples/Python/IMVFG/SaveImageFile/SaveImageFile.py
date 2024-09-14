# -- coding: utf-8 --

import time
import threading
import sys

sys.path.append("../MVSDK")
from IMVFGApi import *

def displayDeviceInfo(interfaceList,deviceInfoList):
    print("Idx  Type   Vendor              Model           S/N                 DeviceUserID    IP Address")
    print("------------------------------------------------------------------------------------------------")
    for index in range(0,interfaceList.nInterfaceNum):
        interfaceInfo = interfaceList.pInterfaceInfoList[index]
        strType = ""
        strVendorName = interfaceInfo.vendorName.decode("ascii")
        strModeName = interfaceInfo.modelName.decode("ascii")
        serialNumber = interfaceInfo.serialNumber.decode("ascii")
        interfaceName = interfaceInfo.interfaceName.decode("ascii")
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
                strIpAdress = ""
                strVendorName = pDeviceInfo.vendorName.decode("ascii")
                strModeName = pDeviceInfo.modelName.decode("ascii")
                strSerialNumber = pDeviceInfo.serialNumber.decode("ascii")
                strCameraname = pDeviceInfo.cameraName.decode("ascii")
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

def selectSaveFormat():
    print("--------------------------------------------")
    print("0.Save to BMP")
    print("1.Save to Jpeg")
    print("2.Save to Png")
    print("3.Save to Tif")
    print("--------------------------------------------")
    inputstr = input("Please select the save format index: ")

    while True:
        if 0 <= int(inputstr) <= 3:
            break
        inputstr = input("Input invalid! Please select the save format index: ")

    if int(inputstr) == 0:
        print("select ", IMV_FG_ESaveType.typeImageBmp)
        return IMV_FG_ESaveType.typeImageBmp
    elif int(inputstr) == 1:
        print("select typeImageJpeg", IMV_FG_ESaveType.typeImageJpeg)
        return IMV_FG_ESaveType.typeImageJpeg
    elif int(inputstr) == 2:
        print("select typeImagePng", IMV_FG_ESaveType.typeImagePng)
        return IMV_FG_ESaveType.typeImagePng
    elif int(inputstr) == 3:
        print("select typeImageTif", IMV_FG_ESaveType.typeImageTif)
        return IMV_FG_ESaveType.typeImageTif
    else:
        print("select typeImageBmp", IMV_FG_ESaveType.typeImageBmp)
        return IMV_FG_ESaveType.typeImageBmp

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
    nRet = card.IMV_FG_OpenInterface(int(boardIndex)-1)
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

    # 取一帧
    frame = IMV_FG_Frame()
    nRet = card.IMV_FG_GetFrame(frame, 5000)
    if IMV_FG_OK != nRet:
        print("Get frame failed!ErrorCode[%d]" % nRet)
        sys.exit()
    saveFormat = selectSaveFormat()

    saveImageToFileParam = IMV_FG_SaveImageToFileParam()
    saveImageToFileParam.eImageType = saveFormat
    saveImageToFileParam.nWidth = frame.frameInfo.width
    saveImageToFileParam.nHeight = frame.frameInfo.height
    saveImageToFileParam.nPixelFormat = frame.frameInfo.pixelFormat
    saveImageToFileParam.pSrcData = frame.pData
    saveImageToFileParam.nSrcDataLen = frame.frameInfo.size
    saveImageToFileParam.nBayerDemosaic = 2
    saveImageToFileParam.pImagePath = c_char_p(0)

    if IMV_FG_ESaveType.typeImageBmp == saveImageToFileParam.eImageType:
        saveImageToFileParam.pImagePath = "Image.bmp".encode("ascii")
    elif IMV_FG_ESaveType.typeImageJpeg == saveImageToFileParam.eImageType:
        saveImageToFileParam.nQuality = 90
        saveImageToFileParam.pImagePath = "Image.jpg".encode("ascii")
    elif IMV_FG_ESaveType.typeImagePng == saveImageToFileParam.eImageType:
        saveImageToFileParam.nQuality = 8
        saveImageToFileParam.pImagePath = "Image.png".encode("ascii")
    elif IMV_FG_ESaveType.typeImageTif == saveImageToFileParam.eImageType:
        saveImageToFileParam.pImagePath = "Image.tif".encode("ascii")

    print("start saveImage", saveImageToFileParam.eImageType)
    nRet = card.IMV_FG_SaveImageToFile(saveImageToFileParam)
    if IMV_FG_OK != nRet:
        print("IMV_FG_SaveImageToFile failed! ErrorCode", nRet)
        sys.exit()

    nRet = card.IMV_FG_ReleaseFrame(frame)
    if IMV_FG_OK != nRet:
        print("IMV_FG_ReleaseFrame failed! ErrorCode", nRet)
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