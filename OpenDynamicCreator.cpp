#include "OpenDynamicCreator.h"
#include "ui_OpenDynamicCreator.h"

OpenDynamicCreator::OpenDynamicCreator(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::OpenDynamicCreator)
{
	ui->setupUi(this);

	// Set up music tree
	this->modelMusic = new MusicTreeViewModel();
	ui->treeMusic->setModel(this->modelMusic);
	this->selMusic = ui->treeMusic->selectionModel();
	// Set up state tree
//	this->modelStates = new StatesTreeViewModel();
//	ui->treeStates->setModel(this->modelStates);
//	this->selStates = ui->treeStates->selectionModel();

	// Preferences window
	PreferencesDialog::initSettings();	// Fill settings with default entries, if necessary
	connect(ui->actionPreferences, SIGNAL(triggered(bool)), this, SLOT(openPreferencesDialog()));

	// Menu for right-clicking an empty area of the music tree or a music item
	this->menuMusicBlankContext = new QMenu(ui->treeMusic);
	this->menuMusicBlankContext->addAction(ui->actionAddTrack);
	this->menuMusicBlankContext->addAction(ui->actionDeleteMusicItem);

	// Menu for right-clicking a song or mixer item
	this->menuSongAndMixerContext = new QMenu(ui->treeMusic);
	this->menuSongAndMixerContext->addAction(ui->actionAddGroup);
	this->menuSongAndMixerContext->addAction(ui->actionAddClip);
	this->menuSongAndMixerContext->addAction(ui->actionDeleteMusicItem);

	// Menu for right-clicking a clip
	this->menuClipContext = new QMenu(ui->treeMusic);
	this->menuClipContext->addAction(ui->actionAddClip);
	this->menuClipContext->addAction(ui->actionDeleteMusicItem);

	this->odcUndo = new QUndoStack(this);
	connect(ui->actionUndo, SIGNAL(triggered()), this->odcUndo, SLOT(undo()));
	connect(ui->actionRedo, SIGNAL(triggered()), this->odcUndo, SLOT(redo()));

	connect(this->modelMusic, SIGNAL(audioClipsDropped(QModelIndex,QStringList)), this, SLOT(addClipList(QModelIndex,QStringList)));
	connect(this->selMusic, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(swapEditorWidget(QItemSelection)));

	connect(ui->actionPlayPause, SIGNAL(toggled(bool)), this, SLOT(playSong(bool)));
	connect(ui->actionStop, SIGNAL(triggered(bool)), this, SLOT(stopSong()));

	ui->statusMain->showMessage(tr("Ready"));
}

OpenDynamicCreator::~OpenDynamicCreator()
{
	delete ui;
}



void OpenDynamicCreator::openPreferencesDialog()
{
	this->dialogPreferences = new PreferencesDialog(this);
	connect(this->dialogPreferences,SIGNAL(finished(int)),this,SLOT(closePreferencesDialog(int)));
	this->dialogPreferences->open();

}
void OpenDynamicCreator::closePreferencesDialog(int)
{
	this->dialogPreferences->deleteLater();
}



void OpenDynamicCreator::addTrack()
{
	TrackItem *ti = new TrackItem(QString("%1 %2").arg(ODC_MUSIC_LABEL).arg(this->modelMusic->rowCount()+1));
	this->modelMusic->invisibleRootItem()->appendRow(ti);
	this->selMusic->select(this->modelMusic->indexFromItem(ti), QItemSelectionModel::ClearAndSelect);
	ui->treeMusic->setExpanded(this->modelMusic->indexFromItem(ti), true);
}

void OpenDynamicCreator::addClipGroup()
{
	QStandardItem *selection = this->checkSelectedMusicTreeItem();
	if(!selection)	return;
	if(selection->type()!=MusicItemType::MIT_TRACK)
		Q_ASSERT(selection=selection->parent());
	int groupcount=1, rowcount = selection->rowCount();
	for(uint16_t i=0; i<rowcount; i++) {
		QStandardItem *item = selection->child(i);
		if(item->type()==MusicItemType::MIT_CLIPGROUP)
			groupcount++;
	}
	ClipGroupItem *cgi = new ClipGroupItem(QString("%1 %2").arg(ODC_CLIP_GROUP_LABEL).arg(groupcount));
	selection->appendRow(cgi);
	ui->treeMusic->setExpanded(this->modelMusic->indexFromItem(cgi), true);
}

