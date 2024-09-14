# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'ComapreTeach.ui'
##
## Created by: Qt User Interface Compiler version 6.7.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QCheckBox, QComboBox, QFrame,
    QGridLayout, QGroupBox, QHBoxLayout, QLabel,
    QPushButton, QScrollArea, QSizePolicy, QSlider,
    QSpacerItem, QSpinBox, QTabWidget, QVBoxLayout,
    QWidget)
import StaticAssets_rc

class Ui_Form(object):
    def setupUi(self, Form):
        if not Form.objectName():
            Form.setObjectName(u"Form")
        Form.resize(886, 651)
        Form.setStyleSheet(u"QWidget{\n"
"	color:white;\n"
"	background-color: rgb(50, 50, 50);\n"
"}\n"
"")
        self.horizontalLayout = QHBoxLayout(Form)
        self.horizontalLayout.setSpacing(0)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.tabWidget = QTabWidget(Form)
        self.tabWidget.setObjectName(u"tabWidget")
        self.tabWidget.setStyleSheet(u"")
        self.tabWidgetPage1 = QWidget()
        self.tabWidgetPage1.setObjectName(u"tabWidgetPage1")
        self.verticalLayout = QVBoxLayout(self.tabWidgetPage1)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.frame_2 = QFrame(self.tabWidgetPage1)
        self.frame_2.setObjectName(u"frame_2")
        self.frame_2.setStyleSheet(u"QFrame{\n"
"	border:none;\n"
"	border-radius:30%;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"")
        self.frame_2.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_2 = QHBoxLayout(self.frame_2)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.label = QLabel(self.frame_2)
        self.label.setObjectName(u"label")
        font = QFont()
        font.setPointSize(10)
        font.setBold(True)
        self.label.setFont(font)
        self.label.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"	boarder-radius:20%;\n"
"	background-color: rgb(83, 26, 255);\n"
"}")
        self.label.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_2.addWidget(self.label)

        self.label_2 = QLabel(self.frame_2)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_2.addWidget(self.label_2, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_3 = QLabel(self.frame_2)
        self.label_3.setObjectName(u"label_3")
        self.label_3.setFont(font)
        self.label_3.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"}")
        self.label_3.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_2.addWidget(self.label_3)

        self.label_6 = QLabel(self.frame_2)
        self.label_6.setObjectName(u"label_6")
        self.label_6.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_2.addWidget(self.label_6, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_4 = QLabel(self.frame_2)
        self.label_4.setObjectName(u"label_4")
        self.label_4.setFont(font)
        self.label_4.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"}")
        self.label_4.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_2.addWidget(self.label_4)

        self.label_7 = QLabel(self.frame_2)
        self.label_7.setObjectName(u"label_7")
        self.label_7.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_2.addWidget(self.label_7, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_5 = QLabel(self.frame_2)
        self.label_5.setObjectName(u"label_5")
        self.label_5.setFont(font)
        self.label_5.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"}")
        self.label_5.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_2.addWidget(self.label_5)


        self.verticalLayout.addWidget(self.frame_2, 0, Qt.AlignmentFlag.AlignTop)

        self.frame_3 = QFrame(self.tabWidgetPage1)
        self.frame_3.setObjectName(u"frame_3")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.frame_3.sizePolicy().hasHeightForWidth())
        self.frame_3.setSizePolicy(sizePolicy)
        self.frame_3.setStyleSheet(u"QFrame{\n"
"	border:none;\n"
"	border-radius:30%;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"")
        self.frame_3.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_3.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_8 = QHBoxLayout(self.frame_3)
        self.horizontalLayout_8.setObjectName(u"horizontalLayout_8")
        self.frame = QFrame(self.frame_3)
        self.frame.setObjectName(u"frame")
        self.frame.setStyleSheet(u"QLabel{\n"
"	border:2px dashed white;\n"
"}")
        self.frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_9 = QHBoxLayout(self.frame)
        self.horizontalLayout_9.setSpacing(0)
        self.horizontalLayout_9.setObjectName(u"horizontalLayout_9")
        self.horizontalLayout_9.setContentsMargins(0, 0, -1, 0)
        self.scrollArea_2 = QScrollArea(self.frame)
        self.scrollArea_2.setObjectName(u"scrollArea_2")
        self.scrollArea_2.setStyleSheet(u"	background-color: rgb(110, 110, 110);")
        self.scrollArea_2.setWidgetResizable(True)
        self.scrollAreaWidgetContents_2 = QWidget()
        self.scrollAreaWidgetContents_2.setObjectName(u"scrollAreaWidgetContents_2")
        self.scrollAreaWidgetContents_2.setGeometry(QRect(0, 0, 529, 462))
        self.horizontalLayout_11 = QHBoxLayout(self.scrollAreaWidgetContents_2)
        self.horizontalLayout_11.setSpacing(0)
        self.horizontalLayout_11.setObjectName(u"horizontalLayout_11")
        self.horizontalLayout_11.setContentsMargins(0, 0, 0, 0)
        self.label_43 = QLabel(self.scrollAreaWidgetContents_2)
        self.label_43.setObjectName(u"label_43")
        self.label_43.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_11.addWidget(self.label_43)

        self.scrollArea_2.setWidget(self.scrollAreaWidgetContents_2)

        self.horizontalLayout_9.addWidget(self.scrollArea_2)


        self.horizontalLayout_8.addWidget(self.frame)

        self.groupBox = QGroupBox(self.frame_3)
        self.groupBox.setObjectName(u"groupBox")
        self.groupBox.setMinimumSize(QSize(300, 0))
        font1 = QFont()
        font1.setBold(True)
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
        self.verticalLayout_6 = QVBoxLayout(self.groupBox)
        self.verticalLayout_6.setObjectName(u"verticalLayout_6")
        self.verticalLayout_6.setContentsMargins(-1, 25, -1, -1)
        self.scrollArea = QScrollArea(self.groupBox)
        self.scrollArea.setObjectName(u"scrollArea")
        self.scrollArea.setWidgetResizable(True)
        self.scrollAreaWidgetContents = QWidget()
        self.scrollAreaWidgetContents.setObjectName(u"scrollAreaWidgetContents")
        self.scrollAreaWidgetContents.setGeometry(QRect(0, -70, 266, 461))
        self.verticalLayout_5 = QVBoxLayout(self.scrollAreaWidgetContents)
        self.verticalLayout_5.setSpacing(6)
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.verticalLayout_5.setContentsMargins(0, 0, 0, 0)
        self.groupBox_2 = QGroupBox(self.scrollAreaWidgetContents)
        self.groupBox_2.setObjectName(u"groupBox_2")
        self.groupBox_2.setFont(font1)
        self.groupBox_2.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.horizontalLayout_12 = QHBoxLayout(self.groupBox_2)
        self.horizontalLayout_12.setObjectName(u"horizontalLayout_12")
        self.horizontalLayout_12.setContentsMargins(-1, 20, -1, -1)
        self.pushButton = QPushButton(self.groupBox_2)
        self.pushButton.setObjectName(u"pushButton")
        self.pushButton.setStyleSheet(u"QPushButton{\n"
"	color: rgb(249, 249, 249);\n"
"	background-color: rgb(70, 70, 70);\n"
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
        icon.addFile(u":/Icons/Assets/Icons/icons8-light-switch-64 (1).png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.pushButton.setIcon(icon)
        self.pushButton.setIconSize(QSize(40, 40))

        self.horizontalLayout_12.addWidget(self.pushButton)

        self.pushButton_2 = QPushButton(self.groupBox_2)
        self.pushButton_2.setObjectName(u"pushButton_2")
        self.pushButton_2.setStyleSheet(u"QPushButton{\n"
"	color: rgb(249, 249, 249);\n"
"	background-color: rgb(70, 70, 70);\n"
"}\n"
"\n"
"QPushButton::hover{\n"
"	background-color: rgb(130, 130, 130);\n"
"}\n"
"\n"
"QPushButton::checked{\n"
"	background-color: rgb(124, 255, 64);\n"
"}")
        icon1 = QIcon()
        icon1.addFile(u":/Icons/Assets/Icons/icons8-save-64.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.pushButton_2.setIcon(icon1)
        self.pushButton_2.setIconSize(QSize(40, 40))

        self.horizontalLayout_12.addWidget(self.pushButton_2)


        self.verticalLayout_5.addWidget(self.groupBox_2)

        self.groupBox_3 = QGroupBox(self.scrollAreaWidgetContents)
        self.groupBox_3.setObjectName(u"groupBox_3")
        self.groupBox_3.setFont(font1)
        self.groupBox_3.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.gridLayout = QGridLayout(self.groupBox_3)
        self.gridLayout.setObjectName(u"gridLayout")
        self.gridLayout.setContentsMargins(-1, 20, -1, -1)
        self.horizontalSlider_3 = QSlider(self.groupBox_3)
        self.horizontalSlider_3.setObjectName(u"horizontalSlider_3")
        self.horizontalSlider_3.setStyleSheet(u"background:none;")
        self.horizontalSlider_3.setMaximum(255)
        self.horizontalSlider_3.setValue(125)
        self.horizontalSlider_3.setOrientation(Qt.Orientation.Horizontal)

        self.gridLayout.addWidget(self.horizontalSlider_3, 7, 2, 1, 1)

        self.label_46 = QLabel(self.groupBox_3)
        self.label_46.setObjectName(u"label_46")
        self.label_46.setFont(font1)

        self.gridLayout.addWidget(self.label_46, 4, 0, 1, 1)

        self.horizontalSlider_2 = QSlider(self.groupBox_3)
        self.horizontalSlider_2.setObjectName(u"horizontalSlider_2")
        self.horizontalSlider_2.setStyleSheet(u"background:none;")
        self.horizontalSlider_2.setMaximum(20)
        self.horizontalSlider_2.setValue(5)
        self.horizontalSlider_2.setOrientation(Qt.Orientation.Horizontal)

        self.gridLayout.addWidget(self.horizontalSlider_2, 9, 2, 1, 1)

        self.line_3 = QFrame(self.groupBox_3)
        self.line_3.setObjectName(u"line_3")
        self.line_3.setStyleSheet(u"border:1.5px dashed white;")
        self.line_3.setFrameShape(QFrame.Shape.HLine)
        self.line_3.setFrameShadow(QFrame.Shadow.Sunken)

        self.gridLayout.addWidget(self.line_3, 3, 2, 1, 1)

        self.label_52 = QLabel(self.groupBox_3)
        self.label_52.setObjectName(u"label_52")
        self.label_52.setFont(font1)

        self.gridLayout.addWidget(self.label_52, 7, 0, 1, 1)

        self.spinBox_3 = QSpinBox(self.groupBox_3)
        self.spinBox_3.setObjectName(u"spinBox_3")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.spinBox_3.sizePolicy().hasHeightForWidth())
        self.spinBox_3.setSizePolicy(sizePolicy1)
        self.spinBox_3.setMinimumSize(QSize(0, 30))
        self.spinBox_3.setMaximum(9999999)

        self.gridLayout.addWidget(self.spinBox_3, 2, 0, 1, 1)

        self.label_48 = QLabel(self.groupBox_3)
        self.label_48.setObjectName(u"label_48")
        self.label_48.setFont(font1)
        self.label_48.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_48, 6, 2, 1, 1)

        self.label_47 = QLabel(self.groupBox_3)
        self.label_47.setObjectName(u"label_47")
        self.label_47.setFont(font1)

        self.gridLayout.addWidget(self.label_47, 5, 0, 1, 1)

        self.spinBox_2 = QSpinBox(self.groupBox_3)
        self.spinBox_2.setObjectName(u"spinBox_2")
        sizePolicy1.setHeightForWidth(self.spinBox_2.sizePolicy().hasHeightForWidth())
        self.spinBox_2.setSizePolicy(sizePolicy1)
        self.spinBox_2.setMinimumSize(QSize(0, 30))
        self.spinBox_2.setMaximum(99999999)

        self.gridLayout.addWidget(self.spinBox_2, 1, 2, 1, 1)

        self.comboBox = QComboBox(self.groupBox_3)
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.setObjectName(u"comboBox")
        self.comboBox.setMinimumSize(QSize(0, 30))
        self.comboBox.setSizeAdjustPolicy(QComboBox.SizeAdjustPolicy.AdjustToMinimumContentsLengthWithIcon)

        self.gridLayout.addWidget(self.comboBox, 4, 2, 1, 1)

        self.horizontalSlider = QSlider(self.groupBox_3)
        self.horizontalSlider.setObjectName(u"horizontalSlider")
        self.horizontalSlider.setStyleSheet(u"background:none;")
        self.horizontalSlider.setMaximum(255)
        self.horizontalSlider.setValue(125)
        self.horizontalSlider.setOrientation(Qt.Orientation.Horizontal)

        self.gridLayout.addWidget(self.horizontalSlider, 5, 2, 1, 1)

        self.label_49 = QLabel(self.groupBox_3)
        self.label_49.setObjectName(u"label_49")
        self.label_49.setFont(font1)

        self.gridLayout.addWidget(self.label_49, 9, 0, 1, 1)

        self.label_45 = QLabel(self.groupBox_3)
        self.label_45.setObjectName(u"label_45")

        self.gridLayout.addWidget(self.label_45, 1, 1, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.spinBox = QSpinBox(self.groupBox_3)
        self.spinBox.setObjectName(u"spinBox")
        sizePolicy1.setHeightForWidth(self.spinBox.sizePolicy().hasHeightForWidth())
        self.spinBox.setSizePolicy(sizePolicy1)
        self.spinBox.setMinimumSize(QSize(50, 30))
        self.spinBox.setMaximum(99999999)

        self.gridLayout.addWidget(self.spinBox, 1, 0, 1, 1)

        self.spinBox_4 = QSpinBox(self.groupBox_3)
        self.spinBox_4.setObjectName(u"spinBox_4")
        sizePolicy1.setHeightForWidth(self.spinBox_4.sizePolicy().hasHeightForWidth())
        self.spinBox_4.setSizePolicy(sizePolicy1)
        self.spinBox_4.setMinimumSize(QSize(0, 30))
        self.spinBox_4.setMaximum(99999999)

        self.gridLayout.addWidget(self.spinBox_4, 2, 2, 1, 1)

        self.label_51 = QLabel(self.groupBox_3)
        self.label_51.setObjectName(u"label_51")

        self.gridLayout.addWidget(self.label_51, 2, 1, 1, 1)

        self.pushButton_3 = QPushButton(self.groupBox_3)
        self.pushButton_3.setObjectName(u"pushButton_3")
        self.pushButton_3.setStyleSheet(u"QPushButton{\n"
"	color: rgb(249, 249, 249);\n"
"	background-color: rgb(70, 70, 70);\n"
"}\n"
"\n"
"QPushButton::hover{\n"
"	background-color: rgb(130, 130, 130);\n"
"}\n"
"\n"
"QPushButton::checked{\n"
"	background-color: rgb(124, 255, 64);\n"
"}")
        icon2 = QIcon()
        icon2.addFile(u":/Icons/Assets/Icons/icons8-crop-100.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.pushButton_3.setIcon(icon2)
        self.pushButton_3.setIconSize(QSize(40, 40))

        self.gridLayout.addWidget(self.pushButton_3, 0, 0, 1, 1)

        self.checkBox = QCheckBox(self.groupBox_3)
        self.checkBox.setObjectName(u"checkBox")
        self.checkBox.setStyleSheet(u"background:none;")
        self.checkBox.setChecked(True)

        self.gridLayout.addWidget(self.checkBox, 0, 2, 1, 1)

        self.label_50 = QLabel(self.groupBox_3)
        self.label_50.setObjectName(u"label_50")
        self.label_50.setFont(font1)
        self.label_50.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_50, 10, 2, 1, 1)

        self.line_2 = QFrame(self.groupBox_3)
        self.line_2.setObjectName(u"line_2")
        self.line_2.setStyleSheet(u"border:1.5px dashed white;")
        self.line_2.setFrameShape(QFrame.Shape.HLine)
        self.line_2.setFrameShadow(QFrame.Shadow.Sunken)

        self.gridLayout.addWidget(self.line_2, 3, 0, 1, 1)

        self.label_53 = QLabel(self.groupBox_3)
        self.label_53.setObjectName(u"label_53")
        self.label_53.setFont(font1)
        self.label_53.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.label_53, 8, 2, 1, 1)

        self.checkBox_2 = QCheckBox(self.groupBox_3)
        self.checkBox_2.setObjectName(u"checkBox_2")
        self.checkBox_2.setStyleSheet(u"background:none;")
        self.checkBox_2.setChecked(False)

        self.gridLayout.addWidget(self.checkBox_2, 10, 0, 1, 1)


        self.verticalLayout_5.addWidget(self.groupBox_3)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_5.addItem(self.verticalSpacer)

        self.scrollArea.setWidget(self.scrollAreaWidgetContents)

        self.verticalLayout_6.addWidget(self.scrollArea)

        self.frame_10 = QFrame(self.groupBox)
        self.frame_10.setObjectName(u"frame_10")
        self.frame_10.setMinimumSize(QSize(0, 30))
        self.frame_10.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_10.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_10 = QHBoxLayout(self.frame_10)
        self.horizontalLayout_10.setObjectName(u"horizontalLayout_10")
        self.horizontalLayout_10.setContentsMargins(0, 0, 0, 0)
        self.pushButton_4 = QPushButton(self.frame_10)
        self.pushButton_4.setObjectName(u"pushButton_4")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.pushButton_4.sizePolicy().hasHeightForWidth())
        self.pushButton_4.setSizePolicy(sizePolicy2)
        self.pushButton_4.setMinimumSize(QSize(0, 50))
        self.pushButton_4.setStyleSheet(u"QPushButton{\n"
"	color: rgb(25, 25, 25);\n"
"	background-color: rgb(221, 221, 221);\n"
"}\n"
"\n"
"QPushButton::hover{\n"
"	background-color: rgb(130, 130, 130);\n"
"}\n"
"\n"
"QPushButton::checked{\n"
"	background-color: rgb(124, 255, 64);\n"
"}")
        self.pushButton_4.setIcon(icon1)
        self.pushButton_4.setIconSize(QSize(40, 40))

        self.horizontalLayout_10.addWidget(self.pushButton_4)


        self.verticalLayout_6.addWidget(self.frame_10)


        self.horizontalLayout_8.addWidget(self.groupBox, 0, Qt.AlignmentFlag.AlignRight)


        self.verticalLayout.addWidget(self.frame_3)

        self.tabWidget.addTab(self.tabWidgetPage1, "")
        self.tab = QWidget()
        self.tab.setObjectName(u"tab")
        self.verticalLayout_4 = QVBoxLayout(self.tab)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.frame_4 = QFrame(self.tab)
        self.frame_4.setObjectName(u"frame_4")
        self.frame_4.setStyleSheet(u"QFrame{\n"
"	border:none;\n"
"	border-radius:30%;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"")
        self.frame_4.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_4.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_5 = QHBoxLayout(self.frame_4)
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.label_22 = QLabel(self.frame_4)
        self.label_22.setObjectName(u"label_22")
        self.label_22.setFont(font)
        self.label_22.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"	boarder-radius:20%;\n"
"	background-color: rgb(83, 26, 255);\n"
"}")
        self.label_22.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_5.addWidget(self.label_22)

        self.label_23 = QLabel(self.frame_4)
        self.label_23.setObjectName(u"label_23")
        self.label_23.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_5.addWidget(self.label_23, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_24 = QLabel(self.frame_4)
        self.label_24.setObjectName(u"label_24")
        self.label_24.setFont(font)
        self.label_24.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"	boarder-radius:20%;\n"
"	background-color: rgb(83, 26, 255);\n"
"}")
        self.label_24.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_5.addWidget(self.label_24)

        self.label_25 = QLabel(self.frame_4)
        self.label_25.setObjectName(u"label_25")
        self.label_25.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_5.addWidget(self.label_25, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_26 = QLabel(self.frame_4)
        self.label_26.setObjectName(u"label_26")
        self.label_26.setFont(font)
        self.label_26.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"}")
        self.label_26.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_5.addWidget(self.label_26)

        self.label_27 = QLabel(self.frame_4)
        self.label_27.setObjectName(u"label_27")
        self.label_27.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_5.addWidget(self.label_27, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_28 = QLabel(self.frame_4)
        self.label_28.setObjectName(u"label_28")
        self.label_28.setFont(font)
        self.label_28.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"}")
        self.label_28.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_5.addWidget(self.label_28)


        self.verticalLayout_4.addWidget(self.frame_4)

        self.frame_5 = QFrame(self.tab)
        self.frame_5.setObjectName(u"frame_5")
        sizePolicy.setHeightForWidth(self.frame_5.sizePolicy().hasHeightForWidth())
        self.frame_5.setSizePolicy(sizePolicy)
        self.frame_5.setStyleSheet(u"QFrame{\n"
"	border:none;\n"
"	border-radius:30%;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"")
        self.frame_5.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_5.setFrameShadow(QFrame.Shadow.Raised)

        self.verticalLayout_4.addWidget(self.frame_5)

        self.tabWidget.addTab(self.tab, "")
        self.tab_2 = QWidget()
        self.tab_2.setObjectName(u"tab_2")
        self.verticalLayout_3 = QVBoxLayout(self.tab_2)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.frame_6 = QFrame(self.tab_2)
        self.frame_6.setObjectName(u"frame_6")
        self.frame_6.setStyleSheet(u"QFrame{\n"
"	border:none;\n"
"	border-radius:30%;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"")
        self.frame_6.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_6.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_6 = QHBoxLayout(self.frame_6)
        self.horizontalLayout_6.setObjectName(u"horizontalLayout_6")
        self.label_29 = QLabel(self.frame_6)
        self.label_29.setObjectName(u"label_29")
        self.label_29.setFont(font)
        self.label_29.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"	boarder-radius:20%;\n"
"	background-color: rgb(83, 26, 255);\n"
"}")
        self.label_29.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_6.addWidget(self.label_29)

        self.label_30 = QLabel(self.frame_6)
        self.label_30.setObjectName(u"label_30")
        self.label_30.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_6.addWidget(self.label_30, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_31 = QLabel(self.frame_6)
        self.label_31.setObjectName(u"label_31")
        self.label_31.setFont(font)
        self.label_31.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"	boarder-radius:20%;\n"
"	background-color: rgb(83, 26, 255);\n"
"}")
        self.label_31.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_6.addWidget(self.label_31)

        self.label_32 = QLabel(self.frame_6)
        self.label_32.setObjectName(u"label_32")
        self.label_32.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_6.addWidget(self.label_32, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_33 = QLabel(self.frame_6)
        self.label_33.setObjectName(u"label_33")
        self.label_33.setFont(font)
        self.label_33.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"	boarder-radius:20%;\n"
"	background-color: rgb(83, 26, 255);\n"
"}")
        self.label_33.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_6.addWidget(self.label_33)

        self.label_34 = QLabel(self.frame_6)
        self.label_34.setObjectName(u"label_34")
        self.label_34.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_6.addWidget(self.label_34, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_35 = QLabel(self.frame_6)
        self.label_35.setObjectName(u"label_35")
        self.label_35.setFont(font)
        self.label_35.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"}")
        self.label_35.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_6.addWidget(self.label_35)


        self.verticalLayout_3.addWidget(self.frame_6)

        self.frame_7 = QFrame(self.tab_2)
        self.frame_7.setObjectName(u"frame_7")
        sizePolicy.setHeightForWidth(self.frame_7.sizePolicy().hasHeightForWidth())
        self.frame_7.setSizePolicy(sizePolicy)
        self.frame_7.setStyleSheet(u"QFrame{\n"
"	border:none;\n"
"	border-radius:30%;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"")
        self.frame_7.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_7.setFrameShadow(QFrame.Shadow.Raised)

        self.verticalLayout_3.addWidget(self.frame_7)

        self.tabWidget.addTab(self.tab_2, "")
        self.tab_3 = QWidget()
        self.tab_3.setObjectName(u"tab_3")
        self.verticalLayout_2 = QVBoxLayout(self.tab_3)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.frame_8 = QFrame(self.tab_3)
        self.frame_8.setObjectName(u"frame_8")
        self.frame_8.setStyleSheet(u"QFrame{\n"
"	border:none;\n"
"	border-radius:30%;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"")
        self.frame_8.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_8.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_7 = QHBoxLayout(self.frame_8)
        self.horizontalLayout_7.setObjectName(u"horizontalLayout_7")
        self.label_36 = QLabel(self.frame_8)
        self.label_36.setObjectName(u"label_36")
        self.label_36.setFont(font)
        self.label_36.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"	boarder-radius:20%;\n"
"	background-color: rgb(83, 26, 255);\n"
"}")
        self.label_36.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_7.addWidget(self.label_36)

        self.label_37 = QLabel(self.frame_8)
        self.label_37.setObjectName(u"label_37")
        self.label_37.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_7.addWidget(self.label_37, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_38 = QLabel(self.frame_8)
        self.label_38.setObjectName(u"label_38")
        self.label_38.setFont(font)
        self.label_38.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"	boarder-radius:20%;\n"
"	background-color: rgb(83, 26, 255);\n"
"}")
        self.label_38.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_7.addWidget(self.label_38)

        self.label_39 = QLabel(self.frame_8)
        self.label_39.setObjectName(u"label_39")
        self.label_39.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_7.addWidget(self.label_39, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_40 = QLabel(self.frame_8)
        self.label_40.setObjectName(u"label_40")
        self.label_40.setFont(font)
        self.label_40.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"	boarder-radius:20%;\n"
"	background-color: rgb(83, 26, 255);\n"
"}")
        self.label_40.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_7.addWidget(self.label_40)

        self.label_41 = QLabel(self.frame_8)
        self.label_41.setObjectName(u"label_41")
        self.label_41.setPixmap(QPixmap(u":/Icons/Assets/Icons/icons8-forward-arrow-100.png"))

        self.horizontalLayout_7.addWidget(self.label_41, 0, Qt.AlignmentFlag.AlignHCenter)

        self.label_42 = QLabel(self.frame_8)
        self.label_42.setObjectName(u"label_42")
        self.label_42.setFont(font)
        self.label_42.setStyleSheet(u"QLabel{\n"
"	padding:20px;\n"
"	border:2px dashed white;\n"
"	boarder-radius:20%;\n"
"	background-color: rgb(83, 26, 255);\n"
"}")
        self.label_42.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_7.addWidget(self.label_42)


        self.verticalLayout_2.addWidget(self.frame_8)

        self.frame_9 = QFrame(self.tab_3)
        self.frame_9.setObjectName(u"frame_9")
        sizePolicy.setHeightForWidth(self.frame_9.sizePolicy().hasHeightForWidth())
        self.frame_9.setSizePolicy(sizePolicy)
        self.frame_9.setStyleSheet(u"QFrame{\n"
"	border:none;\n"
"	border-radius:30%;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"")
        self.frame_9.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_9.setFrameShadow(QFrame.Shadow.Raised)

        self.verticalLayout_2.addWidget(self.frame_9)

        self.tabWidget.addTab(self.tab_3, "")

        self.horizontalLayout.addWidget(self.tabWidget)


        self.retranslateUi(Form)
        self.horizontalSlider.valueChanged.connect(self.label_48.setNum)
        self.horizontalSlider_2.valueChanged.connect(self.label_50.setNum)
        self.horizontalSlider_3.valueChanged.connect(self.label_53.setNum)

        self.tabWidget.setCurrentIndex(0)


        QMetaObject.connectSlotsByName(Form)
    # setupUi

    def retranslateUi(self, Form):
        Form.setWindowTitle(QCoreApplication.translate("Form", u"Form", None))
        self.label.setText(QCoreApplication.translate("Form", u"STEP 1 :\n"
"Image\n"
"Optimization", None))
        self.label_2.setText("")
        self.label_3.setText(QCoreApplication.translate("Form", u"Step 2 :\n"
"Master Image\n"
"Registration", None))
        self.label_6.setText("")
        self.label_4.setText(QCoreApplication.translate("Form", u"Step 3 :\n"
"Teach Target", None))
        self.label_7.setText("")
        self.label_5.setText(QCoreApplication.translate("Form", u"Step 4 :\n"
" IO Config.", None))
        self.label_43.setText(QCoreApplication.translate("Form", u"TextLabel", None))
        self.groupBox.setTitle(QCoreApplication.translate("Form", u"Image Options", None))
        self.groupBox_2.setTitle(QCoreApplication.translate("Form", u"Trigger Options", None))
        self.pushButton.setText(QCoreApplication.translate("Form", u"Trigger", None))
        self.pushButton_2.setText(QCoreApplication.translate("Form", u"Save Current\n"
"Image", None))
        self.groupBox_3.setTitle(QCoreApplication.translate("Form", u"Image Options", None))
        self.label_46.setText(QCoreApplication.translate("Form", u"Image Type :", None))
        self.label_52.setText(QCoreApplication.translate("Form", u"Normalisation Value\n"
"(White):", None))
        self.label_48.setText(QCoreApplication.translate("Form", u"---", None))
        self.label_47.setText(QCoreApplication.translate("Form", u"Normalisation Value\n"
"(Black):", None))
        self.comboBox.setItemText(0, QCoreApplication.translate("Form", u"Normal", None))
        self.comboBox.setItemText(1, QCoreApplication.translate("Form", u"Gray Scale", None))
        self.comboBox.setItemText(2, QCoreApplication.translate("Form", u"Inverted", None))
        self.comboBox.setItemText(3, QCoreApplication.translate("Form", u"Normalised", None))

        self.label_49.setText(QCoreApplication.translate("Form", u"Smoothness Factor :", None))
        self.label_45.setText(QCoreApplication.translate("Form", u"X<->Y", None))
        self.label_51.setText(QCoreApplication.translate("Form", u"W<->H", None))
        self.pushButton_3.setText(QCoreApplication.translate("Form", u"Crop", None))
        self.checkBox.setText(QCoreApplication.translate("Form", u"Full Size", None))
        self.label_50.setText(QCoreApplication.translate("Form", u"---", None))
        self.label_53.setText(QCoreApplication.translate("Form", u"---", None))
        self.checkBox_2.setText(QCoreApplication.translate("Form", u"Activate Smooth", None))
        self.pushButton_4.setText(QCoreApplication.translate("Form", u"Save \n"
"Settings", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tabWidgetPage1), QCoreApplication.translate("Form", u"Image Optimization", None))
        self.label_22.setText(QCoreApplication.translate("Form", u"STEP 1 :\n"
"Image\n"
"Optimization", None))
        self.label_23.setText("")
        self.label_24.setText(QCoreApplication.translate("Form", u"Step 2 :\n"
"Master Image\n"
"Registration", None))
        self.label_25.setText("")
        self.label_26.setText(QCoreApplication.translate("Form", u"Step 3 :\n"
"Teach Target", None))
        self.label_27.setText("")
        self.label_28.setText(QCoreApplication.translate("Form", u"Step 4 :\n"
" IO Config.", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QCoreApplication.translate("Form", u"Master Image reg.", None))
        self.label_29.setText(QCoreApplication.translate("Form", u"STEP 1 :\n"
"Image\n"
"Optimization", None))
        self.label_30.setText("")
        self.label_31.setText(QCoreApplication.translate("Form", u"Step 2 :\n"
"Master Image\n"
"Registration", None))
        self.label_32.setText("")
        self.label_33.setText(QCoreApplication.translate("Form", u"Step 3 :\n"
"Teach Target", None))
        self.label_34.setText("")
        self.label_35.setText(QCoreApplication.translate("Form", u"Step 4 :\n"
" IO Config.", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), QCoreApplication.translate("Form", u"Teach Image ", None))
        self.label_36.setText(QCoreApplication.translate("Form", u"STEP 1 :\n"
"Image\n"
"Optimization", None))
        self.label_37.setText("")
        self.label_38.setText(QCoreApplication.translate("Form", u"Step 2 :\n"
"Master Image\n"
"Registration", None))
        self.label_39.setText("")
        self.label_40.setText(QCoreApplication.translate("Form", u"Step 3 :\n"
"Teach Target", None))
        self.label_41.setText("")
        self.label_42.setText(QCoreApplication.translate("Form", u"Step 4 :\n"
" IO Config.", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_3), QCoreApplication.translate("Form", u"IO Configure", None))
    # retranslateUi

