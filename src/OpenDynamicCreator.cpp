#include "src/OpenDynamicCreator.h"
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
	this->modelStates = new StatesTreeViewModel();
	ui->treeStates->setModel(this->modelStates);
	this->selStates = ui->treeStates->selectionModel();

	// Menu for right-clicking an empty area of the music tree
	this->menuTrackContext = new QMenu(ui->treeMusic);
	this->menuTrackContext->addAction(ui->actionAddTrack);
	this->menuTrackContext->addAction(ui->actionDeleteMusicItem);

	// Menu for right-clicking an item on the music tree
	this->menuClipContext = new QMenu(ui->treeMusic);
	this->menuClipContext->addAction(ui->actionAddGroup);
	this->menuClipContext->addAction(ui->actionAddClip);
	this->menuClipContext->addAction(ui->actionDeleteMusicItem);

	this->setupPlaybackToolbar();

	this->odcUndo = new QUndoStack(this);
	connect(ui->actionUndo, SIGNAL(triggered()), this->odcUndo, SLOT(undo()));
	connect(ui->actionRedo, SIGNAL(triggered()), this->odcUndo, SLOT(redo()));

	ui->statusMain->showMessage(tr("Ready"));
}

OpenDynamicCreator::~OpenDynamicCreator()
{
	delete ui;
}



void OpenDynamicCreator::addTrack()
{
	this->odcUndo->push(new OdcAddTrackCommand(new MusicItem(QString("Track %1").arg(this->modelMusic->rowCount()+1)), ui->treeMusic, this->modelMusic, this->selMusic));
}

void OpenDynamicCreator::addClipGroup()
{
	QStandardItem *selection = this->checkSelectedMusicTreeItem();
	if(!selection)	return;
	if(selection->type()!=MusicItemType::MIT_MUSIC && selection->type()!=MusicItemType::MIT_CLIPGROUP)
		Q_ASSERT(selection=selection->parent());
	int groupcount=1, rowcount = selection->rowCount();
	for(uint16_t i=0; i<rowcount; i++) {
		QStandardItem *item = selection->child(i);
		if(item->type()==MusicItemType::MIT_CLIPGROUP)
			groupcount++;
	}
	selection->appendRow(new ClipGroupItem(QString("Group %1").arg(groupcount)));
}

void OpenDynamicCreator::addClip()
{
	QStandardItem *selection = this->checkSelectedMusicTreeItem();
	if(!selection)	return;
	if(selection->type()!=MusicItemType::MIT_MUSIC && selection->type()!=MusicItemType::MIT_CLIPGROUP)
		Q_ASSERT(selection=selection->parent());
	int clipcount=1, rowcount = selection->rowCount();
	for(uint16_t i=0; i<rowcount; i++) {
		QStandardItem *item = selection->child(i);
		if(item->type()==MusicItemType::MIT_CLIP)
			clipcount++;
	}
	selection->appendRow(new ClipItem(QString("Clip %1").arg(clipcount)));
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
		QMessageBox::warning(this, OAML_TITLE_ADD_CLIP_NO_MUSIC, OAML_TEXT_ADD_CLIP_NO_MUSIC);
		return NULL;
	}
	if(!this->selMusic->hasSelection()) {
		QMessageBox::warning(this, OAML_TITLE_ADD_CLIP_NO_SELECTION, OAML_TEXT_ADD_CLIP_NO_SELECTION);
		return NULL;
	}
	QStandardItem *selection = this->modelMusic->itemFromIndex(this->selMusic->selectedRows()[0]);
	if(selection==NULL) {
		QMessageBox::warning(this, OAML_TITLE_ADD_CLIP_INVALID_SELECTION, OAML_TEXT_ADD_CLIP_INVALID_SELECTION);
		return NULL;
	}
	return selection;
}



void OpenDynamicCreator::addStateSwitch()
{
	this->odcUndo->push(new OdcAddStateSwitchCommand(new StateSwitchItem(QString("Switch %1").arg(this->modelStates->rowCount()+1)), ui->treeStates, this->modelStates, this->selStates));
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
	selection->appendRow(new StateItem(QString("State %1").arg(statecount)));
}

void OpenDynamicCreator::deleteStateItem()
{
	QStandardItem *selection = this->checkSelectedStateTreeItem();
	if(!selection)	return;
	ui->treeStates->deleteSelectedItems();
}

QStandardItem *OpenDynamicCreator::checkSelectedStateTreeItem()
{
	if(this->modelStates->rowCount()==0) {
		QMessageBox::warning(this, OAML_TITLE_ADD_STATE_NO_SWITCH, OAML_TEXT_ADD_STATE_NO_SWITCH);
		return NULL;
	}
	if(!this->selStates->hasSelection()) {
		QMessageBox::warning(this, OAML_TITLE_ADD_STATE_NO_SELECTION, OAML_TEXT_ADD_STATE_NO_SELECTION);
		return NULL;
	}
	QStandardItem *selection = this->modelStates->itemFromIndex(this->selStates->selectedRows()[0]);
	if(selection==NULL) {
		QMessageBox::warning(this, OAML_TITLE_ADD_STATE_INVALID_SELECTION, OAML_TEXT_ADD_STATE_INVALID_SELECTION);
		return NULL;
	}
	return selection;
}



void OpenDynamicCreator::customContextMenuMusic(QPoint point)
{
	QModelIndex index = ui->treeMusic->indexAt(point);
	if(index.isValid()) {
		this->menuClipContext->exec(ui->treeMusic->mapToGlobal(point));
	} else {
		this->menuTrackContext->exec(ui->treeMusic->mapToGlobal(point));
	}
}

void OpenDynamicCreator::setupPlaybackToolbar()
{
	QComboBox *combo = new QComboBox();
	combo->addItem(tr("No IDs"));
	ui->toolbarPlayback->addWidget(combo);

	combo = new QComboBox();
	combo->addItem(tr("No values"));
	ui->toolbarPlayback->addWidget(combo);
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
