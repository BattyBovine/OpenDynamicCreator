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

#include "MusicPropertiesWidget.h"
#include "ClipGroupEditorWidget.h"
#include "ClipEditorWidget.h"
#include "Widgets/ClipMixerWidget.h"

#include "Widgets/MusicTreeView.h"
#include "Widgets/StatesTreeView.h"

#define ODC_MUSIC_LABEL			tr("Song")
#define ODC_CLIP_GROUP_LABEL	tr("Mixer")
#define ODC_CLIP_LABEL			tr("Clip")

#define ODC_STATE_SWITCH_LABEL	tr("Switch")
#define ODC_STATE_LABEL			tr("State")

#define ODC_TITLE_ADD_CLIP_NO_MUSIC				tr("No tracks")
#define ODC_TEXT_ADD_CLIP_NO_MUSIC				tr("Please create a music track first.")
#define ODC_TITLE_ADD_CLIP_NO_SELECTION			tr("Nothing selected")
#define ODC_TEXT_ADD_CLIP_NO_SELECTION			tr("Please select a music track or clip group first.")
#define ODC_TITLE_ADD_CLIP_INVALID_SELECTION	tr("Invalid selection")
#define ODC_TEXT_ADD_CLIP_INVALID_SELECTION		tr("You have somehow selected something that has no valid data.")

#define ODC_TITLE_ADD_STATE_NO_SWITCH			tr("No switches")
#define ODC_TEXT_ADD_STATE_NO_SWITCH			tr("Please create a state switch first.")
#define ODC_TITLE_ADD_STATE_NO_SELECTION		tr("Nothing selected")
#define ODC_TEXT_ADD_STATE_NO_SELECTION			tr("Please select a switch first.")
#define ODC_TITLE_ADD_STATE_INVALID_SELECTION	tr("Invalid selection")
#define ODC_TEXT_ADD_STATE_INVALID_SELECTION	tr("You have somehow selected something that has no valid data.")

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
	void addTrack();
	void addClipGroup();
	void addClip(QModelIndex parent=QModelIndex(), QString file="", QString label="");
	void addClipList(QModelIndex,QStringList);
	void deleteMusicItem();
	QStandardItem *checkSelectedMusicTreeItem();

	void addStateSwitch();
	void addState();
	void deleteStateItem();
	QStandardItem *checkSelectedStateTreeItem();

	void swapEditorWidget(QModelIndex);
	void customContextMenuMusic(QPoint);
	void showAboutDialogue();

private:
	Ui::OpenDynamicCreator *ui;

	void loadTrackEditorWidget(QModelIndex);
	void loadClipGroupEditorWidget(QModelIndex);
	void loadClipEditorWidget(QModelIndex);
	QModelIndex findTrack(QModelIndex);

	MusicTreeViewModel *modelMusic;
	QItemSelectionModel *selMusic;
	QStandardItemModel *modelStates;
	QItemSelectionModel *selStates;

	QMenu *menuMusicBlankContext;
	QMenu *menuSongAndMixerContext;
	QMenu *menuClipContext;

	QUndoStack *odcUndo;
};

#endif // OPENADAPTIVE_H
