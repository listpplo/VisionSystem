import sys
from Development.Samples.Python.IMV.MVSDK.IMVApi import *
from Development.Samples.Python.IMV.MVSDK.IMVDefines import *
from mainWindow import Ui_MainWindow
from PySide6.QtWidgets import QMainWindow, QApplication
from PySide6.QtGui import QCloseEvent, QImage, QPixmap
import sys
from ctypes import *
import numpy
import cv2
import gc
import datetime
from threading import Thread

sys.path.append("Development\Samples\Python\IMV\MVSDK")

def cv2_image_to_qimage(cv2_img):
    """ Convert an OpenCV image (numpy array) to a QImage. """
    # Convert BGR to RGB
    rgb_image = cv2.cvtColor(cv2_img, cv2.COLOR_BGR2RGB)
    h, w, ch = rgb_image.shape
    bytes_per_line = ch * w
    q_img = QImage(rgb_image.data, w, h, bytes_per_line, QImage.Format.Format_RGB888)
    return q_img

class VisionApp(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.pushButton_2.clicked.connect(self.getCameraInfo)
        self.comboBox.currentIndexChanged.connect(self.loadCameraMetaData)
        self.pushButton.clicked.connect(lambda : Thread(target=self.grabImage).start())
        self.deviceList = IMV_DeviceList()
        self.isGrab = True

    def loadCameraMetaData(self):
          current_ip = self.comboBox.currentText()

          for i in range(0, self.deviceList.nDevNum):
            pDeviceInfo = self.deviceList.pDevInfo[i]
            strIpAdress = pDeviceInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress.decode("ascii")
            if strIpAdress == current_ip:
                if pDeviceInfo.nCameraType == typeGigeCamera:
                    strType = "Gige"
                elif pDeviceInfo.nCameraType == typeU3vCamera:
                    strType = "U3V"
                self.lineEdit.setText(strType)

            self.lineEdit_2.setText(pDeviceInfo.modelName.decode("ascii"))
            self.lineEdit_3.setText(pDeviceInfo.serialNumber.decode("ascii"))            

    def grabImage(self):
        cam=MvCamera()
        # 创建设备句柄
        nRet=cam.IMV_CreateHandle(IMV_ECreateHandleMode.modeByIndex,byref(c_void_p(self.comboBox.currentIndex()-1)))
        if IMV_OK != nRet:
            print("Create devHandle failed! ErrorCode",nRet)
            sys.exit()
            
        # 打开相机
        nRet=cam.IMV_Open()
        if IMV_OK != nRet:
            print("Open devHandle failed! ErrorCode",nRet)
            sys.exit()
        
    # 通用属性设置:设置触发模式为off
        nRet=IMV_OK
        nRet=cam.IMV_SetEnumFeatureSymbol("TriggerSource","Software")
        if IMV_OK != nRet:
            print("Set triggerSource value failed! ErrorCode[%d]" % nRet)
            sys.exit()
            
        nRet=cam.IMV_SetEnumFeatureSymbol("TriggerSelector","FrameStart")
        if IMV_OK != nRet:
            print("Set triggerSelector value failed! ErrorCode[%d]" % nRet)
            sys.exit()

        nRet=cam.IMV_SetEnumFeatureSymbol("TriggerMode","Off")
        if IMV_OK != nRet:
            print("Set triggerMode value failed! ErrorCode[%d]" % nRet)
            sys.exit()

        # 开始拉流
        nRet=cam.IMV_StartGrabbing()
        if IMV_OK != nRet:
            print("Start grabbing failed! ErrorCode",nRet)
            sys.exit()
        
        while self.isGrab :
            # 主动取图
            frame = IMV_Frame()
            stPixelConvertParam=IMV_PixelConvertParam()

            nRet = cam.IMV_GetFrame(frame, 1000)
            
            if  IMV_OK != nRet :
                print("getFrame fail! Timeout:[1000]ms")
                continue
            else:
                print("getFrame success BlockId = [" + str(frame.frameInfo.blockId) + "], get frame time: " + str(datetime.datetime.now()))
            
            if  None==byref(frame) :
                print("pFrame is NULL!")
                continue
            # 给转码所需的参数赋值

            if IMV_EPixelType.gvspPixelMono8==frame.frameInfo.pixelFormat:
                nDstBufSize=frame.frameInfo.width * frame.frameInfo.height
            else:
                nDstBufSize=frame.frameInfo.width * frame.frameInfo.height*3
            
            pDstBuf=(c_ubyte*nDstBufSize)()
            memset(byref(stPixelConvertParam), 0, sizeof(stPixelConvertParam))
            
            stPixelConvertParam.nWidth = frame.frameInfo.width
            stPixelConvertParam.nHeight = frame.frameInfo.height
            stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat
            stPixelConvertParam.pSrcData = frame.pData
            stPixelConvertParam.nSrcDataLen = frame.frameInfo.size
            stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX
            stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY
            stPixelConvertParam.eBayerDemosaic = IMV_EBayerDemosaic.demosaicNearestNeighbor
            stPixelConvertParam.eDstPixelFormat = frame.frameInfo.pixelFormat
            stPixelConvertParam.pDstBuf = pDstBuf
            stPixelConvertParam.nDstBufSize = nDstBufSize
            
            # 释放驱动图像缓存
            # release frame resource at the end of use
            
            nRet = cam.IMV_ReleaseFrame(frame)
            if IMV_OK != nRet:
                print("Release frame failed! ErrorCode[%d]\n", nRet)
                sys.exit()
            
            # 如果图像格式是 Mono8 直接使用
            # no format conversion required for Mono8
            if stPixelConvertParam.ePixelFormat == IMV_EPixelType.gvspPixelMono8:
                imageBuff=stPixelConvertParam.pSrcData
                userBuff = c_buffer(b'\0', stPixelConvertParam.nDstBufSize)
            
                memmove(userBuff,imageBuff,stPixelConvertParam.nDstBufSize)
                grayByteArray = bytearray(userBuff)
                
                cvImage = numpy.array(grayByteArray).reshape(stPixelConvertParam.nHeight, stPixelConvertParam.nWidth)
                cv2.imshow("Test",cvImage)
                if (cv2.waitKey(1) >= 0):
                    break
                
            else:
                # 转码 => BGR24
                # convert to BGR24
                stPixelConvertParam.eDstPixelFormat=IMV_EPixelType.gvspPixelBGR8
                #stPixelConvertParam.nDstBufSize=nDstBufSize

                nRet=cam.IMV_PixelConvert(stPixelConvertParam)
                if IMV_OK!=nRet:
                    print("image convert to failed! ErrorCode[%d]" % nRet)
                    del pDstBuf
                    sys.exit()
                rgbBuff = c_buffer(b'\0', stPixelConvertParam.nDstBufSize)
                memmove(rgbBuff,stPixelConvertParam.pDstBuf,stPixelConvertParam.nDstBufSize)
                colorByteArray = bytearray(rgbBuff)
                cvImage = numpy.array(colorByteArray).reshape(stPixelConvertParam.nHeight, stPixelConvertParam.nWidth, 3)
                cvImage = cv2.resize(cvImage, (720, 480))
                self.label.setPixmap(QPixmap.fromImage(cv2_image_to_qimage(cvImage)))
                if None!=pDstBuf:
                    del pDstBuf
                    pass
                    
        
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

    def getCameraInfo(self):
        self.comboBox.clear()
        self.comboBox.addItem("")
        interfaceType = IMV_EInterfaceType.interfaceTypeAll
        nWidth=c_uint()
        nHeight=c_uint()

        nRet=MvCamera.IMV_EnumDevices(self.deviceList,interfaceType)

        if nRet != IMV_OK:
            print("Enumrate Device Failed")
            return
        
        if self.deviceList.nDevNum == 0:
            print("No Device Found !!")
            return
        
        for i in range(0, self.deviceList.nDevNum):
            pDeviceInfo = self.deviceList.pDevInfo[i]
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
            self.comboBox.addItem(strIpAdress)
    
    def closeEvent(self, event: QCloseEvent) -> None:
        self.isGrab = False
        return super().closeEvent(event)
        
if __name__ == "__main__":
    app = QApplication()
    window = VisionApp()
    window.show()
    sys.exit(app.exec())