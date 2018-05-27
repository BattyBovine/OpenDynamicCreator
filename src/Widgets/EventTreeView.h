#ifndef EVENTTREEVIEW_H
#define EVENTTREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelection>

#include <QMimeData>
#include <QKeyEvent>
#include <QMessageBox>

enum EventItemType
{
	EI_EVENT = Qt::UserRole+2000,
	EI_COMMAND
};

class EventItem : public QStandardItem
{
public:
	EventItem(QString t) { this->setText(t); }

	virtual int type() const { return EventItemType::EI_EVENT; }
	virtual QStandardItem *clone() const { return new EventItem(this->text()); }
};

class EventCommandItem : public QStandardItem
{
public:
	EventCommandItem(QString t) { this->setText(t); }
	virtual int type() const { return EventItemType::EI_COMMAND; }
	virtual QStandardItem *clone() const { return new EventCommandItem(this->text()); }
};



class EventTreeViewModel : public QStandardItemModel
{
	Q_OBJECT
public:
	QMimeData *mimeData(const QModelIndexList&) const;
	bool dropMimeData (const QMimeData*, Qt::DropAction, int, int, const QModelIndex&);
};



class EventTreeView : public QTreeView
{
	Q_OBJECT
public:
	explicit EventTreeView(QWidget *parent=Q_NULLPTR):QTreeView(parent){}

	virtual Qt::DropActions supportedDropActions() const { return Qt::CopyAction | Qt::MoveAction; }

public slots:
	void deleteSelectedItems();

protected:
	void keyPressEvent(QKeyEvent*);
};

#endif // EVENTTREEVIEW_H
