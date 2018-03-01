#include "src/OpenDynamicCreator.h"
#include "ui_OpenDynamicCreator.h"

OpenDynamicCreator::OpenDynamicCreator(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::OpenDynamicCreator)
{
	ui->setupUi(this);
	this->modelMusic = new QStandardItemModel();
	ui->treeMusic->setModel(this->modelMusic);
	this->setupPlaybackToolbar();

	this->menuTrackContext = new QMenu(ui->treeMusic);
	this->menuTrackContext->addAction(ui->actionAddTrack);
	this->menuTrackContext->addAction(ui->actionDeleteMusicItem);

	this->menuClipContext = new QMenu(ui->treeMusic);
	this->menuClipContext->addAction(ui->actionAddGroup);
	this->menuClipContext->addAction(ui->actionAddClip);
	this->menuClipContext->addAction(ui->actionDeleteMusicItem);

	this->selMusic = ui->treeMusic->selectionModel();

	ui->statusMain->showMessage(tr("Ready"));
}

OpenDynamicCreator::~OpenDynamicCreator()
{
	delete ui;
}



void OpenDynamicCreator::addTrack()
{
	this->modelMusic->invisibleRootItem()->appendRow(new MusicItem(QIcon(":/icons/note"), QString("Track %1").arg(this->modelMusic->rowCount()+1)));
}

void OpenDynamicCreator::addClipGroup()
{
	QStandardItem *selection = this->checkSelectedMusicTreeItem();
	if(!selection)	return;
	if(selection->type()!=MusicItemType::MIT_MUSIC && selection->type()!=MusicItemType::MIT_CLIPGROUP) {
		QMessageBox::warning(this, OAML_TITLE_ADD_CLIP_INVALID_SELECTION, OAML_TEXT_ADD_CLIP_INVALID_SELECTION);
		return;
	}
	int groupcount=1, rowcount = selection->rowCount();
	for(uint16_t i=0; i<rowcount; i++) {
		QStandardItem *item = selection->child(i);
		if(item->type()==MusicItemType::MIT_CLIPGROUP)
			groupcount++;
	}
	selection->appendRow(new ClipGroupItem(QIcon(":/icons/folder"), QString("Group %1").arg(groupcount)));
}

void OpenDynamicCreator::addClip()
{
	QStandardItem *selection = this->checkSelectedMusicTreeItem();
	if(!selection)	return;
	if(selection->type()!=MusicItemType::MIT_MUSIC && selection->type()!=MusicItemType::MIT_CLIPGROUP) {
		Q_ASSERT(selection=selection->parent());
	}
	int clipcount=1, rowcount = selection->rowCount();
	for(uint16_t i=0; i<rowcount; i++) {
		QStandardItem *item = selection->child(i);
		if(item->type()==MusicItemType::MIT_CLIP)
			clipcount++;
	}
	selection->appendRow(new ClipItem(QIcon(":/icons/waveform"), QString("Clip %1").arg(clipcount)));
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
