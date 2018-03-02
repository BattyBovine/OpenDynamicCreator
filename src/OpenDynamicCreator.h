#ifndef OPENADAPTIVE_H
#define OPENADAPTIVE_H

#include <QMainWindow>
#include <QComboBox>
#include <QUndoStack>
#include <QMessageBox>

#include "src/qt/MusicTreeView.h"
#include "src/qt/StatesTreeView.h"
#include "src/commands/OdcUndoCommands.h"

#define OAML_TITLE_ADD_CLIP_NO_MUSIC			tr("No tracks")
#define OAML_TEXT_ADD_CLIP_NO_MUSIC				tr("Please create a music track first.")
#define OAML_TITLE_ADD_CLIP_NO_SELECTION		tr("Nothing selected")
#define OAML_TEXT_ADD_CLIP_NO_SELECTION			tr("Please select a music track or clip group first.")
#define OAML_TITLE_ADD_CLIP_INVALID_SELECTION	tr("Invalid selection")
#define OAML_TEXT_ADD_CLIP_INVALID_SELECTION	tr("You have somehow selected something that has no valid data.")

#define OAML_TITLE_ADD_STATE_NO_SWITCH			tr("No switches")
#define OAML_TEXT_ADD_STATE_NO_SWITCH			tr("Please create a state switch first.")
#define OAML_TITLE_ADD_STATE_NO_SELECTION		tr("Nothing selected")
#define OAML_TEXT_ADD_STATE_NO_SELECTION		tr("Please select a switch first.")
#define OAML_TITLE_ADD_STATE_INVALID_SELECTION	tr("Invalid selection")
#define OAML_TEXT_ADD_STATE_INVALID_SELECTION	tr("You have somehow selected something that has no valid data.")

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
	void addClip();
	void deleteMusicItem();
	QStandardItem *checkSelectedMusicTreeItem();

	void addStateSwitch();
	void addState();
	void deleteStateItem();
	QStandardItem *checkSelectedStateTreeItem();

	void customContextMenuMusic(QPoint);
	void setupPlaybackToolbar();
	void showAboutDialogue();

private:
	Ui::OpenDynamicCreator *ui;
	QStandardItemModel *modelMusic;
	QItemSelectionModel *selMusic;
	QStandardItemModel *modelStates;
	QItemSelectionModel *selStates;
	QMenu *menuTrackContext;
	QMenu *menuClipContext;
	QUndoStack *odcUndo;
};

#endif // OPENADAPTIVE_H
