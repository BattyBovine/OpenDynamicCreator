#ifndef OPENADAPTIVE_H
#define OPENADAPTIVE_H

#include <QMainWindow>
#include <QScrollArea>
#include <QScrollBar>
#include <QLayout>
#include <QSpacerItem>
#include <QUndoStack>
#include <QMessageBox>
#include <QFileInfo>

#include "PreferencesDialog.h"
#include "MusicPropertiesWidget.h"
#include "ClipGroupEditorWidget.h"
#include "ClipEditorWidget.h"

#include "Classes/SongPlayer.h"

#include "Widgets/MixerWidget.h"
#include "Widgets/MusicTreeView.h"
#include "Widgets/EventTreeView.h"

#define ODC_MUSIC_LABEL			tr("Song")
#define ODC_CLIP_GROUP_LABEL	tr("Mixer")
#define ODC_CLIP_LABEL			tr("Clip")

#define ODC_EVENT_LABEL			tr("Event")
#define ODC_EVENT_COMMAND_LABEL	tr("Command")

#define ODC_TITLE_ADD_CLIP_NO_MUSIC				tr("No tracks")
#define ODC_TEXT_ADD_CLIP_NO_MUSIC				tr("Please create a music track first.")
#define ODC_TITLE_ADD_CLIP_NO_SELECTION			tr("Nothing selected")
#define ODC_TEXT_ADD_CLIP_NO_SELECTION			tr("Please select a music track or clip group first.")
#define ODC_TITLE_ADD_CLIP_INVALID_SELECTION	tr("Invalid selection")
#define ODC_TEXT_ADD_CLIP_INVALID_SELECTION		tr("You have somehow selected something that has no valid data.")

#define ODC_TITLE_ADD_EVENT_NO_EVENT			tr("No events")
#define ODC_TEXT_ADD_EVENT_NO_EVENT				tr("Please create an event first.")
#define ODC_TITLE_ADD_EVENT_NO_SELECTION		tr("Nothing selected")
#define ODC_TEXT_ADD_EVENT_NO_SELECTION			tr("Please select an event first.")
#define ODC_TITLE_ADD_EVENT_INVALID_SELECTION	tr("Invalid selection")
#define ODC_TEXT_ADD_EVENT_INVALID_SELECTION	tr("You have somehow selected something that has no valid data.")

#define ODC_WINDOW_STATE	"WindowState"
#define ODC_WINDOW_GEOMETRY	"WindowGeometry"


namespace Ui {
class OpenDynamicCreator;
}

class OpenDynamicCreator : public QMainWindow
{
	Q_OBJECT

public:
	explicit OpenDynamicCreator(QWidget *parent=Q_NULLPTR);
	~OpenDynamicCreator();

public slots:
	void openPreferencesDialog();
	void closePreferencesDialog(int);

	void addTrack();
	void addClipGroup();
	void addClip(QModelIndex parent=QModelIndex(), QString file="", QString label="");
	void addClipFileDialog(QModelIndex parent=QModelIndex());
	void addClipList(QModelIndex,QStringList);
	void deleteMusicItem();
	QStandardItem *checkSelectedMusicTreeItem();

	void addEvent();
	void addEventCommand();
	void deleteEvent();
	QStandardItem *checkSelectedEventTreeItem();

	void swapEditorWidget(QItemSelection);
	void customContextMenuMusic(QPoint);
	void showAboutDialogue();

	void playSong(bool);
	void stopSong();

protected:
	void closeEvent(QCloseEvent*) override;

private:
	Ui::OpenDynamicCreator *ui;

	void loadTrackEditorWidget(QModelIndex);
	void loadClipGroupEditorWidget(QModelIndex);
	void loadClipEditorWidget(QModelIndex);
	QModelIndex findTrack(QModelIndex);

	PreferencesDialog *dialogPreferences;
	MusicTreeViewModel *modelMusic;
	QItemSelectionModel *selMusic;
	QStandardItemModel *modelEvents;
	QItemSelectionModel *selEvents;

	SongPlayer *spPlayer = NULL;

	QMenu *menuMusicBlankContext;
	QMenu *menuSongAndMixerContext;
	QMenu *menuClipContext;

	QUndoStack *odcUndo;
};

#endif // OPENADAPTIVE_H
