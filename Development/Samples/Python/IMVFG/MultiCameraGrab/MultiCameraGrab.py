# -- coding: utf-8 --

from Device import *

if __name__ == "__main__":

    SysDevice = DeviceSystem()
    SysDevice.initSystem()

    boardIndex = input("Please input the camera index: ")
    boardIndex = int(boardIndex)-1
    if int(boardIndex) >= SysDevice.m_cardDeviceNum:
        print("intput error!")
        sys.exit()

    for i in "0":
        ret = SysDevice.m_cardDevice[boardIndex].openDevicebyKey()
        if ret != IMV_FG_OK:
            print("open camera[%d] failed[%d]"% (boardIndex, ret))
            break

        ret = SysDevice.m_cardDevice[boardIndex].clCamera.openDevicebyKey()
        if ret != IMV_FG_OK:
            print("open camera[%d] failed[%d]" % (boardIndex, ret))
            break

        width = c_int64(0)
        ret = SysDevice.m_cardDevice[boardIndex].getIntValue("Width", width)
        if ret != IMV_FG_OK:
            print("getIntValue camera[%d] failed[%d]"% (boardIndex, ret))
            break

        ret = SysDevice.m_cardDevice[boardIndex].clCamera.setIntValue("Width", width.value)
        if ret != IMV_FG_OK:
            print("setIntValue camera[%d] failed[%d]"% (boardIndex, ret))
            break

        ret = SysDevice.m_cardDevice[boardIndex].startGrabbing()
        if ret != IMV_FG_OK:
            print("start grabbing camera[%d] failed[%d]"% (boardIndex, ret))
            break

        time.sleep(3)
        ret = SysDevice.m_cardDevice[boardIndex].stopGrabbing()
        if ret != IMV_FG_OK:
            print("stop grabbing camera[%d] failed[%d]"% (boardIndex, ret))
            break

        print("start callback grab.")

        ret = SysDevice.m_cardDevice[boardIndex].startGrabbingCallback()
        if ret != IMV_FG_OK:
            print("start callback grabbing camera[%d] failed[%d]"% (boardIndex, ret))
            break

        time.sleep(3)
        ret = SysDevice.m_cardDevice[boardIndex].stopGrabbingCallback()
        if ret != IMV_FG_OK:
            print("stop callback grabbing camera[%d] failed[%d]"% (boardIndex, ret))
            break

    SysDevice.m_cardDevice[boardIndex].clCamera.closeDevice()
    SysDevice.m_cardDevice[boardIndex].closeDevice()
    SysDevice.unInitSystem()

    print("---Demo end---")