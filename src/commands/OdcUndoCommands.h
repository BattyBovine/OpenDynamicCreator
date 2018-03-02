#ifndef ODCUNDOCOMMANDS_H
#define ODCUNDOCOMMANDS_H

#include <QUndoCommand>

#include "src/qt/MusicTreeView.h"
#include "src/qt/StatesTreeView.h"

class OdcAddTrackCommand : public QUndoCommand
{
public:
	OdcAddTrackCommand(MusicItem*, QTreeView*, QStandardItemModel*, QItemSelectionModel*, QUndoCommand *parent=Q_NULLPTR);
	void undo();
	void redo();
private:
	QStandardItem *track;
	QTreeView *view;
	QStandardItemModel *model;
	QItemSelectionModel *selmodel;
	int row;
};



class OdcAddStateSwitchCommand : public QUndoCommand
{
public:
	OdcAddStateSwitchCommand(StateSwitchItem*, QTreeView*, QStandardItemModel*, QItemSelectionModel*, QUndoCommand *parent=Q_NULLPTR);
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
