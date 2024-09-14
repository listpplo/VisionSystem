# -- coding: utf-8 --

import sys
from ctypes import *

sys.path.append("../MVSDK")
from IMVApi import *

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

def selectConvertFormat():
    convertFormatCnt=4
    print("--------------------------------------------")
    print("\t0.Convert to mono8")
    print("\t1.Convert to RGB24")
    print("\t2.Convert to BGR24")
    print("\t3.Convert to BGRA32")
    print("--------------------------------------------")
    inputIndex = input("Please input convert pixelformat: ")

    if int(inputIndex) > convertFormatCnt|int(inputIndex)<0:
        print ("intput error!")
        return IMV_INVALID_PARAM
    inputIndex=int(inputIndex)
    if 0==inputIndex:
        convertFormat=IMV_EPixelType.gvspPixelMono8
    elif 1==inputIndex:
        convertFormat=IMV_EPixelType.gvspPixelRGB8
    elif 2==inputIndex:
        convertFormat=IMV_EPixelType.gvspPixelBGR8
    elif 3==inputIndex:
        convertFormat=IMV_EPixelType.gvspPixelBGRA8
    else:
        convertFormat=IMV_EPixelType.gvspPixelMono8
    
    return convertFormat
    
def imageConvert(cam,frame,convertFormat):
    stPixelConvertParam=IMV_PixelConvertParam()
    
    if IMV_EPixelType.gvspPixelRGB8==convertFormat:
        nDstBufSize=frame.frameInfo.width * frame.frameInfo.height * 3
        FileName="convertRGB8.bin"
        pConvertFormatStr="RGB8"
    elif IMV_EPixelType.gvspPixelBGR8==convertFormat:
        nDstBufSize=frame.frameInfo.width * frame.frameInfo.height * 3
        FileName="convertBGR8.bin"
        pConvertFormatStr="BGR8"
    elif IMV_EPixelType.gvspPixelBGRA8==convertFormat:
        nDstBufSize=frame.frameInfo.width * frame.frameInfo.height * 4
        FileName="convertBGRA8.bin"
        pConvertFormatStr="BGRA8"
    else:
        nDstBufSize=frame.frameInfo.width * frame.frameInfo.height
        FileName="convertMono8.bin"
        pConvertFormatStr="Mono8"

    pDstBuf=(c_ubyte * nDstBufSize)()
    memset(byref(stPixelConvertParam), 0, sizeof(stPixelConvertParam))
    stPixelConvertParam.nWidth = frame.frameInfo.width
    stPixelConvertParam.nHeight = frame.frameInfo.height
    stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat
    stPixelConvertParam.pSrcData = frame.pData
    stPixelConvertParam.nSrcDataLen = frame.frameInfo.size
    stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX
    stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY
    stPixelConvertParam.eBayerDemosaic = IMV_EBayerDemosaic.demosaicNearestNeighbor
    stPixelConvertParam.eDstPixelFormat = convertFormat
    stPixelConvertParam.pDstBuf = pDstBuf
    stPixelConvertParam.nDstBufSize = nDstBufSize

    nRet=cam.IMV_PixelConvert(stPixelConvertParam)
    if IMV_OK==nRet:
        print("image convert to %s successfully! nDstDataLen (%d)" % (pConvertFormatStr,stPixelConvertParam.nDstBufSize))
        hFile=open(FileName.encode('ascii'), "wb+")
        try:
            img_buff = c_buffer(b'\0', stPixelConvertParam.nDstBufSize)
            memmove(img_buff, stPixelConvertParam.pDstBuf, stPixelConvertParam.nDstBufSize)
            hFile.write(img_buff)
        except:
            print("save file executed failed")
        finally:
            hFile.close() 
    else:
        print("image convert to %s failed! ErrorCode[%d]" % (pConvertFormatStr,nRet))
        del pDstBuf
        sys.exit()

    if pDstBuf != None:
        del pDstBuf

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

    # 选择图像转换目标格式
    convertFormat=selectConvertFormat()

    print("BlockId (%d) pixelFormat (%d), Start image convert..." % (frame.frameInfo.blockId,frame.frameInfo.pixelFormat))

    # 图片转化
    imageConvert(cam,frame,convertFormat)

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