from UI_Main.CompareTeach import Ui_Form
import UI_Main.StaticAssets_rc
from PySide6.QtWidgets import QWidget, QApplication
from PySide6.QtGui import QPixmap, QImage
from usefulFunctions import getImageFromCamera, cv2_image_to_qimage, popUpMesg
import toml
import cv2

class TeachWindow(QWidget, Ui_Form):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        # loading COnfig File
        with open("Configuration\GlobalDevice.toml") as file:
            self.globalCameraData = toml.load(file)

        # Setting Up popup mesg
        self.popup =popUpMesg()
        
        self.cameraIndex = self.globalCameraData["Device"]["index"]
        self.height = self.globalCameraData["Device"]["height"]
        self.width = self.globalCameraData["Device"]["width"]
        
        # making Ui Changes
        self.tabWidget.tabBar().hide()

        # Loading Actions
        self.pushButton.clicked.connect(self.getImageViaTrigger)
        self.pushButton_3.clicked.connect(self.cropImage)

        # Setting up title bar
    

    def getImageViaTrigger(self):
        # Checking for Cropping
        try:
            cvImage = getImageFromCamera(self.cameraIndex, (self.height, self.width))
        except Exception as e:
            print(e)
            self.popup.makePopUp("Error !!", mesg=f"{e}")
        
        if self.checkBox_2.isChecked():
            kernal_size = (self.horizontalSlider_2.value(), self.horizontalSlider_2.value())
            cvImage = cv2.GaussianBlur(cvImage, kernal_size, sigmaX=2)

        match self.comboBox.currentText():
            case "Normal":
                ...
            case "Gray Scale":
                cvImage = cv2.cvtColor(cvImage,  cv2.COLOR_BGR2GRAY)
                self.label_43.setPixmap(QPixmap.fromImage(cv2_image_to_qimage(cvImage)))
            case "Normalised":
                cvImage_grey = cv2.cvtColor(cvImage,  cv2.COLOR_BGR2GRAY)
                _, cvImage = cv2.threshold(cvImage_grey, self.horizontalSlider_3.value(), self.horizontalSlider.value(), cv2.THRESH_BINARY)

        if self.checkBox.isChecked():
            self.label_43.setPixmap(QPixmap.fromImage(cv2_image_to_qimage(cvImage)))
               
        else:
            x, y, w, h = self.spinBox.value(), self.spinBox_2.value(), self.spinBox_3.value(), self.spinBox_4.value()
            cvImage_croped = cvImage[y:y+h, x:x+w]
            cvImage_croped_resized = cv2.resize(cvImage_croped, (self.height, self.width))
            self.label_43.setPixmap(QPixmap.fromImage(cv2_image_to_qimage(cvImage_croped_resized)))

    def cropImage(self):
        self.checkBox.setChecked(False)
        try:
            cvImage = getImageFromCamera(self.cameraIndex, (self.height, self.width))
            roi = cv2.selectROI("Select ROI", cvImage, fromCenter=False, printNotice=True)
            cv2.destroyAllWindows()
            print(roi)
            x, y, w, h = roi
            self.spinBox.setValue(x)
            self.spinBox_2.setValue(y)
            self.spinBox_4.setValue(h)
            self.spinBox_3.setValue(w)
            cvImage_croped = cvImage[y:y+h, x:x+w]
            cvImage_croped_resized = cv2.resize(cvImage_croped, (self.height, self.width))
            self.label_43.setPixmap(QPixmap.fromImage(cv2_image_to_qimage(cvImage_croped_resized)))
        except Exception as e:
            print(e)
            self.popup.makePopUp("Error !!", mesg=f"{e}")

    def saveParameters(self):
        data = {
            "crop" : {
                "x": self.spinBox.value(),
                "y": self.spinBox_2.value(),
                "w": self.spinBox_3.value(),
                "h": self.spinBox_4.value()
            }
            
        }

if __name__ == "__main__":
    app = QApplication()
    window = TeachWindow()
    window.show()
    app.exec()