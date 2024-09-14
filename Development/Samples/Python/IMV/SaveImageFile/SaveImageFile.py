# -- coding: utf-8 --

import sys

sys.path.append("../MVSDK")
from IMVApi import *
import time


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
        print("[%d]  %s   %s    %s      %s     %s           %s" % (
        i + 1, strType, strVendorName, strModeName, strSerialNumber, strCameraname, strIpAdress))


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
        print("select ", IMV_ESaveType.typeImageBmp)
        return IMV_ESaveType.typeImageBmp
    elif int(inputstr) == 1:
        print("select typeImageJpeg", IMV_ESaveType.typeImageJpeg)
        return IMV_ESaveType.typeImageJpeg
    elif int(inputstr) == 2:
        print("select typeImagePng", IMV_ESaveType.typeImagePng)
        return IMV_ESaveType.typeImagePng
    elif int(inputstr) == 3:
        print("select typeImageTif", IMV_ESaveType.typeImageTif)
        return IMV_ESaveType.typeImageTif
    else:
        print("select typeImageBmp", IMV_ESaveType.typeImageBmp)
        return IMV_ESaveType.typeImageBmp

if __name__ == "__main__":
    deviceList = IMV_DeviceList()
    interfaceType = IMV_EInterfaceType.interfaceTypeAll
    frame = IMV_Frame()

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

    # 打开相机
    nRet = cam.IMV_Open()
    if IMV_OK != nRet:
        print("Open devHandle failed! ErrorCode", nRet)
        sys.exit()

    # 开始拉流
    nRet = cam.IMV_StartGrabbing()
    if IMV_OK != nRet:
        print("Start grabbing failed! ErrorCode", nRet)
        sys.exit()

    # 取一帧
    nRet = cam.IMV_GetFrame(frame, 500)
    if IMV_OK != nRet:
        print("Get frame failed!ErrorCode[%d]" % nRet)
        sys.exit()
    saveFormat = selectSaveFormat()

    saveImageToFileParam = IMV_SaveImageToFileParam()
    saveImageToFileParam.eImageType = saveFormat
    saveImageToFileParam.nWidth = frame.frameInfo.width
    saveImageToFileParam.nHeight = frame.frameInfo.height
    saveImageToFileParam.nPixelFormat = frame.frameInfo.pixelFormat
    saveImageToFileParam.pSrcData = frame.pData
    saveImageToFileParam.nSrcDataLen = frame.frameInfo.size
    saveImageToFileParam.nBayerDemosaic = 2
    saveImageToFileParam.pImagePath = c_char_p(0)

    if IMV_ESaveType.typeImageBmp == saveImageToFileParam.eImageType:
        saveImageToFileParam.pImagePath = "Image.bmp".encode("ascii")
    elif IMV_ESaveType.typeImageJpeg == saveImageToFileParam.eImageType:
        saveImageToFileParam.nQuality = 90
        saveImageToFileParam.pImagePath = "Image.jpg".encode("ascii")
    elif IMV_ESaveType.typeImagePng == saveImageToFileParam.eImageType:
        saveImageToFileParam.nQuality = 8
        saveImageToFileParam.pImagePath = "Image.png".encode("ascii")
    elif IMV_ESaveType.typeImageTif == saveImageToFileParam.eImageType:
        saveImageToFileParam.pImagePath = "Image.tif".encode("ascii")

    print("start saveImage", saveImageToFileParam.eImageType)
    nRet = cam.IMV_SaveImageToFile(saveImageToFileParam)
    if IMV_OK != nRet:
        print("IMV_SaveImageToFile failed! ErrorCode", nRet)
        sys.exit()

    nRet = cam.IMV_ReleaseFrame(frame)
    if IMV_OK != nRet:
        print("IMV_ReleaseFrame failed! ErrorCode", nRet)
        sys.exit()

        # 停止拉流
    nRet = cam.IMV_StopGrabbing()
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

    print("---Demo end---")
