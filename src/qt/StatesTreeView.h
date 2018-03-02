#ifndef STATESTREEVIEW_H
#define STATESTREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelection>

#include <QMimeData>
#include <QKeyEvent>
#include <QMessageBox>

enum StateItemType
{
	SIT_STATESWITCH = Qt::UserRole+102,
	SIT_STATE
};

class StateItem : public QObject, public QStandardItem
{
	Q_OBJECT
public:
	StateItem(QString t) { this->setText(t); }

	virtual int type() const { return StateItemType::SIT_STATE; }
	virtual QStandardItem *clone() const { return new StateItem(this->text()); }
};

class StateSwitchItem : public QObject, public QStandardItem
{
	Q_OBJECT
public:
	StateSwitchItem(QString t) { this->setText(t); }
	virtual int type() const { return StateItemType::SIT_STATESWITCH; }
	virtual QStandardItem *clone() const { return new StateSwitchItem(this->text()); }
};



class StatesTreeViewModel : public QStandardItemModel
{
	Q_OBJECT
public:
	QMimeData *mimeData(const QModelIndexList&) const;
	bool dropMimeData (const QMimeData*, Qt::DropAction, int, int, const QModelIndex&);
};



class StatesTreeView : public QTreeView
{
	Q_OBJECT
public:
	explicit StatesTreeView(QWidget *parent=Q_NULLPTR):QTreeView(parent){}

	virtual Qt::DropActions supportedDropActions() const { return Qt::CopyAction | Qt::MoveAction; }

public slots:
	void deleteSelectedItems();

protected:
	void keyPressEvent(QKeyEvent*);
};

#endif // STATESTREEVIEW_H
