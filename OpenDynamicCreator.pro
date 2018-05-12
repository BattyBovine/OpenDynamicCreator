#-------------------------------------------------
#
# Project created by QtCreator 2018-02-21T23:57:50
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets
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
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

LIBS += -logg -lvorbis -lvorbisfile

SOURCES += main.cpp \
	OpenDynamicCreator.cpp \
	src/StatePropertiesEditor.cpp \
	src/Widgets/MusicTreeView.cpp \
	src/Widgets/StatesTreeView.cpp \
	src/Widgets/TimelineWidget.cpp \
	src/Commands/OdcUndoCommands.cpp \
	src/ClipGroupEditorWidget.cpp \
	src/MusicPropertiesWidget.cpp \
	src/ClipEditorWidget.cpp \
    src/Widgets/MixerWidget.cpp \
    src/Widgets/MixerVolumeSlider.cpp \
    src/Widgets/MixerPanSlider.cpp \
    src/Widgets/GraphicsItems/PlayMarkerItem.cpp \
    src/Widgets/GraphicsItems/TimelineItem.cpp \
    src/Widgets/GraphicsItems/ClipTimelineItem.cpp \
    src/Classes/ClipContainer.cpp

HEADERS  += version.h \
	OpenDynamicCreator.h \
	src/StatePropertiesEditor.h \
	src/Widgets/MusicTreeView.h \
	src/Widgets/StatesTreeView.h \
	src/Widgets/TimelineWidget.h \
	src/Commands/OdcUndoCommands.h \
	src/ClipGroupEditorWidget.h \
	src/MusicPropertiesWidget.h \
	src/ClipEditorWidget.h \
	src/Widgets/MixerWidget.h \
	src/Widgets/MixerVolumeSlider.h \
	src/Widgets/MixerPanSlider.h \
	src/Classes/MusicEvent.h \
	src/Libraries/dr_wav.h \
    src/Widgets/GraphicsItems/PlayMarkerItem.h \
    src/Widgets/GraphicsItems/TimelineItem.h \
    src/Widgets/GraphicsItems/ClipTimelineItem.h \
    src/Classes/ClipContainer.h

FORMS    += \
	OpenDynamicCreator.ui \
	ui/ClipGroupEditorWidget.ui \
	ui/MusicPropertiesWidget.ui \
    ui/StatePropertiesEditor.ui \
    ui/ClipEditorWidget.ui \
    ui/Widgets/MixerWidget.ui

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
