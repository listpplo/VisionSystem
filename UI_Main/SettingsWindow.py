# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'SettingsWindow.ui'
##
## Created by: Qt User Interface Compiler version 6.8.0
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
from PySide6.QtWidgets import (QAbstractItemView, QApplication, QCheckBox, QComboBox,
    QFrame, QGridLayout, QGroupBox, QHBoxLayout,
    QHeaderView, QLabel, QLineEdit, QPushButton,
    QScrollArea, QSizePolicy, QSpacerItem, QSpinBox,
    QTableWidget, QTableWidgetItem, QVBoxLayout, QWidget)

class Ui_SettingWindow(object):
    def setupUi(self, SettingWindow):
        if not SettingWindow.objectName():
            SettingWindow.setObjectName(u"SettingWindow")
        SettingWindow.resize(840, 533)
        SettingWindow.setStyleSheet(u"QWidget{\n"
"	color:white;\n"
"	background-color: rgb(50, 50, 50);\n"
"}")
        self.horizontalLayout = QHBoxLayout(SettingWindow)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.groupBox_4 = QGroupBox(SettingWindow)
        self.groupBox_4.setObjectName(u"groupBox_4")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.groupBox_4.sizePolicy().hasHeightForWidth())
        self.groupBox_4.setSizePolicy(sizePolicy)
        font = QFont()
        font.setBold(True)
        self.groupBox_4.setFont(font)
        self.groupBox_4.setStyleSheet(u"QGroupBox{\n"
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
        self.groupBox_4.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.groupBox_4.setFlat(False)
        self.verticalLayout = QVBoxLayout(self.groupBox_4)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(-1, 20, -1, -1)
        self.label = QLabel(self.groupBox_4)
        self.label.setObjectName(u"label")

        self.verticalLayout.addWidget(self.label)

        self.tableWidget = QTableWidget(self.groupBox_4)
        if (self.tableWidget.columnCount() < 5):
            self.tableWidget.setColumnCount(5)
        __qtablewidgetitem = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(0, __qtablewidgetitem)
        __qtablewidgetitem1 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(1, __qtablewidgetitem1)
        __qtablewidgetitem2 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(2, __qtablewidgetitem2)
        __qtablewidgetitem3 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(3, __qtablewidgetitem3)
        __qtablewidgetitem4 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(4, __qtablewidgetitem4)
        self.tableWidget.setObjectName(u"tableWidget")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Expanding)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.tableWidget.sizePolicy().hasHeightForWidth())
        self.tableWidget.setSizePolicy(sizePolicy1)
        self.tableWidget.setStyleSheet(u"QTableWidget{\n"
"	color:black;\n"
"	background-color:rgb(255, 255, 255);\n"
"}")
        self.tableWidget.setEditTriggers(QAbstractItemView.EditTrigger.NoEditTriggers)

        self.verticalLayout.addWidget(self.tableWidget)

        self.groupBox_3 = QGroupBox(self.groupBox_4)
        self.groupBox_3.setObjectName(u"groupBox_3")
        self.horizontalLayout_3 = QHBoxLayout(self.groupBox_3)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.horizontalLayout_3.setContentsMargins(-1, 20, -1, -1)
        self.label_10 = QLabel(self.groupBox_3)
        self.label_10.setObjectName(u"label_10")

        self.horizontalLayout_3.addWidget(self.label_10)

        self.comboBox = QComboBox(self.groupBox_3)
        self.comboBox.setObjectName(u"comboBox")
        self.comboBox.setMinimumSize(QSize(60, 30))

        self.horizontalLayout_3.addWidget(self.comboBox, 0, Qt.AlignmentFlag.AlignLeft)


        self.verticalLayout.addWidget(self.groupBox_3, 0, Qt.AlignmentFlag.AlignLeft)

        self.groupBox = QGroupBox(self.groupBox_4)
        self.groupBox.setObjectName(u"groupBox")
        self.groupBox.setStyleSheet(u"QGroupBox{\n"
"	border:none;\n"
"}\n"
"\n"
"QPushButton{\n"
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
        self.horizontalLayout_2 = QHBoxLayout(self.groupBox)
        self.horizontalLayout_2.setSpacing(0)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.pushButton_4 = QPushButton(self.groupBox)
        self.pushButton_4.setObjectName(u"pushButton_4")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Minimum)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.pushButton_4.sizePolicy().hasHeightForWidth())
        self.pushButton_4.setSizePolicy(sizePolicy2)
        self.pushButton_4.setMinimumSize(QSize(0, 40))

        self.horizontalLayout_2.addWidget(self.pushButton_4)

        self.pushButton_5 = QPushButton(self.groupBox)
        self.pushButton_5.setObjectName(u"pushButton_5")
        sizePolicy2.setHeightForWidth(self.pushButton_5.sizePolicy().hasHeightForWidth())
        self.pushButton_5.setSizePolicy(sizePolicy2)

        self.horizontalLayout_2.addWidget(self.pushButton_5)

        self.pushButton_6 = QPushButton(self.groupBox)
        self.pushButton_6.setObjectName(u"pushButton_6")
        sizePolicy2.setHeightForWidth(self.pushButton_6.sizePolicy().hasHeightForWidth())
        self.pushButton_6.setSizePolicy(sizePolicy2)

        self.horizontalLayout_2.addWidget(self.pushButton_6)


        self.verticalLayout.addWidget(self.groupBox)


        self.horizontalLayout.addWidget(self.groupBox_4)

        self.groupBox_5 = QGroupBox(SettingWindow)
        self.groupBox_5.setObjectName(u"groupBox_5")
        sizePolicy3 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Preferred)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.groupBox_5.sizePolicy().hasHeightForWidth())
        self.groupBox_5.setSizePolicy(sizePolicy3)
        self.groupBox_5.setFont(font)
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
"	color:rgb(0, 0,0);\n"
"	background-color:none;\n"
"}\n"
"\n"
"QPushButton{\n"
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
        self.groupBox_5.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.groupBox_5.setFlat(False)
        self.verticalLayout_2 = QVBoxLayout(self.groupBox_5)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.verticalLayout_2.setContentsMargins(-1, 20, -1, -1)
        self.groupBox_6 = QGroupBox(self.groupBox_5)
        self.groupBox_6.setObjectName(u"groupBox_6")
        self.gridLayout_3 = QGridLayout(self.groupBox_6)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.pushButton_8 = QPushButton(self.groupBox_6)
        self.pushButton_8.setObjectName(u"pushButton_8")
        sizePolicy4 = QSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)
        sizePolicy4.setHorizontalStretch(0)
        sizePolicy4.setVerticalStretch(0)
        sizePolicy4.setHeightForWidth(self.pushButton_8.sizePolicy().hasHeightForWidth())
        self.pushButton_8.setSizePolicy(sizePolicy4)
        self.pushButton_8.setMinimumSize(QSize(100, 0))

        self.gridLayout_3.addWidget(self.pushButton_8, 2, 0, 1, 1)

        self.pushButton_9 = QPushButton(self.groupBox_6)
        self.pushButton_9.setObjectName(u"pushButton_9")
        sizePolicy5 = QSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)
        sizePolicy5.setHorizontalStretch(0)
        sizePolicy5.setVerticalStretch(0)
        sizePolicy5.setHeightForWidth(self.pushButton_9.sizePolicy().hasHeightForWidth())
        self.pushButton_9.setSizePolicy(sizePolicy5)
        self.pushButton_9.setMinimumSize(QSize(100, 0))

        self.gridLayout_3.addWidget(self.pushButton_9, 2, 1, 1, 1)

        self.label_7 = QLabel(self.groupBox_6)
        self.label_7.setObjectName(u"label_7")

        self.gridLayout_3.addWidget(self.label_7, 0, 0, 1, 1, Qt.AlignmentFlag.AlignRight)

        self.label_8 = QLabel(self.groupBox_6)
        self.label_8.setObjectName(u"label_8")

        self.gridLayout_3.addWidget(self.label_8, 1, 0, 1, 1, Qt.AlignmentFlag.AlignRight)

        self.spinBox_3 = QSpinBox(self.groupBox_6)
        self.spinBox_3.setObjectName(u"spinBox_3")
        self.spinBox_3.setMinimumSize(QSize(100, 30))
        self.spinBox_3.setMaximum(9999)

        self.gridLayout_3.addWidget(self.spinBox_3, 1, 1, 1, 1, Qt.AlignmentFlag.AlignLeft)

        self.spinBox_2 = QSpinBox(self.groupBox_6)
        self.spinBox_2.setObjectName(u"spinBox_2")
        self.spinBox_2.setMinimumSize(QSize(100, 30))
        self.spinBox_2.setMaximum(99999)

        self.gridLayout_3.addWidget(self.spinBox_2, 0, 1, 1, 1, Qt.AlignmentFlag.AlignLeft)


        self.verticalLayout_2.addWidget(self.groupBox_6, 0, Qt.AlignmentFlag.AlignHCenter)

        self.frame = QFrame(self.groupBox_5)
        self.frame.setObjectName(u"frame")
        sizePolicy6 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        sizePolicy6.setHorizontalStretch(0)
        sizePolicy6.setVerticalStretch(0)
        sizePolicy6.setHeightForWidth(self.frame.sizePolicy().hasHeightForWidth())
        self.frame.setSizePolicy(sizePolicy6)
        self.frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_3 = QVBoxLayout(self.frame)
        self.verticalLayout_3.setSpacing(0)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.verticalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.scrollArea = QScrollArea(self.frame)
        self.scrollArea.setObjectName(u"scrollArea")
        self.scrollArea.setWidgetResizable(True)
        self.scrollAreaWidgetContents = QWidget()
        self.scrollAreaWidgetContents.setObjectName(u"scrollAreaWidgetContents")
        self.scrollAreaWidgetContents.setGeometry(QRect(0, 0, 227, 338))
        self.horizontalLayout_4 = QHBoxLayout(self.scrollAreaWidgetContents)
        self.horizontalLayout_4.setSpacing(0)
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.horizontalLayout_4.setContentsMargins(0, 0, 0, 0)
        self.label_9 = QLabel(self.scrollAreaWidgetContents)
        self.label_9.setObjectName(u"label_9")
        self.label_9.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.horizontalLayout_4.addWidget(self.label_9)

        self.scrollArea.setWidget(self.scrollAreaWidgetContents)

        self.verticalLayout_3.addWidget(self.scrollArea)


        self.verticalLayout_2.addWidget(self.frame)


        self.horizontalLayout.addWidget(self.groupBox_5)

        self.groupBox_2 = QGroupBox(SettingWindow)
        self.groupBox_2.setObjectName(u"groupBox_2")
        self.groupBox_2.setFont(font)
        self.groupBox_2.setStyleSheet(u"QGroupBox{\n"
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
"	color:rgb(0, 0,0);\n"
"	background-color:rgb(170, 170, 170);\n"
"}\n"
"\n"
"QPushButton{\n"
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
        self.groupBox_2.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.groupBox_2.setFlat(False)
        self.gridLayout = QGridLayout(self.groupBox_2)
        self.gridLayout.setObjectName(u"gridLayout")
        self.gridLayout.setContentsMargins(-1, 20, -1, -1)
        self.label_5 = QLabel(self.groupBox_2)
        self.label_5.setObjectName(u"label_5")

        self.gridLayout.addWidget(self.label_5, 3, 0, 1, 1)

        self.lineEdit_4 = QLineEdit(self.groupBox_2)
        self.lineEdit_4.setObjectName(u"lineEdit_4")
        self.lineEdit_4.setMinimumSize(QSize(0, 30))
        self.lineEdit_4.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.lineEdit_4, 5, 1, 1, 1)

        self.label_2 = QLabel(self.groupBox_2)
        self.label_2.setObjectName(u"label_2")

        self.gridLayout.addWidget(self.label_2, 0, 0, 1, 1)

        self.label_6 = QLabel(self.groupBox_2)
        self.label_6.setObjectName(u"label_6")

        self.gridLayout.addWidget(self.label_6, 4, 0, 1, 1)

        self.pushButton_2 = QPushButton(self.groupBox_2)
        self.pushButton_2.setObjectName(u"pushButton_2")

        self.gridLayout.addWidget(self.pushButton_2, 7, 1, 1, 1)

        self.label_4 = QLabel(self.groupBox_2)
        self.label_4.setObjectName(u"label_4")

        self.gridLayout.addWidget(self.label_4, 2, 0, 1, 1)

        self.label_3 = QLabel(self.groupBox_2)
        self.label_3.setObjectName(u"label_3")

        self.gridLayout.addWidget(self.label_3, 1, 0, 1, 1)

        self.pushButton_3 = QPushButton(self.groupBox_2)
        self.pushButton_3.setObjectName(u"pushButton_3")
        sizePolicy2.setHeightForWidth(self.pushButton_3.sizePolicy().hasHeightForWidth())
        self.pushButton_3.setSizePolicy(sizePolicy2)
        self.pushButton_3.setMinimumSize(QSize(0, 40))

        self.gridLayout.addWidget(self.pushButton_3, 8, 0, 1, 1)

        self.lineEdit = QLineEdit(self.groupBox_2)
        self.lineEdit.setObjectName(u"lineEdit")
        self.lineEdit.setMinimumSize(QSize(0, 30))
        self.lineEdit.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.lineEdit, 0, 1, 1, 1)

        self.spinBox = QSpinBox(self.groupBox_2)
        self.spinBox.setObjectName(u"spinBox")
        sizePolicy7 = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Preferred)
        sizePolicy7.setHorizontalStretch(0)
        sizePolicy7.setVerticalStretch(0)
        sizePolicy7.setHeightForWidth(self.spinBox.sizePolicy().hasHeightForWidth())
        self.spinBox.setSizePolicy(sizePolicy7)
        self.spinBox.setMinimumSize(QSize(0, 30))
        self.spinBox.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.spinBox.setMaximum(99999)

        self.gridLayout.addWidget(self.spinBox, 1, 1, 1, 1)

        self.pushButton = QPushButton(self.groupBox_2)
        self.pushButton.setObjectName(u"pushButton")
        sizePolicy2.setHeightForWidth(self.pushButton.sizePolicy().hasHeightForWidth())
        self.pushButton.setSizePolicy(sizePolicy2)

        self.gridLayout.addWidget(self.pushButton, 7, 0, 1, 1)

        self.comboBox_2 = QComboBox(self.groupBox_2)
        self.comboBox_2.addItem("")
        self.comboBox_2.addItem("")
        self.comboBox_2.setObjectName(u"comboBox_2")
        self.comboBox_2.setMinimumSize(QSize(0, 30))

        self.gridLayout.addWidget(self.comboBox_2, 2, 1, 1, 1)

        self.lineEdit_2 = QLineEdit(self.groupBox_2)
        self.lineEdit_2.setObjectName(u"lineEdit_2")
        self.lineEdit_2.setMinimumSize(QSize(0, 30))
        self.lineEdit_2.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.lineEdit_2, 3, 1, 1, 1)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.gridLayout.addItem(self.verticalSpacer, 9, 1, 1, 1)

        self.label_11 = QLabel(self.groupBox_2)
        self.label_11.setObjectName(u"label_11")

        self.gridLayout.addWidget(self.label_11, 5, 0, 1, 1)

        self.lineEdit_3 = QLineEdit(self.groupBox_2)
        self.lineEdit_3.setObjectName(u"lineEdit_3")
        self.lineEdit_3.setMinimumSize(QSize(0, 30))
        self.lineEdit_3.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.gridLayout.addWidget(self.lineEdit_3, 4, 1, 1, 1)

        self.checkBox = QCheckBox(self.groupBox_2)
        self.checkBox.setObjectName(u"checkBox")
        self.checkBox.setStyleSheet(u"QCheckBox{\n"
"	background:none;\n"
"}")

        self.gridLayout.addWidget(self.checkBox, 6, 1, 1, 1)


        self.horizontalLayout.addWidget(self.groupBox_2, 0, Qt.AlignmentFlag.AlignRight)


        self.retranslateUi(SettingWindow)

        QMetaObject.connectSlotsByName(SettingWindow)
    # setupUi

    def retranslateUi(self, SettingWindow):
        SettingWindow.setWindowTitle(QCoreApplication.translate("SettingWindow", u"Settings", None))
        self.groupBox_4.setTitle(QCoreApplication.translate("SettingWindow", u"Camera Settings", None))
        self.label.setText(QCoreApplication.translate("SettingWindow", u"Available Cameras :", None))
        ___qtablewidgetitem = self.tableWidget.horizontalHeaderItem(0)
        ___qtablewidgetitem.setText(QCoreApplication.translate("SettingWindow", u"Ip", None));
        ___qtablewidgetitem1 = self.tableWidget.horizontalHeaderItem(1)
        ___qtablewidgetitem1.setText(QCoreApplication.translate("SettingWindow", u"Type", None));
        ___qtablewidgetitem2 = self.tableWidget.horizontalHeaderItem(2)
        ___qtablewidgetitem2.setText(QCoreApplication.translate("SettingWindow", u"Vendor Name", None));
        ___qtablewidgetitem3 = self.tableWidget.horizontalHeaderItem(3)
        ___qtablewidgetitem3.setText(QCoreApplication.translate("SettingWindow", u"Serial No.", None));
        ___qtablewidgetitem4 = self.tableWidget.horizontalHeaderItem(4)
        ___qtablewidgetitem4.setText(QCoreApplication.translate("SettingWindow", u"Camera Name", None));
        self.groupBox_3.setTitle(QCoreApplication.translate("SettingWindow", u"Camera Selection :", None))
        self.label_10.setText(QCoreApplication.translate("SettingWindow", u"Select Camera Index  :", None))
        self.pushButton_4.setText(QCoreApplication.translate("SettingWindow", u"Save", None))
        self.pushButton_5.setText(QCoreApplication.translate("SettingWindow", u"Refresh", None))
        self.pushButton_6.setText(QCoreApplication.translate("SettingWindow", u"Save Image", None))
        self.groupBox_5.setTitle(QCoreApplication.translate("SettingWindow", u"Image Settings", None))
        self.groupBox_6.setTitle("")
        self.pushButton_8.setText(QCoreApplication.translate("SettingWindow", u"Capture Sample\n"
" Image", None))
        self.pushButton_9.setText(QCoreApplication.translate("SettingWindow", u"Save", None))
        self.label_7.setText(QCoreApplication.translate("SettingWindow", u"Height :", None))
        self.label_8.setText(QCoreApplication.translate("SettingWindow", u"Width :", None))
        self.label_9.setText(QCoreApplication.translate("SettingWindow", u"Image Will Appear Here", None))
        self.groupBox_2.setTitle(QCoreApplication.translate("SettingWindow", u"PLC Settings", None))
        self.label_5.setText(QCoreApplication.translate("SettingWindow", u"Mapping Adress:", None))
        self.label_2.setText(QCoreApplication.translate("SettingWindow", u"Enter PLC IP :", None))
        self.label_6.setText(QCoreApplication.translate("SettingWindow", u"Trigger Adress:", None))
        self.pushButton_2.setText(QCoreApplication.translate("SettingWindow", u"Open Connection\n"
"Settings", None))
        self.label_4.setText(QCoreApplication.translate("SettingWindow", u"Select Communication\n"
"Protocol :", None))
        self.label_3.setText(QCoreApplication.translate("SettingWindow", u"Enter PLC Port :", None))
        self.pushButton_3.setText(QCoreApplication.translate("SettingWindow", u"Save", None))
        self.pushButton.setText(QCoreApplication.translate("SettingWindow", u"Test Connection", None))
        self.comboBox_2.setItemText(0, QCoreApplication.translate("SettingWindow", u"Modbus", None))
        self.comboBox_2.setItemText(1, QCoreApplication.translate("SettingWindow", u"Melsec Protocol", None))

        self.label_11.setText(QCoreApplication.translate("SettingWindow", u"Feedback Adress :", None))
        self.checkBox.setText(QCoreApplication.translate("SettingWindow", u"No PLC Mode", None))
    # retranslateUi

