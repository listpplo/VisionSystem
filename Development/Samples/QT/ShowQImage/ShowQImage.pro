#-------------------------------------------------
#
# Project created by QtCreator 2017-01-09T10:03:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShowQImage
CONFIG(debug,debug|release) {
    TARGET = $$join(TARGET,,,_d)
}


Debug {
	contains(QMAKE_COMPILER_DEFINES, _WIN64) {
		LIBS += -L./Depends/x64/vs2013shared				-lMVSDKmd
		DESTDIR = ./Bin/x64
	}
	else {
		LIBS += -L./Depends/win32/vs2013shared				-lMVSDKmd
		DESTDIR = ./Bin/win32
	}
}
else {
	contains(QMAKE_COMPILER_DEFINES, _WIN64) {
		LIBS += -L./Depends/x64/vs2013shared				-lMVSDKmd
		DESTDIR = ./Bin/x64
	}
	else {
		LIBS += -L./Depends/win32/vs2013shared				-lMVSDKmd
		DESTDIR = ./Bin/win32
    }
}

			
INCLUDEPATH = ./Include

SOURCES += 	Src/main.cpp\
			Src/CammerWidget.cpp\
			Src/form.cpp

HEADERS += 	Src/CammerWidget.h\
			Src/MessageQue.h\
			Src/form.h

FORMS   += 	Src/cammerwidget.ui \
			Src/form.ui





