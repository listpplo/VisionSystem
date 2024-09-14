# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'MesgWindow.ui'
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
from PySide6.QtWidgets import (QAbstractButton, QApplication, QDialogButtonBox, QFrame,
    QLabel, QSizePolicy, QVBoxLayout, QWidget)

class Ui_MesgWindow(object):
    def setupUi(self, MesgWindow):
        if not MesgWindow.objectName():
            MesgWindow.setObjectName(u"MesgWindow")
        MesgWindow.resize(339, 124)
        MesgWindow.setWindowOpacity(0.900000000000000)
        MesgWindow.setStyleSheet(u"QWidget{\n"
"	color:white;\n"
"	background-color: rgb(50, 50, 50);\n"
"}")
        self.verticalLayout = QVBoxLayout(MesgWindow)
        self.verticalLayout.setSpacing(0)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 9)
        self.frame = QFrame(MesgWindow)
        self.frame.setObjectName(u"frame")
        self.frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_2 = QVBoxLayout(self.frame)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.MesgHeading = QLabel(self.frame)
        self.MesgHeading.setObjectName(u"MesgHeading")
        font = QFont()
        font.setPointSize(18)
        self.MesgHeading.setFont(font)
        self.MesgHeading.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.verticalLayout_2.addWidget(self.MesgHeading)

        self.Mesg = QLabel(self.frame)
        self.Mesg.setObjectName(u"Mesg")
        self.Mesg.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.verticalLayout_2.addWidget(self.Mesg)


        self.verticalLayout.addWidget(self.frame)

        self.buttonBox = QDialogButtonBox(MesgWindow)
        self.buttonBox.setObjectName(u"buttonBox")
        self.buttonBox.setStandardButtons(QDialogButtonBox.StandardButton.Ok)
        self.buttonBox.setCenterButtons(True)

        self.verticalLayout.addWidget(self.buttonBox)


        self.retranslateUi(MesgWindow)

        QMetaObject.connectSlotsByName(MesgWindow)
    # setupUi

    def retranslateUi(self, MesgWindow):
        MesgWindow.setWindowTitle(QCoreApplication.translate("MesgWindow", u"Mesg Window", None))
        self.MesgHeading.setText(QCoreApplication.translate("MesgWindow", u"TextLabel", None))
        self.Mesg.setText(QCoreApplication.translate("MesgWindow", u"Mesg", None))
    # retranslateUi

