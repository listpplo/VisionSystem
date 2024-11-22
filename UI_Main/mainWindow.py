# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainWindow.ui'
##
## Created by: Qt User Interface Compiler version 6.8.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QAction, QBrush, QColor, QConicalGradient,
    QCursor, QFont, QFontDatabase, QGradient,
    QIcon, QImage, QKeySequence, QLinearGradient,
    QPainter, QPalette, QPixmap, QRadialGradient,
    QTransform)
from PySide6.QtWidgets import (QApplication, QFrame, QGroupBox, QHBoxLayout,
    QLabel, QLineEdit, QMainWindow, QMenu,
    QMenuBar, QPushButton, QSizePolicy, QSpacerItem,
    QVBoxLayout, QWidget)
import StaticAssets_rc

class Ui_VisionApp(object):
    def setupUi(self, VisionApp):
        if not VisionApp.objectName():
            VisionApp.setObjectName(u"VisionApp")
        VisionApp.resize(916, 630)
        font = QFont()
        font.setBold(False)
        VisionApp.setFont(font)
        VisionApp.setStyleSheet(u"QWidget{\n"
"	color:white;\n"
"	background-color: rgb(50, 50, 50);\n"
"}")
        VisionApp.setUnifiedTitleAndToolBarOnMac(True)
        self.actionSave_Current_Image = QAction(VisionApp)
        self.actionSave_Current_Image.setObjectName(u"actionSave_Current_Image")
        self.actionCreateOrModifyRecipe = QAction(VisionApp)
        self.actionCreateOrModifyRecipe.setObjectName(u"actionCreateOrModifyRecipe")
        self.actionOpen_Teaching_Window = QAction(VisionApp)
        self.actionOpen_Teaching_Window.setObjectName(u"actionOpen_Teaching_Window")
        font1 = QFont()
        font1.setBold(True)
        self.actionOpen_Teaching_Window.setFont(font1)
        self.actionOpen_Device_Settings = QAction(VisionApp)
        self.actionOpen_Device_Settings.setObjectName(u"actionOpen_Device_Settings")
        self.actionOpen_Device_Settings.setCheckable(False)
        self.actionLoad_Image_2 = QAction(VisionApp)
        self.actionLoad_Image_2.setObjectName(u"actionLoad_Image_2")
        self.actionSave_Current_Image_2 = QAction(VisionApp)
        self.actionSave_Current_Image_2.setObjectName(u"actionSave_Current_Image_2")
        self.actionPLC_Device_Settings = QAction(VisionApp)
        self.actionPLC_Device_Settings.setObjectName(u"actionPLC_Device_Settings")
        self.centralwidget = QWidget(VisionApp)
        self.centralwidget.setObjectName(u"centralwidget")
        self.centralwidget.setStyleSheet(u"QWidget{\n"
"	background-color: rgb(50, 50, 50);\n"
"}")
        self.horizontalLayout = QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.frame_2 = QFrame(self.centralwidget)
        self.frame_2.setObjectName(u"frame_2")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.frame_2.sizePolicy().hasHeightForWidth())
        self.frame_2.setSizePolicy(sizePolicy)
        self.frame_2.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_4 = QVBoxLayout(self.frame_2)
        self.verticalLayout_4.setSpacing(0)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.verticalLayout_4.setContentsMargins(0, 0, 0, 0)
        self.groupBox_5 = QGroupBox(self.frame_2)
        self.groupBox_5.setObjectName(u"groupBox_5")
        self.groupBox_5.setFont(font1)
        self.groupBox_5.setStyleSheet(u"QGroupBox{\n"
"	border:2px dashed black;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"\n"
"QLabel{\n"
"	color: rgb(255, 255, 255);\n"
"	background-color:none;\n"
"}\n"
"\n"
"QLineEdit{\n"
"	color:rgb(6, 6, 6);\n"
"	background-color:none;\n"
"}")
        self.groupBox_5.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.horizontalLayout_4 = QHBoxLayout(self.groupBox_5)
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.horizontalLayout_4.setContentsMargins(-1, 25, -1, -1)
        self.label_7 = QLabel(self.groupBox_5)
        self.label_7.setObjectName(u"label_7")

        self.horizontalLayout_4.addWidget(self.label_7)

        self.lineEdit_4 = QLineEdit(self.groupBox_5)
        self.lineEdit_4.setObjectName(u"lineEdit_4")
        font2 = QFont()
        font2.setPointSize(12)
        font2.setBold(True)
        self.lineEdit_4.setFont(font2)

        self.horizontalLayout_4.addWidget(self.lineEdit_4)

        self.label_12 = QLabel(self.groupBox_5)
        self.label_12.setObjectName(u"label_12")

        self.horizontalLayout_4.addWidget(self.label_12)

        self.lineEdit_5 = QLineEdit(self.groupBox_5)
        self.lineEdit_5.setObjectName(u"lineEdit_5")
        self.lineEdit_5.setMaximumSize(QSize(72, 16777215))
        self.lineEdit_5.setFont(font2)

        self.horizontalLayout_4.addWidget(self.lineEdit_5)

        self.label_14 = QLabel(self.groupBox_5)
        self.label_14.setObjectName(u"label_14")

        self.horizontalLayout_4.addWidget(self.label_14)

        self.lineEdit_6 = QLineEdit(self.groupBox_5)
        self.lineEdit_6.setObjectName(u"lineEdit_6")
        self.lineEdit_6.setMaximumSize(QSize(72, 16777215))
        self.lineEdit_6.setFont(font2)

        self.horizontalLayout_4.addWidget(self.lineEdit_6)

        self.label_13 = QLabel(self.groupBox_5)
        self.label_13.setObjectName(u"label_13")

        self.horizontalLayout_4.addWidget(self.label_13)

        self.label_11 = QLabel(self.groupBox_5)
        self.label_11.setObjectName(u"label_11")

        self.horizontalLayout_4.addWidget(self.label_11)

        self.pushButton_3 = QPushButton(self.groupBox_5)
        self.pushButton_3.setObjectName(u"pushButton_3")
        self.pushButton_3.setEnabled(True)
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.pushButton_3.sizePolicy().hasHeightForWidth())
        self.pushButton_3.setSizePolicy(sizePolicy1)
        self.pushButton_3.setMinimumSize(QSize(80, 0))
        self.pushButton_3.setStyleSheet(u"QPushButton{\n"
"	\n"
"	background-color: rgb(255, 0, 0);\n"
"}")

        self.horizontalLayout_4.addWidget(self.pushButton_3, 0, Qt.AlignmentFlag.AlignLeft)


        self.verticalLayout_4.addWidget(self.groupBox_5)

        self.OutputImage = QLabel(self.frame_2)
        self.OutputImage.setObjectName(u"OutputImage")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Expanding)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.OutputImage.sizePolicy().hasHeightForWidth())
        self.OutputImage.setSizePolicy(sizePolicy2)
        self.OutputImage.setFont(font2)
        self.OutputImage.setStyleSheet(u"QLabel{\n"
"	color:white;\n"
"}")
        self.OutputImage.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.verticalLayout_4.addWidget(self.OutputImage)


        self.horizontalLayout.addWidget(self.frame_2)

        self.groupBox = QGroupBox(self.centralwidget)
        self.groupBox.setObjectName(u"groupBox")
        self.groupBox.setMinimumSize(QSize(250, 0))
        self.groupBox.setFont(font1)
        self.groupBox.setStyleSheet(u"QGroupBox{\n"
"	border:2px dashed black;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"\n"
"QLabel{\n"
"	color: rgb(255, 255, 255);\n"
"	background-color:none;\n"
"}\n"
"\n"
"QLineEdit{\n"
"	color:rgb(16, 255, 12);\n"
"	background-color:none;\n"
"}")
        self.groupBox.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.verticalLayout_3 = QVBoxLayout(self.groupBox)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.verticalLayout_3.setContentsMargins(-1, 25, -1, -1)
        self.groupBox_2 = QGroupBox(self.groupBox)
        self.groupBox_2.setObjectName(u"groupBox_2")
        self.groupBox_2.setFont(font1)
        self.groupBox_2.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.verticalLayout = QVBoxLayout(self.groupBox_2)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(-1, 25, -1, -1)
        self.label_2 = QLabel(self.groupBox_2)
        self.label_2.setObjectName(u"label_2")

        self.verticalLayout.addWidget(self.label_2)

        self.lineEdit = QLineEdit(self.groupBox_2)
        self.lineEdit.setObjectName(u"lineEdit")
        self.lineEdit.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.lineEdit.setReadOnly(True)

        self.verticalLayout.addWidget(self.lineEdit)

        self.label_3 = QLabel(self.groupBox_2)
        self.label_3.setObjectName(u"label_3")

        self.verticalLayout.addWidget(self.label_3)

        self.lineEdit_2 = QLineEdit(self.groupBox_2)
        self.lineEdit_2.setObjectName(u"lineEdit_2")
        self.lineEdit_2.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.lineEdit_2.setReadOnly(True)

        self.verticalLayout.addWidget(self.lineEdit_2)

        self.label_4 = QLabel(self.groupBox_2)
        self.label_4.setObjectName(u"label_4")

        self.verticalLayout.addWidget(self.label_4)

        self.lineEdit_3 = QLineEdit(self.groupBox_2)
        self.lineEdit_3.setObjectName(u"lineEdit_3")
        self.lineEdit_3.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.lineEdit_3.setReadOnly(True)

        self.verticalLayout.addWidget(self.lineEdit_3)

        self.lineEdit.raise_()
        self.label_2.raise_()
        self.label_3.raise_()
        self.label_4.raise_()
        self.lineEdit_2.raise_()
        self.lineEdit_3.raise_()

        self.verticalLayout_3.addWidget(self.groupBox_2)

        self.groupBox_3 = QGroupBox(self.groupBox)
        self.groupBox_3.setObjectName(u"groupBox_3")
        self.groupBox_3.setFont(font1)
        self.groupBox_3.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.verticalLayout_2 = QVBoxLayout(self.groupBox_3)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.verticalLayout_2.setContentsMargins(-1, 25, -1, -1)
        self.frame = QFrame(self.groupBox_3)
        self.frame.setObjectName(u"frame")
        self.frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_2 = QHBoxLayout(self.frame)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.label = QLabel(self.frame)
        self.label.setObjectName(u"label")
        font3 = QFont()
        font3.setPointSize(20)
        self.label.setFont(font3)
        self.label.setStyleSheet(u"QLabel{\n"
"	color:black;\n"
"	background-color: rgb(85, 255, 0);\n"
"}")
        self.label.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_2.addWidget(self.label)

        self.label_5 = QLabel(self.frame)
        self.label_5.setObjectName(u"label_5")
        self.label_5.setFont(font3)
        self.label_5.setStyleSheet(u"QLabel{\n"
"		color:black;\n"
"	background-color: rgb(255, 88, 0);\n"
"}")
        self.label_5.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_2.addWidget(self.label_5)


        self.verticalLayout_2.addWidget(self.frame)

        self.label_6 = QLabel(self.groupBox_3)
        self.label_6.setObjectName(u"label_6")
        self.label_6.setFont(font3)
        self.label_6.setStyleSheet(u"QLabel{\n"
"	color:black;\n"
"	background-color: rgb(255, 255, 0);\n"
"}")
        self.label_6.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.verticalLayout_2.addWidget(self.label_6)


        self.verticalLayout_3.addWidget(self.groupBox_3)

        self.groupBox_4 = QGroupBox(self.groupBox)
        self.groupBox_4.setObjectName(u"groupBox_4")
        self.groupBox_4.setFont(font1)
        self.groupBox_4.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.verticalLayout_5 = QVBoxLayout(self.groupBox_4)
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.verticalLayout_5.setContentsMargins(-1, 25, -1, -1)
        self.pushButton_4 = QPushButton(self.groupBox_4)
        self.pushButton_4.setObjectName(u"pushButton_4")
        self.pushButton_4.setStyleSheet(u"QPushButton{\n"
"	color: rgb(249, 249, 249);\n"
"}\n"
"\n"
"QPushButton::hover{\n"
"	background-color: rgb(130, 130, 130);\n"
"}\n"
"\n"
"QPushButton::checked{\n"
"	background-color: rgb(124, 255, 64);\n"
"}")
        icon = QIcon()
        icon.addFile(u":/Icons/Assets/Icons/icons8-start-64.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.pushButton_4.setIcon(icon)
        self.pushButton_4.setIconSize(QSize(40, 40))
        self.pushButton_4.setCheckable(True)

        self.verticalLayout_5.addWidget(self.pushButton_4)

        self.pushButton = QPushButton(self.groupBox_4)
        self.pushButton.setObjectName(u"pushButton")
        self.pushButton.setStyleSheet(u"QPushButton{\n"
"	color: rgb(249, 249, 249);\n"
"}\n"
"\n"
"QPushButton::hover{\n"
"	background-color: rgb(130, 130, 130);\n"
"}")
        icon1 = QIcon()
        icon1.addFile(u":/Icons/Assets/Icons/icons8-light-switch-64 (1).png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.pushButton.setIcon(icon1)
        self.pushButton.setIconSize(QSize(40, 40))

        self.verticalLayout_5.addWidget(self.pushButton)

        self.pushButton_2 = QPushButton(self.groupBox_4)
        self.pushButton_2.setObjectName(u"pushButton_2")
        self.pushButton_2.setStyleSheet(u"QPushButton{\n"
"	color: rgb(249, 249, 249);\n"
"}\n"
"\n"
"QPushButton::hover{\n"
"	background-color: rgb(130, 130, 130);\n"
"}")
        icon2 = QIcon()
        icon2.addFile(u":/Icons/Assets/Icons/icons8-abc-block-100.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.pushButton_2.setIcon(icon2)
        self.pushButton_2.setIconSize(QSize(40, 40))

        self.verticalLayout_5.addWidget(self.pushButton_2)


        self.verticalLayout_3.addWidget(self.groupBox_4)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_3.addItem(self.verticalSpacer)


        self.horizontalLayout.addWidget(self.groupBox, 0, Qt.AlignmentFlag.AlignRight)

        VisionApp.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(VisionApp)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 916, 33))
        self.menuFile = QMenu(self.menubar)
        self.menuFile.setObjectName(u"menuFile")
        self.menuFile.setFont(font1)
        self.menuTeach = QMenu(self.menubar)
        self.menuTeach.setObjectName(u"menuTeach")
        self.menuTeach.setFont(font1)
        self.menuSettings = QMenu(self.menubar)
        self.menuSettings.setObjectName(u"menuSettings")
        self.menuSettings.setFont(font1)
        VisionApp.setMenuBar(self.menubar)

        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuTeach.menuAction())
        self.menubar.addAction(self.menuSettings.menuAction())
        self.menuFile.addAction(self.actionCreateOrModifyRecipe)
        self.menuFile.addAction(self.actionLoad_Image_2)
        self.menuFile.addAction(self.actionSave_Current_Image_2)
        self.menuTeach.addAction(self.actionOpen_Teaching_Window)
        self.menuSettings.addAction(self.actionOpen_Device_Settings)

        self.retranslateUi(VisionApp)

        QMetaObject.connectSlotsByName(VisionApp)
    # setupUi

    def retranslateUi(self, VisionApp):
        VisionApp.setWindowTitle(QCoreApplication.translate("VisionApp", u"Vision System", None))
        self.actionSave_Current_Image.setText(QCoreApplication.translate("VisionApp", u"Load Program", None))
        self.actionCreateOrModifyRecipe.setText(QCoreApplication.translate("VisionApp", u"Create Or Modify Recipe", None))
        self.actionOpen_Teaching_Window.setText(QCoreApplication.translate("VisionApp", u"Open Teaching Window", None))
        self.actionOpen_Device_Settings.setText(QCoreApplication.translate("VisionApp", u"Open Device Settings", None))
        self.actionLoad_Image_2.setText(QCoreApplication.translate("VisionApp", u"Load Image", None))
        self.actionSave_Current_Image_2.setText(QCoreApplication.translate("VisionApp", u"Save Current Image", None))
        self.actionPLC_Device_Settings.setText(QCoreApplication.translate("VisionApp", u"PLC Device Settings", None))
        self.groupBox_5.setTitle(QCoreApplication.translate("VisionApp", u"Current Program :", None))
        self.label_7.setText(QCoreApplication.translate("VisionApp", u"Current Recipe\n"
"Name :", None))
        self.lineEdit_4.setText("")
        self.label_12.setText(QCoreApplication.translate("VisionApp", u"Current Reipe\n"
"Numer :", None))
        self.lineEdit_5.setText("")
        self.label_14.setText(QCoreApplication.translate("VisionApp", u"PLC\n"
"Command :", None))
        self.lineEdit_6.setText("")
        self.label_13.setText("")
        self.label_11.setText(QCoreApplication.translate("VisionApp", u"Application\n"
"Status :", None))
        self.pushButton_3.setText("")
        self.OutputImage.setText(QCoreApplication.translate("VisionApp", u"Here The Image will be shown", None))
        self.groupBox.setTitle(QCoreApplication.translate("VisionApp", u"Running Parameters", None))
        self.groupBox_2.setTitle(QCoreApplication.translate("VisionApp", u"Camera Parameter", None))
        self.label_2.setText(QCoreApplication.translate("VisionApp", u"Model Name :", None))
        self.label_3.setText(QCoreApplication.translate("VisionApp", u"Model Serial No :", None))
        self.label_4.setText(QCoreApplication.translate("VisionApp", u"InterFace Type :", None))
        self.groupBox_3.setTitle(QCoreApplication.translate("VisionApp", u"Cycle Parameter", None))
        self.label.setText(QCoreApplication.translate("VisionApp", u"---", None))
        self.label_5.setText(QCoreApplication.translate("VisionApp", u"---", None))
        self.label_6.setText(QCoreApplication.translate("VisionApp", u"---", None))
        self.groupBox_4.setTitle(QCoreApplication.translate("VisionApp", u"Operations", None))
        self.pushButton_4.setText(QCoreApplication.translate("VisionApp", u" Start Process        ", None))
#if QT_CONFIG(shortcut)
        self.pushButton_4.setShortcut(QCoreApplication.translate("VisionApp", u"Return", None))
#endif // QT_CONFIG(shortcut)
        self.pushButton.setText(QCoreApplication.translate("VisionApp", u"Manual Trigger", None))
#if QT_CONFIG(shortcut)
        self.pushButton.setShortcut(QCoreApplication.translate("VisionApp", u"Return", None))
#endif // QT_CONFIG(shortcut)
        self.pushButton_2.setText(QCoreApplication.translate("VisionApp", u"Open Teach Menu", None))
        self.menuFile.setTitle(QCoreApplication.translate("VisionApp", u"File", None))
        self.menuTeach.setTitle(QCoreApplication.translate("VisionApp", u"Teach", None))
        self.menuSettings.setTitle(QCoreApplication.translate("VisionApp", u"Settings", None))
    # retranslateUi

