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

win32 {
	contains(QT_ARCH, i386) { LIBS += -logg -lvorbis -lvorbisfile }
	contains(QT_ARCH, x86_64) { LIBS += -logg64 -lvorbis64 -lvorbisfile64 }
}

SOURCES += main.cpp \
	OpenDynamicCreator.cpp \
	src/StatePropertiesEditor.cpp \
	src/Widgets/MusicTreeView.cpp \
	src/Widgets/TimelineWidget.cpp \
	src/Commands/OdcUndoCommands.cpp \
	src/ClipGroupEditorWidget.cpp \
	src/MusicPropertiesWidget.cpp \
	src/ClipEditorWidget.cpp \
	src/PreferencesDialog.cpp \
    src/Widgets/MixerWidget.cpp \
    src/Widgets/MixerVolumeSlider.cpp \
    src/Widgets/MixerPanSlider.cpp \
    src/Widgets/GraphicsItems/PlayMarkerItem.cpp \
    src/Widgets/GraphicsItems/TimelineItem.cpp \
	src/Widgets/GraphicsItems/ClipTimelineItem.cpp \
	src/Widgets/GraphicsItems/InvertedLineItem.cpp \
    src/Classes/ClipContainer.cpp \
	src/Classes/WaveformThread.cpp \
    src/Widgets/GraphicsItems/EventMarkerItem.cpp \
    src/Classes/SongPlayer.cpp \
    src/Classes/MusicEvent.cpp \
    src/EventEditorWidget.cpp \
    src/Widgets/EventTreeView.cpp \
    src/Classes/EventCommand.cpp

HEADERS  += version.h \
	OpenDynamicCreator.h \
	src/StatePropertiesEditor.h \
	src/Widgets/MusicTreeView.h \
	src/Widgets/TimelineWidget.h \
	src/Commands/OdcUndoCommands.h \
	src/ClipGroupEditorWidget.h \
	src/MusicPropertiesWidget.h \
	src/ClipEditorWidget.h \
	src/PreferencesDialog.h \
	src/Widgets/MixerWidget.h \
	src/Widgets/MixerVolumeSlider.h \
	src/Widgets/MixerPanSlider.h \
	src/Classes/MusicEvent.h \
	src/Libraries/dr_wav.h \
    src/Widgets/GraphicsItems/PlayMarkerItem.h \
    src/Widgets/GraphicsItems/TimelineItem.h \
	src/Widgets/GraphicsItems/ClipTimelineItem.h \
	src/Widgets/GraphicsItems/InvertedLineItem.h \
    src/Classes/ClipContainer.h \
	src/Classes/WaveformThread.h \
    src/Classes/Beat.h \
    src/Widgets/GraphicsItems/EventMarkerItem.h \
    src/Classes/SongPlayer.h \
    src/EventEditorWidget.h \
    src/Widgets/EventTreeView.h \
    src/Classes/StaticMusicEvent.h \
    src/Classes/EventCommand.h \
    src/Classes/MusicEventWorker.h

FORMS    += \
	OpenDynamicCreator.ui \
	ui/ClipGroupEditorWidget.ui \
	ui/MusicPropertiesWidget.ui \
    ui/StatePropertiesEditor.ui \
    ui/ClipEditorWidget.ui \
    ui/Widgets/MixerWidget.ui \
    ui/PreferencesDialog.ui \
    ui/EventEditorWidget.ui

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
