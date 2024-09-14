# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'CreateOrModifyFile.ui'
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
from PySide6.QtWidgets import (QApplication, QComboBox, QGridLayout, QGroupBox,
    QLabel, QLineEdit, QPushButton, QSizePolicy,
    QSpacerItem, QVBoxLayout, QWidget)
import StaticAssets_rc

class Ui_CreateorModifyRecipe(object):
    def setupUi(self, CreateorModifyRecipe):
        if not CreateorModifyRecipe.objectName():
            CreateorModifyRecipe.setObjectName(u"CreateorModifyRecipe")
        CreateorModifyRecipe.resize(544, 302)
        CreateorModifyRecipe.setStyleSheet(u"QWidget{\n"
"	color:white;\n"
"	background-color: rgb(50, 50, 50);\n"
"}\n"
"QLabel{\n"
"	color: rgb(255, 255, 255);\n"
"	background-color:none;\n"
"}\n"
"\n"
"QLineEdit{\n"
"	color:black;\n"
"	background-color:none;\n"
"}")
        self.verticalLayout = QVBoxLayout(CreateorModifyRecipe)
        self.verticalLayout.setSpacing(6)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.groupBox_2 = QGroupBox(CreateorModifyRecipe)
        self.groupBox_2.setObjectName(u"groupBox_2")
        font = QFont()
        font.setBold(True)
        self.groupBox_2.setFont(font)
        self.groupBox_2.setStyleSheet(u"QGroupBox{\n"
"	border:2px dashed black;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"")
        self.groupBox_2.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.gridLayout_2 = QGridLayout(self.groupBox_2)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.gridLayout_2.setContentsMargins(-1, 20, -1, -1)
        self.label_3 = QLabel(self.groupBox_2)
        self.label_3.setObjectName(u"label_3")
        font1 = QFont()
        font1.setPointSize(10)
        self.label_3.setFont(font1)

        self.gridLayout_2.addWidget(self.label_3, 0, 0, 1, 1)

        self.pushButton_2 = QPushButton(self.groupBox_2)
        self.pushButton_2.setObjectName(u"pushButton_2")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.pushButton_2.sizePolicy().hasHeightForWidth())
        self.pushButton_2.setSizePolicy(sizePolicy)
        self.pushButton_2.setMinimumSize(QSize(203, 50))
        self.pushButton_2.setMaximumSize(QSize(203, 16777215))
        self.pushButton_2.setFont(font1)
        self.pushButton_2.setStyleSheet(u"QPushButton{\n"
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
        icon.addFile(u":/Icons/Assets/Icons/icons8-modify-128.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.pushButton_2.setIcon(icon)
        self.pushButton_2.setIconSize(QSize(40, 40))

        self.gridLayout_2.addWidget(self.pushButton_2, 2, 1, 1, 1, Qt.AlignmentFlag.AlignRight)

        self.comboBox_2 = QComboBox(self.groupBox_2)
        self.comboBox_2.setObjectName(u"comboBox_2")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.comboBox_2.sizePolicy().hasHeightForWidth())
        self.comboBox_2.setSizePolicy(sizePolicy1)
        self.comboBox_2.setMinimumSize(QSize(0, 40))
        self.comboBox_2.setFont(font1)
        self.comboBox_2.setStyleSheet(u"QComboBox{\n"
"	\n"
"}")

        self.gridLayout_2.addWidget(self.comboBox_2, 0, 1, 1, 1)


        self.verticalLayout.addWidget(self.groupBox_2)

        self.groupBox = QGroupBox(CreateorModifyRecipe)
        self.groupBox.setObjectName(u"groupBox")
        self.groupBox.setFont(font)
        self.groupBox.setStyleSheet(u"QGroupBox{\n"
"	border:2px dashed black;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(110, 110, 110);\n"
"}\n"
"")
        self.groupBox.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.gridLayout = QGridLayout(self.groupBox)
        self.gridLayout.setObjectName(u"gridLayout")
        self.gridLayout.setContentsMargins(-1, 20, -1, -1)
        self.comboBox = QComboBox(self.groupBox)
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.setObjectName(u"comboBox")
        self.comboBox.setMinimumSize(QSize(0, 40))
        self.comboBox.setFont(font1)
        self.comboBox.setStyleSheet(u"QComboBox{\n"
"	\n"
"}")

        self.gridLayout.addWidget(self.comboBox, 1, 1, 1, 1)

        self.label = QLabel(self.groupBox)
        self.label.setObjectName(u"label")
        self.label.setFont(font1)

        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)

        self.label_2 = QLabel(self.groupBox)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setFont(font1)

        self.gridLayout.addWidget(self.label_2, 1, 0, 1, 1)

        self.lineEdit = QLineEdit(self.groupBox)
        self.lineEdit.setObjectName(u"lineEdit")
        self.lineEdit.setFont(font1)
        self.lineEdit.setCursor(QCursor(Qt.CursorShape.IBeamCursor))

        self.gridLayout.addWidget(self.lineEdit, 0, 1, 1, 1)

        self.pushButton = QPushButton(self.groupBox)
        self.pushButton.setObjectName(u"pushButton")
        sizePolicy.setHeightForWidth(self.pushButton.sizePolicy().hasHeightForWidth())
        self.pushButton.setSizePolicy(sizePolicy)
        self.pushButton.setMinimumSize(QSize(203, 50))
        self.pushButton.setMaximumSize(QSize(203, 16777215))
        self.pushButton.setFont(font1)
        self.pushButton.setStyleSheet(u"QPushButton{\n"
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
        icon1 = QIcon()
        icon1.addFile(u":/Icons/Assets/Icons/icons8-add-96.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.pushButton.setIcon(icon1)
        self.pushButton.setIconSize(QSize(40, 40))

        self.gridLayout.addWidget(self.pushButton, 2, 1, 1, 1, Qt.AlignmentFlag.AlignRight)


        self.verticalLayout.addWidget(self.groupBox)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer)


        self.retranslateUi(CreateorModifyRecipe)

        QMetaObject.connectSlotsByName(CreateorModifyRecipe)
    # setupUi

    def retranslateUi(self, CreateorModifyRecipe):
        CreateorModifyRecipe.setWindowTitle(QCoreApplication.translate("CreateorModifyRecipe", u"Create Or Modify", None))
        self.groupBox_2.setTitle(QCoreApplication.translate("CreateorModifyRecipe", u"Modify Program", None))
        self.label_3.setText(QCoreApplication.translate("CreateorModifyRecipe", u"Select Program Name", None))
        self.pushButton_2.setText(QCoreApplication.translate("CreateorModifyRecipe", u"Modify Existing", None))
        self.groupBox.setTitle(QCoreApplication.translate("CreateorModifyRecipe", u"Create New Program", None))
        self.comboBox.setItemText(0, QCoreApplication.translate("CreateorModifyRecipe", u"Image Compare", None))
        self.comboBox.setItemText(1, QCoreApplication.translate("CreateorModifyRecipe", u"Learning Mode", None))
        self.comboBox.setItemText(2, QCoreApplication.translate("CreateorModifyRecipe", u"Sorting Mode", None))

        self.label.setText(QCoreApplication.translate("CreateorModifyRecipe", u"Enter Program Name :", None))
        self.label_2.setText(QCoreApplication.translate("CreateorModifyRecipe", u"Select Program Type :", None))
        self.pushButton.setText(QCoreApplication.translate("CreateorModifyRecipe", u"Create", None))
    # retranslateUi

