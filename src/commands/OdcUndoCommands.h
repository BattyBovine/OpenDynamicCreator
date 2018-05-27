#ifndef ODCUNDOCOMMANDS_H
#define ODCUNDOCOMMANDS_H

#include <QUndoCommand>

#include "Widgets/MusicTreeView.h"
#include "Widgets/EventTreeView.h"

class OdcAddItemCommand : public QUndoCommand
{
public:
    OdcAddItemCommand(QStandardItem*, QStandardItem*, QTreeView*, QStandardItemModel*, QItemSelectionModel*, QUndoCommand *parent=Q_NULLPTR);
    void undo();
    void redo();
private:
    QStandardItem *item, *parent;
    QTreeView *view;
    QStandardItemModel *model;
    QItemSelectionModel *selmodel;
    int row;
};



class OdcAddStateSwitchCommand : public QUndoCommand
{
public:
	OdcAddStateSwitchCommand(EventItem*, QTreeView*, QStandardItemModel*, QItemSelectionModel*, QUndoCommand *parent=Q_NULLPTR);
	void undo();
	void redo();
private:
	QStandardItem *stateswitch;
	QTreeView *view;
	QStandardItemModel *model;
	QItemSelectionModel *selmodel;
	int row;
};

#endif // ODCUNDOCOMMANDS_H
