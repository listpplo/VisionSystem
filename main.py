from PySide6.QtWidgets import QMainWindow, QApplication, QWidget, QTableWidgetItem
from PySide6.QtGui import QPixmap, QImage, QShowEvent, QCloseEvent
from UI_Main.mainWindow import Ui_VisionApp
from UI_Main.SettingsWindow import Ui_SettingWindow
from TeachWindow import TeachWindow
from UI_Main.createOrModifyRecipe import Ui_CreateorModifyRecipe
from Development.Samples.Python.IMV.MVSDK.IMVApi import *
from Development.Samples.Python.IMV.MVSDK.IMVDefines import *
import sys
import os
from ctypes import *
import numpy
import cv2
import toml
import gc
from usefulFunctions import getImageFromCamera, cv2_image_to_qimage, popUpMesg


# Loading global files
with open("Configuration\GlobalDevice.toml") as file:
    data = toml.load(file)
    height_saved = data["Device"]["height"]
    width_saved = data["Device"]["width"]

# Class Cereate Modify recipe
class RecipeOptions(QWidget, Ui_CreateorModifyRecipe):
    def __init__(self) -> None:
        super().__init__()
        self.setupUi(self)
        self.setFixedSize(544, 320)
        self.popup = popUpMesg()

        # Adding Actions 
        self.pushButton.clicked.connect(self.addRecipe)
        self.CompTeachWindow = TeachWindow()

    def showEvent(self, event: QShowEvent) -> None:
        self.comboBox_2.clear()
        recipeList = os.listdir("Recipe")
        self.comboBox_2.addItems(recipeList)
        return super().showEvent(event)

    def addRecipe(self) -> None:
        recipeName = self.lineEdit.text()
        mode = self.comboBox.currentText()
        try:
            os.mkdir(f"Recipe/{recipeName}")
        except Exception as e:
            print(e)
            self.popup.makePopUp(heading="Error !!", mesg=f"{e}")

        data = {
            "Mode" : mode
        }
        with open(f"Recipe/{recipeName}/details.toml", "w+") as file:
            toml.dump(data, file)
        
        self.popup.makePopUp("Sucess !!", mesg="Recipe Created")
        self.comboBox_2.addItem(recipeName)
        if mode == "Comp"

# This Class implemets the global setting for the applicaton
class AppSettingWindow(QWidget, Ui_SettingWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.pushButton_5.clicked.connect(self.getCamerasInfo)
        self.pushButton_8.clicked.connect(self.getImage)
        self.pushButton_4.clicked.connect(self.saveImageAndDeviceSettings)
        self.pushButton_9.clicked.connect(self.saveImageAndDeviceSettings)
        self.pushButton_3.clicked.connect(self.savePLCSettings)
        self.popup = popUpMesg()
    
    def getCamerasInfo(self):
        self.tableWidget.clearContents()
        self.comboBox.clear()

        deviceList = IMV_DeviceList()
        interfaceType = IMV_EInterfaceType.interfaceTypeAll
        nWidth=c_uint()
        nHeight=c_uint()

        nRet=MvCamera.IMV_EnumDevices(deviceList,interfaceType)

        if nRet != IMV_OK:
            print("Enumrate Device Failed")

        self.tableWidget.setRowCount(deviceList.nDevNum)

        self.popup.makePopUp(heading="Sucess !!!", mesg=f"Scan Sucess Found {deviceList.nDevNum} Devices")
        
        for i in range(0, deviceList.nDevNum):
            pDeviceInfo = deviceList.pDevInfo[i]
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

            self.tableWidget.setItem(i, 0, QTableWidgetItem(strIpAdress))
            self.tableWidget.setItem(i, 1, QTableWidgetItem(strType))
            self.tableWidget.setItem(i, 2, QTableWidgetItem(strVendorName))
            self.tableWidget.setItem(i, 3, QTableWidgetItem(strSerialNumber))
            self.tableWidget.setItem(i, 4, QTableWidgetItem(strModeName))
        
            # print ("[%d]  %s   %s    %s      %s     %s           %s" % (i+1, strType,strVendorName,strModeName,strSerialNumber,strCameraname,strIpAdress))
            self.comboBox.addItem(f"{i+1}")

    def getImage(self):
        try:
            cvImage = getImageFromCamera(int(self.comboBox.currentText()), (self.spinBox_2.value(), self.spinBox_3.value()))
            self.label_9.setPixmap(QPixmap.fromImage(cv2_image_to_qimage(cvImage)))
        except Exception as e:
            self.popup.makePopUp(heading="Error !!!", mesg=f"{e}")

    def saveImageAndDeviceSettings(self):
        data = {
            "Device":{
                "index" : int(self.comboBox.currentText()),
                "height": self.spinBox_2.value(),
                "width" : self.spinBox_3.value()
            }
        }
        with open("Configuration/GlobalDevice.toml", "w+") as file:
            toml.dump(data, file)
        
        self.popup.makePopUp("Sucess !!", "Data Saved Sucessfully")

    def savePLCSettings(self):
        data = {
            "PLC" : {
                "ip"             : self.lineEdit.text(),
                "port"           : self.spinBox.value(),
                "Comm.Protocol"  : self.comboBox_2.currentText(),
                "Mapping Address": self.lineEdit_2.text(),
                "Trigger Address": self.lineEdit_3.text(),
                "Feedback Adress": self.lineEdit_4.text()
            }
        }

        with open("Configuration/PLCDevice.toml", "w+") as file:
            toml.dump(data, file)
        
        self.popup.makePopUp("Sucess !!", "Data Saved Sucessfully")

    def showEvent(self, event: QShowEvent) -> None:
        # Loading camera info
        with open("Configuration/GlobalDevice.toml") as file:
            data = toml.load(file)

        self.comboBox.addItem(str(data["Device"]["index"]))
        self.spinBox_2.setValue(data["Device"]["height"])
        self.spinBox_3.setValue(data["Device"]["width"])
    
        with open("Configuration/PLCDevice.toml") as file:
            data2 = toml.load(file)

        print(data)
        
        self.lineEdit.setText(data2["PLC"]["ip"])
        self.spinBox.setValue(data2["PLC"]["port"])
        if data2["PLC"]["Comm.Protocol"] == "Modbus":
            self.comboBox_2.setCurrentIndex(0)
        else:
            self.comboBox_2.setCurrentIndex(1)
        
        self.lineEdit_2.setText(data2["PLC"]["Mapping Address"])
        self.lineEdit_3.setText(data2["PLC"]["Trigger Address"])
        self.lineEdit_4.setText(data2["PLC"]["Feedback Adress"])

        return super().showEvent(event)

# This the main Application Class for the App
class VisionApp(QMainWindow, Ui_VisionApp):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        # Defining the ui pages
        self.settingsPage = AppSettingWindow()
        self.recipeOptions = RecipeOptions()
        self.CompareTeach = TeachWindow()
        
        # Connecting Actions 
        self.actionOpen_Device_Settings.triggered.connect(lambda : self.settingsPage.show())
        self.actionCreateOrModifyRecipe.triggered.connect(lambda : self.recipeOptions.show())
        self.pushButton_2.clicked.connect(lambda : self.recipeOptions.show())
    
    def closeEvent(self, event: QCloseEvent) -> None:
        self.settingsPage.close()
        self.recipeOptions.close()
        return super().closeEvent(event)

if __name__ == "__main__":
    app = QApplication()
    window = VisionApp()
    window.show()
    app.exec()