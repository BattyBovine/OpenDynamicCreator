#ifndef MUSICTREEVIEW_H
#define MUSICTREEVIEW_H

#include <QDebug>
#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelection>

#include <QMimeData>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QUuid>

#define MIT_MIME "Qt/MusicItemType"

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
	TrackItem(QString);

    virtual int type() const { return MusicItemType::MIT_TRACK; }
    virtual QStandardItem *clone() const
    {
		TrackItem *mi = new TrackItem(this->text());
        for(int i=0; i<this->rowCount(); i++)
            mi->appendRow(this->child(i)->clone());
		mi->setUuid(this->getUuid());
        return mi;
    }

	void setTempo(int t) { this->iTempo = t; }
	int tempo() { return this->iTempo; }
	void setBeatsPerMeasure(int b) { this->iBeatsPerMeasure = b; }
	int beatsPerMeasure() { return this->iBeatsPerMeasure; }
	void setBeatUnit(int b) { this->iBeatUnit = b; }
	int beatUnit() { return this->iBeatUnit; }
	void setPlaybackSpeed(float s) { this->fPlaybackSpeed = s; }
	float playbackSpeed() { return this->fPlaybackSpeed; }

private:
	int iTempo = 120;
	int iBeatsPerMeasure = 4;
	int iBeatUnit = 4;
	float fPlaybackSpeed = 1.0f;
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
	ClipItem(QString, QString f="", float v=100.0f);

	virtual int type() const { return MusicItemType::MIT_CLIP; }
	virtual QStandardItem *clone() const
	{
		ClipItem *ci = new ClipItem(this->text(), this->sClipFile);
		ci->setUuid(this->getUuid());
		return ci;
	}

	void setClip(QString s) { this->sClipFile=s; }
	QString clip() { return this->sClipFile; }
	void setClipVolume(float v) { this->iVolume=v; }
	float clipVolume() { return this->iVolume; }

private:
	QString sClipFile;
	float iVolume;
};



class MusicTreeViewModel : public QStandardItemModel
{
	Q_OBJECT
public:
	QStringList mimeTypes() const;
	QMimeData *mimeData(const QModelIndexList&) const;
	bool dropMimeData (const QMimeData*, Qt::DropAction, int, int, const QModelIndex&);

signals:
	void audioClipsDropped(QModelIndex,QStringList);

private:
	QStandardItem *parseMusicItemMimeData(QStandardItem*);
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