void OpenDynamicCreator::addClip(QModelIndex parent, QString file, QString label)
{
	QStandardItem *selection = NULL;
	if(!parent.isValid())
		selection = this->checkSelectedMusicTreeItem();
	else
		selection = this->modelMusic->itemFromIndex(parent);
	if(!selection) return;
    if(selection->type()!=MusicItemType::MIT_TRACK && selection->type()!=MusicItemType::MIT_CLIPGROUP)
		selection = selection->parent();
	if(!selection) return;
	int clipcount=1, rowcount = selection->rowCount();
	for(uint16_t i=0; i<rowcount; i++) {
		QStandardItem *item = selection->child(i);
		if(item->type()==MusicItemType::MIT_CLIP)
			clipcount++;
	}
	selection->appendRow(new ClipItem((label.isEmpty() ? QString("%1 %2").arg(ODC_CLIP_LABEL).arg(clipcount) : label), file));
}

void OpenDynamicCreator::addClipList(QModelIndex parent, QStringList files)
{
	foreach(QString file, files) {
		QFileInfo info(file);
		this->addClip(parent, file, info.baseName());
	}
}

void OpenDynamicCreator::deleteMusicItem()
{
	QStandardItem *selection = this->checkSelectedMusicTreeItem();
	if(!selection)	return;
	ui->treeMusic->deleteSelectedItems();
}

QStandardItem *OpenDynamicCreator::checkSelectedMusicTreeItem()
{
	if(this->modelMusic->rowCount()==0) {
		QMessageBox::warning(this, ODC_TITLE_ADD_CLIP_NO_MUSIC, ODC_TEXT_ADD_CLIP_NO_MUSIC);
		return NULL;
	}
	if(!this->selMusic->hasSelection()) {
		QMessageBox::warning(this, ODC_TITLE_ADD_CLIP_NO_SELECTION, ODC_TEXT_ADD_CLIP_NO_SELECTION);
		return NULL;
	}
	QStandardItem *selection = this->modelMusic->itemFromIndex(this->selMusic->selectedRows()[0]);
	if(selection==NULL) {
		QMessageBox::warning(this, ODC_TITLE_ADD_CLIP_INVALID_SELECTION, ODC_TEXT_ADD_CLIP_INVALID_SELECTION);
		return NULL;
	}
	return selection;
}



void OpenDynamicCreator::addStateSwitch()
{
//	StateSwitchItem *ss = new StateSwitchItem(QString("%1 %2").arg(ODC_STATE_SWITCH_LABEL).arg(this->modelStates->rowCount()+1));
//	this->modelStates->invisibleRootItem()->appendRow(ss);
//	this->selStates->select(this->modelStates->indexFromItem(ss), QItemSelectionModel::ClearAndSelect);
//	ui->treeStates->setExpanded(this->modelStates->indexFromItem(ss), true);
}

void OpenDynamicCreator::addState()
{
	QStandardItem *selection = this->checkSelectedStateTreeItem();
	if(!selection)	return;
	if(selection->type()!=StateItemType::SIT_STATESWITCH)
		Q_ASSERT(selection=selection->parent());
	int statecount=1, rowcount = selection->rowCount();
	for(uint16_t i=0; i<rowcount; i++) {
		QStandardItem *item = selection->child(i);
		if(item->type()==StateItemType::SIT_STATE)
			statecount++;
	}
	selection->appendRow(new StateItem(QString("%1 %2").arg(ODC_STATE_LABEL).arg(statecount)));
}

void OpenDynamicCreator::deleteStateItem()
{
//	QStandardItem *selection = this->checkSelectedStateTreeItem();
//	if(!selection)	return;
//	ui->treeStates->deleteSelectedItems();
}

QStandardItem *OpenDynamicCreator::checkSelectedStateTreeItem()
{
	if(this->modelStates->rowCount()==0) {
		QMessageBox::warning(this, ODC_TITLE_ADD_STATE_NO_SWITCH, ODC_TEXT_ADD_STATE_NO_SWITCH);
		return NULL;
	}
	if(!this->selStates->hasSelection()) {
		QMessageBox::warning(this, ODC_TITLE_ADD_STATE_NO_SELECTION, ODC_TEXT_ADD_STATE_NO_SELECTION);
		return NULL;
	}
	QStandardItem *selection = this->modelStates->itemFromIndex(this->selStates->selectedRows()[0]);
	if(selection==NULL) {
		QMessageBox::warning(this, ODC_TITLE_ADD_STATE_INVALID_SELECTION, ODC_TEXT_ADD_STATE_INVALID_SELECTION);
		return NULL;
	}
	return selection;
}



