# -- coding: utf-8 --

from platform import node
import sys
import msvcrt
import time
import threading

from ctypes import *

sys.path.append("../MVSDK")
from IMVApi import *


def modifyCameraExposureTime(cam):
    exposureTimeValue = c_double(0.0)
    exposureMinValue = c_double(0)
    exposureMaxValue = c_double(0)

    # 获取属性值
    ret = cam.IMV_GetDoubleFeatureValue("ExposureTime", exposureTimeValue)
    if ret != IMV_OK:
        print("Get feature value failed! ErrorCode:", ret)
        return ret

    print("Before change,exposureTime is ", exposureTimeValue.value)

    # 获取属性可设的最小值
    ret = cam.IMV_GetDoubleFeatureMin("ExposureTime", exposureMinValue)
    if ret != IMV_OK:
        print("Get feature minimum value failed! ErrorCode:", ret)
        return ret

    print("exposureTime settable minimum value is ", exposureMinValue.value)

    # 获取属性可设的最大值
    ret = cam.IMV_GetDoubleFeatureMax("ExposureTime", exposureMaxValue)
    if ret != IMV_OK:
        print("Get feature maximum value failed! ErrorCode:", ret)
        return ret

    print("exposureTIme settable maximum value is ", exposureMaxValue.value)

    if exposureTimeValue.value < (exposureMinValue.value + 2.0):
        exposureTimeValue.value += 2.0
    else:
        exposureTimeValue.value -= 2.0

    # 设置属性值
    ret = cam.IMV_SetDoubleFeatureValue("ExposureTime", exposureTimeValue.value)
    if ret != IMV_OK:
        print("Set feature value failed！ ErrorCode:", ret)
        return ret

    # 获取属性值
    ret = cam.IMV_GetDoubleFeatureValue("ExposureTime", exposureTimeValue)
    if ret != IMV_OK:
        print("Get feature value failed! ErrorCode:", ret)
        return ret

    print("After change, exposureTime is ", exposureTimeValue.value)
    return ret


def modifyCameraWidth(cam):
    widthValue = c_int(0)
    widthMinValue = c_int(0)
    widthMaxValue = c_int(0)
    incrementValue = c_int(0)

    # 获取属性值
    ret = cam.IMV_GetIntFeatureValue("Width", widthValue)
    if ret != IMV_OK:
        print("Get feature value failed! ErrorCode:", ret)
        return ret

    print("Before change , width is ", widthValue.value)

    # 获取属性可设的最小值
    ret = cam.IMV_GetIntFeatureMin("Width", widthMinValue)
    if ret != IMV_OK:
        print("Get feature minimum value failed! ErrorCode:", ret)
        return ret

    print("width settable minimum value is ", widthMinValue.value)

    # 获取属性可设的最大值
    ret = cam.IMV_GetIntFeatureMax("Width", widthMaxValue)
    if ret != IMV_OK:
        print("Get feature maximum value failed! ErrorCode:", ret)
        return ret

    print("width settable maximum value is ", widthMaxValue.value)

    # 获取属性步长
    ret = cam.IMV_GetIntFeatureInc("Width", incrementValue)
    if ret != IMV_OK:
        print("Get feature increment value failed! ErrorCode:", ret)
        return ret

    print("width increment value is ", incrementValue.value)

    if widthValue.value < (widthMinValue.value + incrementValue.value):
        widthValue.value += incrementValue.value
    else:
        widthValue.value -= incrementValue.value

    # 设置属性值
    ret = cam.IMV_SetIntFeatureValue("Width", widthValue.value)
    if ret != IMV_OK:
        print("Set feature value failed!ErrorCode:", ret)
        return ret

    # 获取属性值
    ret = cam.IMV_GetIntFeatureValue("Width", widthValue)
    if ret != IMV_OK:
        print("Get feature value failed! ErrorCode:", ret)
        return ret

    print("After change , width is ", widthValue.value)
    return ret


