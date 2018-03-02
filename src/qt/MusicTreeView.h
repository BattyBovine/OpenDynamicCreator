#ifndef MUSICTREEVIEW_H
#define MUSICTREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelection>

#include <QMimeData>
#include <QKeyEvent>
#include <QMessageBox>

enum MusicItemType
{
	MIT_MUSIC = Qt::UserRole+2,
	MIT_CLIP,
	MIT_CLIPGROUP
};

class MusicItem : public QObject, public QStandardItem
{
	Q_OBJECT
public:
	MusicItem(QString t) { this->setText(t); this->setIcon(QIcon(":/icons/note")); }

	virtual int type() const { return MusicItemType::MIT_MUSIC; }
	virtual QStandardItem *clone() const { return new MusicItem(this->text()); }
};

class ClipGroupItem : public QObject, public QStandardItem
{
	Q_OBJECT
public:
	ClipGroupItem(QString t) { this->setText(t); this->setIcon(QIcon(":/icons/folder")); }

	virtual int type() const { return MusicItemType::MIT_CLIPGROUP; }
	virtual QStandardItem *clone() const { return new ClipGroupItem(this->text()); }
};

class ClipItem : public QObject, public QStandardItem
{
	Q_OBJECT
public:
	ClipItem(QString t) { this->setText(t); this->setIcon(QIcon(":/icons/waveform")); }
	ClipItem(QString f, QString t) { this->setClip(f); this->setText(t); this->setIcon(QIcon(":/icons/waveform")); }

	QString clip() { return this->sClipFile; }

	virtual int type() const { return MusicItemType::MIT_CLIP; }
	virtual QStandardItem *clone() const { return new ClipItem(this->sClipFile, this->text()); }

public slots:
	void setClip(QString s) { this->sClipFile=s; }

private:
	QString sClipFile;
};



class MusicTreeViewModel : public QStandardItemModel
{
	Q_OBJECT
public:
	QMimeData *mimeData(const QModelIndexList&) const;
	bool dropMimeData (const QMimeData*, Qt::DropAction, int, int, const QModelIndex&);
};



class MusicTreeView : public QTreeView
{
	Q_OBJECT
public:
	explicit MusicTreeView(QWidget *parent=Q_NULLPTR):QTreeView(parent){}

	virtual Qt::DropActions supportedDropActions() const { return Qt::CopyAction | Qt::MoveAction; }

public slots:
	void deleteSelectedItems();

protected:
	void keyPressEvent(QKeyEvent*);
};

#endif // MUSICTREEVIEW_H