void OpenDynamicCreator::swapEditorWidget(QItemSelection i)
{
	ui->actionStop->toggle();
	if(this->selMusic->selectedIndexes().size()!=1) {
		this->setCentralWidget(new QWidget());
		return;
	}
	QModelIndexList selected = i.indexes();
	foreach(QModelIndex index, selected) {
		QStandardItem *item = this->modelMusic->itemFromIndex(index);
		if(!item)
			continue;
		switch(item->type()) {
		case MusicItemType::MIT_TRACK:
			this->loadTrackEditorWidget(index);
			break;
		case MusicItemType::MIT_CLIPGROUP:
			this->loadClipGroupEditorWidget(index);
			break;
		case MusicItemType::MIT_CLIP:
			this->loadClipEditorWidget(index);
			break;
		}
		return;
	}
}

void OpenDynamicCreator::customContextMenuMusic(QPoint point)
{
	QModelIndex index = ui->treeMusic->indexAt(point);
	if(index.isValid()) {
		switch(this->modelMusic->itemFromIndex(index)->type()) {
		case MusicItemType::MIT_TRACK:
		case MusicItemType::MIT_CLIPGROUP:
			this->menuSongAndMixerContext->exec(ui->treeMusic->mapToGlobal(point));
			break;
		case MusicItemType::MIT_CLIP:
			this->menuClipContext->exec(ui->treeMusic->mapToGlobal(point));
			break;
		default:
			this->menuMusicBlankContext->exec(ui->treeMusic->mapToGlobal(point));
		}
	} else {
		this->menuMusicBlankContext->exec(ui->treeMusic->mapToGlobal(point));
	}
}

void OpenDynamicCreator::showAboutDialogue()
{
	QString abouttext = QString("%1 v%2\n\n%3")
						.arg(qApp->applicationName())
						.arg(qApp->applicationVersion())
						.arg("©2018 BattyBovine. All rights reserved.");
	QMessageBox::question(this, qApp->applicationName(),
						  abouttext,
						  QMessageBox::Ok);
}



void OpenDynamicCreator::loadTrackEditorWidget(QModelIndex i)
{
	TrackItem *item = static_cast<TrackItem*>(this->modelMusic->itemFromIndex(i));
	if(!item)	return;
	MusicPropertiesWidget *mpw = new MusicPropertiesWidget(item);
	this->setCentralWidget(mpw);
}

void OpenDynamicCreator::loadClipGroupEditorWidget(QModelIndex i)
{
	TrackItem *track = static_cast<TrackItem*>(this->modelMusic->itemFromIndex(this->findTrack(i)));
	if(!track)	return;
	QStandardItem *clip = this->modelMusic->itemFromIndex(i);
	if(!clip)	return;
	ClipGroupEditorWidget *cgew = new ClipGroupEditorWidget();
	int rowcount = clip->rowCount();
	for(int row=0; row<rowcount; row++)
		cgew->addClipGroupEditor(static_cast<ClipItem*>(clip->child(row))->clipContainer());
	this->setCentralWidget(cgew);
}

void OpenDynamicCreator::loadClipEditorWidget(QModelIndex i)
{
	TrackItem *track = static_cast<TrackItem*>(this->modelMusic->itemFromIndex(this->findTrack(i)));
	if(!track)	return;
	ClipItem *clip = static_cast<ClipItem*>(this->modelMusic->itemFromIndex(i));
	if(!clip)	return;
	ClipEditorWidget *cew = new ClipEditorWidget();
	cew->setClipEditor(clip->clipContainer());
	this->setCentralWidget(cew);
}

QModelIndex OpenDynamicCreator::findTrack(QModelIndex index)
{
	while(index.parent().isValid())
		index = index.parent();
	return index;
}



void OpenDynamicCreator::playSong(bool play)
{
	if(play) {
		if(!this->spPlayer) {
			this->spPlayer = new SongPlayer(this);
			TrackItem *track = static_cast<TrackItem*>(this->modelMusic->itemFromIndex(this->selMusic->currentIndex()));
			this->spPlayer->buildSong(track);
		}
		this->spPlayer->playSong();
	} else {
		this->spPlayer->pauseSong();
	}
}

void OpenDynamicCreator::stopSong()
{
	if(this->spPlayer) {
		this->spPlayer->stopSong();
		this->spPlayer->deleteLater();
		this->spPlayer = NULL;
	}
}

void OpenDynamicCreator::resetPlayerState()
{
	ui->actionPlayPause->setChecked(false);
}
