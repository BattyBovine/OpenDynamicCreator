#ifndef MUSICTREEVIEW_H
#define MUSICTREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelection>

#include <QKeyEvent>
#include <QMessageBox>

enum MusicItemType
{
	MIT_MUSIC	= Qt::UserRole+2,
	MIT_CLIP,
	MIT_CLIPGROUP
};

class MusicItem : public QObject, public QStandardItem
{
	Q_OBJECT
public:
	MusicItem(QString t) { this->setText(t); }
	MusicItem(QIcon i, QString t) { this->setIcon(i); this->setText(t); }
	MusicItem(QString f, QIcon i, QString t) { this->setClip(f); this->setIcon(i); this->setText(t); }

	virtual int type() const { return MusicItemType::MIT_MUSIC; }

public slots:
	void setClip(QString s) { this->sClipFile=s; }

private:
	QString sClipFile;
};

class ClipGroupItem : public QObject, public QStandardItem
{
	Q_OBJECT
public:
	ClipGroupItem(QString t) { this->setText(t); }
	ClipGroupItem(QIcon i, QString t) { this->setIcon(i); this->setText(t); }

	virtual int type() const { return MusicItemType::MIT_CLIPGROUP; }
};

class ClipItem : public QObject, public QStandardItem
{
	Q_OBJECT
public:
	ClipItem(QString t) { this->setText(t); }
	ClipItem(QIcon i, QString t) { this->setIcon(i); this->setText(t); }

	virtual int type() const { return MusicItemType::MIT_CLIP; }
};



class MusicTreeView : public QTreeView
{
	Q_OBJECT
public:
	explicit MusicTreeView(QWidget *parent=Q_NULLPTR):QTreeView(parent){}

public slots:
	void deleteSelectedItems();

protected:
	void keyPressEvent(QKeyEvent*);
};

#endif // MUSICTREEVIEW_H
