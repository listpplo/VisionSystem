from concurrent.futures import ThreadPoolExecutor, Future
import functools
from Development.Samples.Python.IMV.MVSDK.IMVApi import *
from Development.Samples.Python.IMV.MVSDK.IMVDefines import *
import sys
import cv2
from PySide6.QtWidgets import QWidget
from PySide6.QtGui import QImage
from UI_Main.mesgPopUp import Ui_MesgWindow
import numpy

# Define the decorator
def run_in_thread(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        # Define a function to run in a thread
        def thread_target():
            return func(*args, **kwargs)
        
        # Create a ThreadPoolExecutor
        with ThreadPoolExecutor(max_workers=1) as executor:
            # Submit the function to the thread pool
            future: Future = executor.submit(thread_target)
            
            # Wait for the result and return it
            return future.result()
    
    return wrapper

# Function to Convert opencv image to qimage
def cv2_image_to_qimage(cv2_img: any):
    """ Convert an OpenCV image (numpy array) to a QImage. """
    # Convert BGR to RGB
    rgb_image = cv2.cvtColor(cv2_img, cv2.COLOR_BGR2RGB)
    h, w, ch = rgb_image.shape
    bytes_per_line = ch * w
    q_img = QImage(rgb_image.data, w, h, bytes_per_line, QImage.Format.Format_RGB888)
    return q_img

# Function To get image from camera
@run_in_thread
def getImageFromCamera(cameraIndex: int, imageSize:tuple[int, int] = [None, None]) -> QImage:
    """ This method is responsible for image aquring through the irayple gigi cam """
    global height_saved, width_saved
    # From Here I have copy pasted the code good luck ... :)
    # Creating Camera Object
    deviceList = IMV_DeviceList()
    interfaceType = IMV_EInterfaceType.interfaceTypeAll
    nWidth=c_uint()
    nHeight=c_uint()

    nRet=MvCamera.IMV_EnumDevices(deviceList,interfaceType)
    cam=MvCamera()
    nRet=cam.IMV_CreateHandle(IMV_ECreateHandleMode.modeByIndex,byref(c_void_p(cameraIndex-1)))
    # Opening the camera object
    nRet=cam.IMV_Open()
    Ret=cam.IMV_SetEnumFeatureSymbol("TriggerSource","Software")
    nRet=cam.IMV_SetEnumFeatureSymbol("TriggerSelector","FrameStart")
    nRet=cam.IMV_SetEnumFeatureSymbol("TriggerMode","Off")
    nRet=cam.IMV_StartGrabbing()
    # Creating a Frame Object
    frame = IMV_Frame()
    stPixelConvertParam=IMV_PixelConvertParam()
    # Retriveing the Frame
    nRet = cam.IMV_GetFrame(frame, 1000)
    if  None==byref(frame) :
        print("pFrame is NULL!")
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

    nRet = cam.IMV_ReleaseFrame(frame)
    if IMV_OK != nRet:
        print("Release frame failed! ErrorCode[%d]\n", nRet)
    
    if stPixelConvertParam.ePixelFormat == IMV_EPixelType.gvspPixelMono8:
        imageBuff=stPixelConvertParam.pSrcData
        userBuff = c_buffer(b'\0', stPixelConvertParam.nDstBufSize)
    
        memmove(userBuff,imageBuff,stPixelConvertParam.nDstBufSize)
        grayByteArray = bytearray(userBuff)
        
        cvImage = numpy.array(grayByteArray).reshape(stPixelConvertParam.nHeight, stPixelConvertParam.nWidth)
    else:
        # convert to BGR24
        stPixelConvertParam.eDstPixelFormat=IMV_EPixelType.gvspPixelBGR8
        #stPixelConvertParam.nDstBufSize=nDstBufSize
        nRet=cam.IMV_PixelConvert(stPixelConvertParam)
        if IMV_OK!=nRet:
            print("image convert to failed! ErrorCode[%d]" % nRet)
            del pDstBuf
        rgbBuff = c_buffer(b'\0', stPixelConvertParam.nDstBufSize)
        memmove(rgbBuff,stPixelConvertParam.pDstBuf,stPixelConvertParam.nDstBufSize)
        colorByteArray = bytearray(rgbBuff)
        cvImage = numpy.array(colorByteArray).reshape(stPixelConvertParam.nHeight, stPixelConvertParam.nWidth, 3)
        if None!=pDstBuf:
            del pDstBuf
            pass

        nRet=cam.IMV_StopGrabbing()
        if IMV_OK != nRet:
            print("Stop grabbing failed! ErrorCode",nRet)
            sys.exit()
        
        nRet=cam.IMV_Close()
        if IMV_OK != nRet:
            print("Close camera failed! ErrorCode",nRet)
            sys.exit()
        
        if(cam.handle):
            nRet=cam.IMV_DestroyHandle()
    
    # Deploying the global Size 
    if imageSize == (None, None):
       return cv2.resize(cvImage, (height_saved, width_saved))
    else:
        return cv2.resize(cvImage, imageSize)

# This class deals with the popup Messages
class popUpMesg(QWidget, Ui_MesgWindow):
    def __init__(self) -> None:
        """This class is responsible for the popup messages"""
        super().__init__()
        self.setupUi(self)
        self.buttonBox.accepted.connect(lambda : self.close())
    
    def makePopUp(self, heading:str, mesg:str) -> None:
        self.MesgHeading.setText(heading)
        self.Mesg.setText(mesg)
        self.show()
