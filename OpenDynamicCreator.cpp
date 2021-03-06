#include "OpenDynamicCreator.h"
#include "ui_OpenDynamicCreator.h"

OpenDynamicCreator::OpenDynamicCreator(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::OpenDynamicCreator)
{
	ui->setupUi(this);

	QSettings settings;
	if(settings.contains(ODC_WINDOW_GEOMETRY))
		this->restoreGeometry(settings.value(ODC_WINDOW_GEOMETRY).toByteArray());
	if(settings.contains(ODC_WINDOW_STATE))
		this->restoreState(settings.value(ODC_WINDOW_STATE).toByteArray());

	// Set up music tree
	this->modelMusic = new MusicTreeViewModel();
	ui->treeMusic->setModel(this->modelMusic);
	this->selMusic = ui->treeMusic->selectionModel();
	// Set up event tree
	this->modelEvents = new EventTreeViewModel();
	ui->treeEvents->setModel(this->modelEvents);
	this->selEvents = ui->treeEvents->selectionModel();

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

	ui->statusMain->showMessage(tr("Ready"));
}

OpenDynamicCreator::~OpenDynamicCreator()
{
	delete ui;
}

void OpenDynamicCreator::closeEvent(QCloseEvent *e)
{
	QSettings settings;
	settings.setValue("WindowState", this->saveState());
	settings.setValue("WindowGeometry", this->saveGeometry());
	QMainWindow::closeEvent(e);
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
	if(!selection)
		return;
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
void OpenDynamicCreator::addClipFileDialog(QModelIndex parent)
{
	QList<QUrl> files = QFileDialog::getOpenFileUrls(this, tr("Open clip files"), QUrl(), "Ogg Vorbis (*.ogg);;WAV (*.wav)");
	foreach(QUrl url, files) {
		QFileInfo info(url.toLocalFile());
		if(info.exists())
			this->addClip(parent, url.toString(), info.baseName());
	}
}
void OpenDynamicCreator::addClipList(QModelIndex parent, QStringList files)
{
	foreach(QString file, files) {
		QUrl url(file);
		QFileInfo info(url.toLocalFile());
		if(info.exists())
			this->addClip(parent, url.toString(), info.baseName());
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



void OpenDynamicCreator::addEvent()
{
	QStandardItem *selection = this->checkSelectedEventTreeItem();
	if(!selection)	return;
	if(selection->type()!=EventItemType::EI_EVENT)
		Q_ASSERT(selection=selection->parent());
	int statecount=1, rowcount = selection->rowCount();
	for(uint16_t i=0; i<rowcount; i++) {
		QStandardItem *item = selection->child(i);
		if(item->type()==EventItemType::EI_COMMAND)
			statecount++;
	}
	selection->appendRow(new EventCommandItem(QString("%1 %2").arg(ODC_EVENT_COMMAND_LABEL).arg(statecount)));
}

void OpenDynamicCreator::addEventCommand()
{
	EventItem *ei = new EventItem(QString("%1 %2").arg(ODC_EVENT_LABEL).arg(this->modelEvents->rowCount()+1));
	this->modelEvents->invisibleRootItem()->appendRow(ei);
	this->selEvents->select(this->modelEvents->indexFromItem(ei), QItemSelectionModel::ClearAndSelect);
	ui->treeEvents->setExpanded(this->modelEvents->indexFromItem(ei), true);
}

void OpenDynamicCreator::deleteEvent()
{
	QStandardItem *selection = this->checkSelectedEventTreeItem();
	if(!selection)
		return;
	ui->treeEvents->deleteSelectedItems();
}

QStandardItem *OpenDynamicCreator::checkSelectedEventTreeItem()
{
	if(this->modelEvents->rowCount()==0) {
		QMessageBox::warning(this, ODC_TITLE_ADD_EVENT_NO_EVENT, ODC_TEXT_ADD_EVENT_NO_EVENT);
		return NULL;
	}
	if(!this->selEvents->hasSelection()) {
		QMessageBox::warning(this, ODC_TITLE_ADD_EVENT_NO_SELECTION, ODC_TEXT_ADD_EVENT_NO_SELECTION);
		return NULL;
	}
	QStandardItem *selection = this->modelEvents->itemFromIndex(this->selEvents->selectedRows()[0]);
	if(selection==NULL) {
		QMessageBox::warning(this, ODC_TITLE_ADD_EVENT_INVALID_SELECTION, ODC_TEXT_ADD_EVENT_INVALID_SELECTION);
		return NULL;
	}
	return selection;
}



void OpenDynamicCreator::swapEditorWidget(QItemSelection newitem)
{
	if(this->widgetActiveCentral) {
		this->widgetActiveCentral->deleteLater();
		this->widgetActiveCentral = NULL;
	}
	if(this->selMusic->selectedIndexes().size()!=1)
		return;
	QModelIndex index = this->selMusic->currentIndex();
	if(!index.isValid()) {
		foreach(QModelIndex i, newitem.indexes()) {
			if(i.isValid()) {
				index = i;
				break;
			}
		}
	}
	QStandardItem *item = this->modelMusic->itemFromIndex(index);
	if(item) {
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
	MusicPropertiesWidget *mpw = new MusicPropertiesWidget(item, this);
	ui->layoutCentralWidget->addWidget(mpw);
	this->widgetActiveCentral = mpw;
}

void OpenDynamicCreator::loadClipGroupEditorWidget(QModelIndex i)
{
	TrackItem *track = static_cast<TrackItem*>(this->modelMusic->itemFromIndex(this->findTrack(i)));
	if(!track)	return;
	BaseMusicItem *clip = (BaseMusicItem*)this->modelMusic->itemFromIndex(i);
	if(!clip)	return;
	ClipGroupEditorWidget *cgew = new ClipGroupEditorWidget(this);
	int rowcount = clip->rowCount();
	for(int row=0; row<rowcount; row++)
		cgew->addClipEditor(((ClipItem*)clip->child(row))->clipContainer());
	cgew->setClipGroupEditor(clip->clipContainer());
	ui->layoutCentralWidget->addWidget(cgew);
	this->widgetActiveCentral = cgew;
}

void OpenDynamicCreator::loadClipEditorWidget(QModelIndex i)
{
	TrackItem *track = static_cast<TrackItem*>(this->modelMusic->itemFromIndex(this->findTrack(i)));
	if(!track)	return;
	ClipItem *clip = static_cast<ClipItem*>(this->modelMusic->itemFromIndex(i));
	if(!clip)	return;
	ClipEditorWidget *cew = new ClipEditorWidget(this);
	cew->setClipEditor(clip->clipContainer());
	ui->layoutCentralWidget->addWidget(cew);
	this->widgetActiveCentral = cew;
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
		QUuid selectedclipid = QUuid();
		if(!this->spPlayer) {
			this->spPlayer = new SongPlayer(this);
			QModelIndex selectedindex = this->selMusic->currentIndex();
			if(!selectedindex.isValid())
				return;
			BaseMusicItem *item = static_cast<BaseMusicItem*>(this->modelMusic->itemFromIndex(selectedindex));
			if(item->type()==MIT_TRACK) {
				BaseMusicItem *firstchild = static_cast<BaseMusicItem*>(item->child(0));
				if(!firstchild)	return;
				selectedclipid = firstchild->clipContainer()->uuid();
			} else {
				selectedclipid = item->clipContainer()->uuid();
			}
			while(item->type()!=MIT_TRACK)
				item = (BaseMusicItem*)item->parent();
			if(this->spPlayer->buildSong(item) != SongPlayer::Error::SP_OK)
				this->stopSong();
		}
		connect(this->spPlayer, SIGNAL(finished()), this, SLOT(stopSong()));
		if(this->spPlayer->playSong(selectedclipid) != SongPlayer::Error::SP_OK)
			this->stopSong();
	} else {
		if(this->spPlayer)
			this->spPlayer->pauseSong();
	}
}

void OpenDynamicCreator::stopSong()
{
	ui->actionPlayPause->setChecked(false);
	if(this->spPlayer) {
		this->spPlayer->stopSong();
		this->spPlayer->deleteLater();
		this->spPlayer = NULL;
	}
}
