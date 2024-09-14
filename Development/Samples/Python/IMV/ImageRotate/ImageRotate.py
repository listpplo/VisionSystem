# -- coding: utf-8 --

import sys
from ctypes import *

sys.path.append("../MVSDK")
from IMVApi import *

MONO_CHANNEL_NUM=1
RGB_CHANNEL_NUM=3
BGR_CHANNEL_NUM=3

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

def selectRotationAngle():
    rotationAngleCnt=3
    print("--------------------------------------------")
    print("\t0.Image rotation 90 degree angle")
    print("\t1.Image rotation 180 degree angle")
    print("\t2.Image rotation 270 degree angle")
    print("--------------------------------------------")
    inputIndex = input("Please select the rotation angle index: ")

    if int(inputIndex) >=rotationAngleCnt|int(inputIndex)<0:
        print ("intput error!")
        return IMV_INVALID_PARAM

    return  int(inputIndex)
    
def rotateImage(cam,frame,rotationAngle):
    stPixelConvertParam=IMV_PixelConvertParam()
    stRotateImageParam=IMV_RotateImageParam()
    pConvertBuf=None
    nChannelNum=0
    
    memset(byref(stRotateImageParam),0,sizeof(stRotateImageParam))
    if IMV_EPixelType.gvspPixelMono8==frame.frameInfo.pixelFormat:
        stRotateImageParam.pSrcData = frame.pData
        stRotateImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * MONO_CHANNEL_NUM
        stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat
        nChannelNum = MONO_CHANNEL_NUM
    elif IMV_EPixelType.gvspPixelBGR8==frame.frameInfo.pixelFormat:
        stRotateImageParam.pSrcData = frame.pData
        stRotateImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * BGR_CHANNEL_NUM
        stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat
        nChannelNum = BGR_CHANNEL_NUM
    elif IMV_EPixelType.gvspPixelRGB8==frame.frameInfo.pixelFormat:
        stRotateImageParam.pSrcData = frame.pData
        stRotateImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * RGB_CHANNEL_NUM
        stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat
        nChannelNum = RGB_CHANNEL_NUM

    #MONO8/RGB24/BGR24以外的格式都转化成BGR24
    else:
        nConvertBufSize = frame.frameInfo.width * frame.frameInfo.height * BGR_CHANNEL_NUM;
        pConvertBuf=(c_ubyte * nConvertBufSize)()
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
        stPixelConvertParam.pDstBuf = pConvertBuf
        stPixelConvertParam.nDstBufSize = nConvertBufSize

        nRet=cam.IMV_PixelConvert(stPixelConvertParam)
        if IMV_OK==nRet:
            stRotateImageParam.pSrcData = pConvertBuf
            stRotateImageParam.nSrcDataLen = stPixelConvertParam.nDstDataLen
            stRotateImageParam.ePixelFormat = IMV_EPixelType.gvspPixelBGR8
            nChannelNum = BGR_CHANNEL_NUM
        else:
            stRotateImageParam.pSrcData=None
            print("image convert to BGR8 failed! ErrorCode[%d]", nRet)
    bEnd=True
    while bEnd:
        if None==stRotateImageParam.pSrcData:
            print("stRotateImageParam pSrcData is NULL!")
            break
        nRotateBufSize=frame.frameInfo.width * frame.frameInfo.height * nChannelNum
        pRotateBuf=(c_ubyte*nRotateBufSize)()

        stRotateImageParam.nWidth = frame.frameInfo.width
        stRotateImageParam.nHeight = frame.frameInfo.height
        stRotateImageParam.eRotationAngle = rotationAngle
        stRotateImageParam.pDstBuf = pRotateBuf
        stRotateImageParam.nDstBufSize = nRotateBufSize

        nRet=cam.IMV_RotateImage(stRotateImageParam)

        if IMV_OK == nRet:
            if IMV_ERotationAngle.rotationAngle90 == rotationAngle:
                print("Image rotation angle 90 degree successfully!")
                FileName="rotationAngle90.bin"
                hFile=open(FileName.encode('ascii'), "wb")
            elif IMV_ERotationAngle.rotationAngle180 == rotationAngle:
                print("Image rotation angle 180 degree successfully!")
                FileName="rotationAngle180.bin"
                hFile=open(FileName.encode('ascii'), "wb")
            else:
                print("Image rotation angle 270 degree successfully!")
                FileName="rotationAngle270.bin"
                hFile=open(FileName.encode('ascii'), "wb")

            try:
                img_buff = c_buffer(b'\0', stRotateImageParam.nDstBufSize)
                memmove(img_buff,stRotateImageParam.pDstBuf,stRotateImageParam.nDstBufSize)
                hFile.write(img_buff)
            except:
                print("save file executed failed")
            finally:
                hFile.close() 

        else:
            if IMV_ERotationAngle.rotationAngle90 == rotationAngle:
                print("Image rotation angle 90 degree failed! ErrorCode[%d]", nRet)
            elif IMV_ERotationAngle.rotationAngle180 == rotationAngle:
                print("Image rotation angle 180 degree failed! ErrorCode[%d]", nRet)
            else:
                print("Image rotation angle 270 degree failed! ErrorCode[%d]", nRet)
        if None!=pConvertBuf:
            del pConvertBuf
            pConvertBuf = None
        if None!=pRotateBuf:
            del pRotateBuf
        bEnd=False

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
    
    # 取一帧图像
    nRet=cam.IMV_GetFrame(frame,500)
    if IMV_OK!=nRet:
        print("Get frame failed!ErrorCode[%d]" % nRet)
        sys.exit()

    # 选择图像旋转角度
    imageRotationAngle=selectRotationAngle()

    print("BlockId (%d) pixelFormat (%d), Start image rotate..." % (frame.frameInfo.blockId,frame.frameInfo.pixelFormat))

    # 图片转化
    rotateImage(cam,frame,imageRotationAngle)

    # 释放图像缓存
    nRet=cam.IMV_ReleaseFrame(frame)
    if IMV_OK!=nRet:
        print("Release frame failed!Errorcode[%d]" % nRet)
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