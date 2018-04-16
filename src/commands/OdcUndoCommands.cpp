#include "OdcUndoCommands.h"

OdcAddItemCommand::OdcAddItemCommand(QStandardItem *i, QStandardItem *p, QTreeView *v, QStandardItemModel *m, QItemSelectionModel *s, QUndoCommand *parent) : QUndoCommand(parent)
{
    this->item = i;
    this->parent = p;
    this->view = v;
    this->model = m;
    this->selmodel = s;
}
void OdcAddItemCommand::undo()
{
}
void OdcAddItemCommand::redo()
{
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
}
void OdcAddStateSwitchCommand::redo()
{
}
