#ifndef OPENADAPTIVE_H
#define OPENADAPTIVE_H

#include <QMainWindow>
#include <QComboBox>
#include <QMessageBox>

#include "src/qt/MusicTreeView.h"

#define OAML_TITLE_ADD_CLIP_NO_MUSIC			tr("No tracks")
#define OAML_TEXT_ADD_CLIP_NO_MUSIC				tr("Please create a music track first.")
#define OAML_TITLE_ADD_CLIP_NO_SELECTION		tr("Nothing selected")
#define OAML_TEXT_ADD_CLIP_NO_SELECTION			tr("Please select a music track or clip group first.")
#define OAML_TITLE_ADD_CLIP_INVALID_SELECTION	tr("Invalid selection")
#define OAML_TEXT_ADD_CLIP_INVALID_SELECTION	tr("You have somehow selected something that has no valid data.")

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

	void customContextMenuMusic(QPoint);
	void setupPlaybackToolbar();
	void showAboutDialogue();

private:
	Ui::OpenDynamicCreator *ui;
	QStandardItemModel *modelMusic;
	QItemSelectionModel *selMusic;
	QMenu *menuTrackContext;
	QMenu *menuClipContext;
	QList<ClipItem*> lClips;
};

#endif // OPENADAPTIVE_H
