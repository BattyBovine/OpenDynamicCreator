#-------------------------------------------------
#
# Project created by QtCreator 2018-02-21T23:57:50
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenDynamicCreator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
	src/OpenDynamicCreator.cpp \
    src/qt/MusicTreeView.cpp

HEADERS  += version.h \
	src/OpenDynamicCreator.h \
    src/qt/MusicTreeView.h

FORMS    += \
    ui/OpenDynamicCreator.ui

DISTFILES +=

RESOURCES += \ 
	icons.qrc

#win32:RC_ICONS += res/icon.ico
#ICON = res/icon.icns

QMAKE_TARGET_PRODUCT = "OpenDynamic Creator"
QMAKE_TARGET_COMPANY = "Batty Bovine Productions, LLC"
QMAKE_TARGET_COPYRIGHT = "(c) 2018 Batty Bovine Productions, LLC. All Rights Reserved."
GENERATED_VERSION_NUMBER = $$system(perl versionup.pl -get)
VERSION = $${GENERATED_VERSION_NUMBER}
