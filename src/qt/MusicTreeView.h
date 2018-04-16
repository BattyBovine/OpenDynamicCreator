#ifndef MUSICTREEVIEW_H
#define MUSICTREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelection>

#include <QMimeData>
#include <QKeyEvent>
#include <QMessageBox>
#include <QUuid>

enum MusicItemType
{
    MIT_TRACK = Qt::UserRole+1000,
	MIT_CLIP,
	MIT_CLIPGROUP
};

class BaseMusicItem : public QStandardItem
{
public:
	BaseMusicItem(QString t) {
        this->setText(t);
		this->uuid = QUuid::createUuid();
    }
	void setUuid(QUuid u) { this->uuid=u; }
	QUuid getUuid() const { return this->uuid; }

private:
    QUuid uuid;
};

class TrackItem : public BaseMusicItem
{
public:
    TrackItem(QString t) : BaseMusicItem(t)
    {
        this->setIcon(QIcon(":/icons/note"));
    }

    virtual int type() const { return MusicItemType::MIT_TRACK; }
    virtual QStandardItem *clone() const
    {
		TrackItem *mi = new TrackItem(this->text());
        for(int i=0; i<this->rowCount(); i++)
            mi->appendRow(this->child(i)->clone());
		mi->setUuid(this->getUuid());
        return mi;
    }
};

class ClipGroupItem : public BaseMusicItem
{
public:
    ClipGroupItem(QString t) : BaseMusicItem(t)
    {
		this->setIcon(QIcon(":/icons/mixer"));
    }

	virtual int type() const { return MusicItemType::MIT_CLIPGROUP; }
    virtual QStandardItem *clone() const
    {
		ClipGroupItem *cgi = new ClipGroupItem(this->text());
        for(int i=0; i<this->rowCount(); i++)
			cgi->appendRow(this->child(i)->clone());
		cgi->setUuid(this->getUuid());
		return cgi;
    }
};

class ClipItem : public BaseMusicItem
{
public:
    ClipItem(QString t) : BaseMusicItem(t) { this->setText(t); this->setIcon(QIcon(":/icons/waveform")); }
    ClipItem(QString f, QString t) : BaseMusicItem(t) { this->setClip(f); this->setText(t); this->setIcon(QIcon(":/icons/waveform")); }

	QString clip() { return this->sClipFile; }

	virtual int type() const { return MusicItemType::MIT_CLIP; }
	virtual QStandardItem *clone() const
	{
		ClipItem *ci = new ClipItem(this->sClipFile, this->text());
		ci->setUuid(this->getUuid());
		return ci;
	}

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