def modifyCameraReverseX(cam):
    reverseXValue = c_bool(0)

    # 获取属性值
    ret = cam.IMV_GetBoolFeatureValue("ReverseX", reverseXValue)
    if ret != IMV_OK:
        print("Get feature value failed! ErrorCode:", ret)
        return ret

    print("Before change,reverseX is ", reverseXValue.value)

    # 设置属性值
    ret = cam.IMV_SetBoolFeatureValue("ReverseX", not reverseXValue.value)
    if ret != IMV_OK:
        print("Set feature value failed! ErrorCode:", ret)
        return ret

    # 获取属性值
    ret = cam.IMV_GetBoolFeatureValue("ReverseX", reverseXValue)
    if ret != IMV_OK:
        print("Get feature value failed! ErrorCode:", ret)
        return ret

    print("After change , reverseX is ", reverseXValue.value)
    return ret


def modifyCameraDeviceUserID(cam):
    stringValue = IMV_String()

    # 获取属性值
    ret = cam.IMV_GetStringFeatureValue("DeviceUserID", stringValue)
    if ret != IMV_OK:
        print("Get feature value failed! ErrorCode:", ret)
        return ret

    print("Before change , deviceUserID is ", stringValue.str.decode('ascii'))

    # 设置属性值
    ret = cam.IMV_SetStringFeatureValue("DeviceUserID","Camera")
    if ret != IMV_OK:
        print("Set feature value failed!ErrorCode:", ret)
        return ret

    # 获取属性值
    ret = cam.IMV_GetStringFeatureValue("DeviceUserID", stringValue)
    if ret != IMV_OK:
        print("Get feature value failed! ErrorCode:", ret)
        return ret

    print("After change , deviceUserID is ", stringValue.str.decode('ascii'))
    return ret

def modifyCameraTriggerMode(cam):
    enumSymbolValue = IMV_String()

    # 获取属性值
    ret = cam.IMV_GetEnumFeatureSymbol("TriggerMode", enumSymbolValue)
    if ret != IMV_OK:
        print("Get feature value failed! ErrorCode:", ret)
        return ret

    print("Before change , triggerMode is ", enumSymbolValue.str.decode('ascii'))

    # 设置属性值
    ret = cam.IMV_SetEnumFeatureSymbol("TriggerMode", "On")
    if ret != IMV_OK:
        print("Set feature value failed! ErrorCode: ", ret)
        return ret

        # 获取属性值
    ret = cam.IMV_GetEnumFeatureSymbol("TriggerMode",enumSymbolValue)
    if ret != IMV_OK:
        print("Get feature value failed! ErrorCode:", ret)
        return ret

    print("After change , triggerMode is ", enumSymbolValue.str.decode('ascii'))
    return ret

def displayDeviceInfo(deviceInfoList):
    print("Idx  Type   Vendor              Model           S/N                 DeviceUserID    IP Address")
    print("------------------------------------------------------------------------------------------------")
    for i in range(0,deviceInfoList.nDevNum):
        pDeviceInfo=deviceInfoList.pDevInfo[i]
        strType=""
        strVendorName = pDeviceInfo.vendorName.decode("ascii")
        strModeName = pDeviceInfo.modelName.decode("ascii")
        strSerialNumber = pDeviceInfo.serialNumber.decode("ascii")
        strCameraname = pDeviceInfo.cameraName.decode("ascii")
        strIpAdress = pDeviceInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress.decode("ascii")
        if pDeviceInfo.nCameraType == typeGigeCamera:
            strType="Gige"
        elif pDeviceInfo.nCameraType == typeU3vCamera:
            strType="U3V"
        print ("[%d]  %s   %s    %s      %s     %s           %s" % (i+1, strType,strVendorName,strModeName,strSerialNumber,strCameraname,strIpAdress))

if __name__ == "__main__":

    deviceList = IMV_DeviceList()
    interfaceType = IMV_EInterfaceType.interfaceTypeAll

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

    #修改相机曝光时间，通用double型属性访问实例
    if modifyCameraExposureTime(cam) != IMV_OK:
        sys.exit()

    #修改相机像素宽度，通用int型属性访问实例
    if modifyCameraWidth(cam) != IMV_OK:
        sys.exit()

    #修改相机ReverseX,通用bool型属性访问实例
    if modifyCameraReverseX(cam) != IMV_OK:
        sys.exit()

    #修改相机DeviceID，通用string型属性访问实例
    if modifyCameraDeviceUserID(cam) != IMV_OK:
        sys.exit()

    #修改相机TriggerMode，通用enum型属性访问实例
    if modifyCameraTriggerMode(cam) != IMV_OK:
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
