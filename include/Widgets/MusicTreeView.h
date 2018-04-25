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


class MusicEvent
{
public:
	MusicEvent(float t=0.0f) { this->setTime(t); }

	void setTime(float t) { this->fTime=t; }

	float time() const { return this->fTime; }

	void operator=(float t) { this->setTime(t); }

private:
	float fTime;
};


enum MusicItemType
{
    MIT_TRACK = Qt::UserRole+1000,
	MIT_CLIP,
	MIT_CLIPGROUP
};

typedef QVector<MusicEvent> MusicEventList;

class BaseMusicItem : public QStandardItem
{
public:
	BaseMusicItem(QString t) {
		this->uuid = QUuid::createUuid();
		this->setText(t);
		this->setVolume(1.0f);
		this->setPan(0);
	}

	void cloneBase(BaseMusicItem *bmi) const {
		for(int i=0; i<this->rowCount(); i++)
			bmi->appendRow(this->child(i)->clone());
		bmi->uuid = this->uuid;
		bmi->setEvents(this->events());
		bmi->setVolume(this->volume());
		bmi->setPan(this->pan());
	}

	virtual void setVolume(float v) { this->fVolume=v; }
	virtual void setPan(int p) { this->iPan=p; }
	virtual void addEvent(MusicEvent e) { this->vEvents << e; }
	virtual void insertEvent(int i, MusicEvent e) { this->vEvents.insert(i,e); }
	virtual void setEvents(MusicEventList el) { this->vEvents = el; }

	virtual float volume() const { return this->fVolume; }
	virtual int pan() const { return this->iPan; }
	virtual MusicEvent event(int i) const { return this->vEvents[i]; }
	virtual MusicEventList events() const { return this->vEvents; }

	virtual int measureLength() = NULL;

private:
	QUuid uuid;

protected:
	float fVolume;
	int iPan;
	MusicEventList vEvents;
};

class TrackItem : public BaseMusicItem
{
public:
	TrackItem(QString);
	virtual int type() const { return MusicItemType::MIT_TRACK; }
	virtual QStandardItem *clone() const
	{
		TrackItem *ti = new TrackItem(this->text());
		this->cloneBase(ti);
		return ti;
	}

	void setTempo(int t) { this->iTempo = t; }
	int tempo() { return this->iTempo; }
	void setBeatsPerMeasure(int b) { this->iBeatsPerMeasure = b; }
	int beatsPerMeasure() { return this->iBeatsPerMeasure; }
	void setBeatUnit(int b) { this->iBeatUnit = b; }
	int beatUnit() { return this->iBeatUnit; }
	void setPlaybackSpeed(float s) { this->fPlaybackSpeed = s; }
	float playbackSpeed() { return this->fPlaybackSpeed; }

	virtual int measureLength() { return 0; }

private:
	int iTempo = 120;
	int iBeatsPerMeasure = 4;
	int iBeatUnit = 4;
	float fPlaybackSpeed = 1.0f;
};

class ClipGroupItem : public BaseMusicItem
{
public:
	ClipGroupItem(QString t);
	virtual int type() const { return MusicItemType::MIT_CLIPGROUP; }
	virtual QStandardItem *clone() const
	{
		ClipGroupItem *cgi = new ClipGroupItem(this->text());
		this->cloneBase(cgi);
		return cgi;
	}

	virtual int measureLength() { return 0; }
};

class ClipItem : public BaseMusicItem
{
public:
	ClipItem(QString, QString f="");
	virtual int type() const { return MusicItemType::MIT_CLIP; }
	virtual QStandardItem *clone() const
	{
		ClipItem *ci = new ClipItem(this->text(), this->sClipFile);
		this->cloneBase(ci);
		return ci;
	}

	void setClip(QString s) { this->sClipFile=s; }
	QString clip() { return this->sClipFile; }

	virtual int measureLength() { return 0; }

private:
	QString sClipFile;
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
