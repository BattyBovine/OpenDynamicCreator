#include "OdcUndoCommands.h"

OdcAddTrackCommand::OdcAddTrackCommand(MusicItem *t, QTreeView *v, QStandardItemModel *m, QItemSelectionModel *s, QUndoCommand *parent) : QUndoCommand(parent)
{
	this->track = t;
	this->view = v;
	this->model = m;
	this->selmodel = s;
}
void OdcAddTrackCommand::undo()
{
	this->track = this->track->clone();
	this->model->invisibleRootItem()->removeRow(this->row);
}
void OdcAddTrackCommand::redo()
{
	this->row = this->model->rowCount();
	this->model->invisibleRootItem()->appendRow(this->track);
	this->selmodel->select(this->model->indexFromItem(this->track), QItemSelectionModel::ClearAndSelect);
	this->view->setExpanded(this->model->indexFromItem(this->track), true);
}



OdcAddStateSwitchCommand::OdcAddStateSwitchCommand(StateSwitchItem *t, QTreeView *v, QStandardItemModel *m, QItemSelectionModel *s, QUndoCommand *parent) : QUndoCommand(parent)
{
	this->stateswitch = t;
	this->view = v;
	this->model = m;
	this->selmodel = s;
}
void OdcAddStateSwitchCommand::undo()
{
	this->stateswitch = this->stateswitch->clone();
	this->model->invisibleRootItem()->removeRow(this->row);
}
void OdcAddStateSwitchCommand::redo()
{
	this->row = this->model->rowCount();
	this->model->invisibleRootItem()->appendRow(this->stateswitch);
	this->selmodel->select(this->model->indexFromItem(this->stateswitch), QItemSelectionModel::ClearAndSelect);
	this->view->setExpanded(this->model->indexFromItem(this->stateswitch), true);
}
