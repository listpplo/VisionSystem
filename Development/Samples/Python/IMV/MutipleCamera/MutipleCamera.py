# -- coding: utf-8 --

from Device import *

if __name__ == "__main__":

    SysDevice = DeviceSystem()
    SysDevice.initSystem()

    cameraIndex = input("Please input the camera index: ")
    cameraIndex = int(cameraIndex)-1
    print(SysDevice.m_DeviceNum)
    if int(cameraIndex) > SysDevice.m_DeviceNum:
        print("intput error!")
        sys.exit()

    for i in "1":
        ret = SysDevice.m_Device[cameraIndex].openDevicebyKey()
        if ret != IMV_OK:
            print("open camera[%d] failed[%d]"% (cameraIndex, ret))
            break

        width = c_int(0)
        ret = SysDevice.m_Device[cameraIndex].getIntValue("Width", width)
        if ret != IMV_OK:
            print("getIntValue camera[%d] failed[%d]"% (cameraIndex, ret))
            break

        ret = SysDevice.m_Device[cameraIndex].setIntValue("Width", width.value)
        if ret != IMV_OK:
            print("setIntValue camera[%d] failed[%d]"% (cameraIndex, ret))
            break

        ret = SysDevice.m_Device[cameraIndex].startGrabbing()
        if ret != IMV_OK:
            print("start grabbing camera[%d] failed[%d]"% (cameraIndex, ret))
            break

        time.sleep(3)
        ret = SysDevice.m_Device[cameraIndex].stopGrabbing()
        if ret != IMV_OK:
            print("stop grabbing camera[%d] failed[%d]"% (cameraIndex, ret))
            break

        print("start callback grab.")

        ret = SysDevice.m_Device[cameraIndex].startGrabbingCallback()
        if ret != IMV_OK:
            print("start callback grabbing camera[%d] failed[%d]"% (cameraIndex, ret))
            break

        time.sleep(3)
        ret = SysDevice.m_Device[cameraIndex].stopGrabbingCallback()
        if ret != IMV_OK:
            print("stop callback grabbing camera[%d] failed[%d]"% (cameraIndex, ret))
            break

    SysDevice.m_Device[cameraIndex].closeDevice()
    SysDevice.unInitSystem()

    print("---Demo end---")