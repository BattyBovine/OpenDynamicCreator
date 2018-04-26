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


class Beat
{
public:
	Beat(int beat=0, float tempo=120.0f, quint8 beatunit=4)
	{
		this->iBeat=beat;
		this->fTempo=tempo;
		this->iBeatUnit=beatunit;
	}

	void setBeat(int b) { this->iBeat=b; }
	void setTempo(float t) { this->fTempo=t; }
	void setBeatUnit(quint8 u) { this->iBeatUnit=u; }

	int beat() { return this->iBeat; }
	float tempo() { return this->fTempo; }
	quint8 beatUnit() { return this->iBeatUnit; }
	static Beat fromSeconds(float secs, float tempo, quint8 beatunit) { return Beat(roundf((secs*(60.0f*beatunit))/tempo), tempo, beatunit); }
	float toSeconds() const { return (this->fTempo/60.0f) * (this->iBeat/float(this->iBeatUnit)); }

	bool operator==(Beat &b) { return (this->beat()==b.beat() && this->unitMatch(b)); }
	Beat operator+(Beat &b)  { if(!this->unitMatch(b)) return Beat(); return Beat(this->beat()+b.beat(), this->tempo(), this->beatUnit()); }
	void operator+=(Beat &b) { if(this->unitMatch(b)) this->setBeat(this->beat()+b.beat()); }
	Beat operator-(Beat &b)  { if(!this->unitMatch(b)) return Beat(); return Beat(this->beat()-b.beat(), this->tempo(), this->beatUnit()); }
	void operator-=(Beat &b) { if(this->unitMatch(b)) this->setBeat(this->beat()+b.beat()); }

private:
	bool unitMatch(Beat &b) { return (this->tempo()==b.tempo() && this->beatUnit()==b.beatUnit()); }
	float fTempo;
	int iBeat;
	quint8 iBeatUnit;
};

class MusicEvent
{
public:
	MusicEvent(Beat b=Beat()) { this->setBeat(b); }

	void setBeat(Beat b) { this->oBeat=b; }
	Beat beat() const { return this->oBeat; }

private:
	Beat oBeat;
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
	BaseMusicItem(QString t, float volume=1.0f, int pan=0)
	{
		this->setUuid(QUuid::createUuid());
		this->setText(t);
		this->setVolume(volume);
		this->setPan(pan);
	}

	void cloneBase(BaseMusicItem *bmi) const
	{
		for(int i=0; i<this->rowCount(); i++)
			bmi->appendRow(this->child(i)->clone());
		bmi->setUuid(this->uuid());
		bmi->setVolume(this->volume());
		bmi->setPan(this->pan());
		bmi->setEvents(this->events());
	}

	virtual void setVolume(float v) { this->fVolume=v; }
	virtual void setPan(int p) { this->iPan=p; }
	virtual void addEvent(MusicEvent e) { this->vEvents << e; }
	virtual void insertEvent(int i, MusicEvent e) { this->vEvents.insert(i,e); }

	virtual QUuid uuid() const { return this->qUuid; }
	virtual QString uuidString() const { return this->qUuid.toString(); }
	virtual float volume() const { return this->fVolume; }
	virtual int pan() const { return this->iPan; }
	virtual MusicEvent event(int i) const { return this->vEvents[i]; }
	virtual MusicEventList events() const { return this->vEvents; }

	virtual int measureLength() = NULL;

protected:
	float fVolume;
	int iPan;
	MusicEventList vEvents;

private:
	virtual void setUuid(QUuid u) { this->qUuid=u; }
	virtual void setEvents(MusicEventList el) { this->vEvents = el; }
	QUuid qUuid;
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
		ti->setTempo(this->tempo());
		ti->setBeatsPerMeasure(this->beatsPerMeasure());
		ti->setBeatUnit(this->beatUnit());
		ti->setPlaybackSpeed(this->playbackSpeed());
		return ti;
	}

	virtual void setTempo(float t) { this->fTempo=t; }
	virtual void setBeatsPerMeasure(int b) { this->iBeatsPerMeasure = b; }
	virtual void setBeatUnit(int b) { this->iBeatUnit = b; }
	virtual void setPlaybackSpeed(float s) { this->fPlaybackSpeed = s; }

	virtual float tempo() const { return this->fTempo; }
	virtual int beatsPerMeasure() const { return this->iBeatsPerMeasure; }
	virtual int beatUnit() const { return this->iBeatUnit; }
	virtual float playbackSpeed() const { return this->fPlaybackSpeed; }

	virtual int measureLength() { return 0; }

private:
	float fTempo;
	int iBeatsPerMeasure;
	int iBeatUnit;
	float fPlaybackSpeed;
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
