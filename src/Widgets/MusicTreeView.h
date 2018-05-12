#ifndef MUSICTREEVIEW_H
#define MUSICTREEVIEW_H

#include <memory>

#include <QDebug>
#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelection>

#include <QMimeData>
#include <QKeyEvent>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QMessageBox>

#include "Classes/MusicEvent.h"
#include "Classes/ClipContainer.h"

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
	BaseMusicItem(QString t)
	{
		this->setText(t);
	}

	void cloneBase(BaseMusicItem *bmi) const
	{
		for(int i=0; i<this->rowCount(); i++)
			bmi->appendRow(this->child(i)->clone());
	}
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
};

class ClipItem : public BaseMusicItem
{
public:
	ClipItem(QString, QUrl);
	ClipItem(QString, QString f="");
	virtual int type() const { return MusicItemType::MIT_CLIP; }
	virtual QStandardItem *clone() const
	{
		ClipItem *ci = new ClipItem(this->text());
		this->cloneBase(ci);
		ci->ccClip = std::make_shared<ClipContainer>(*this->ccClip);
		return ci;
	}

	void loadClip(QUrl);
	void loadClip(QString);

	std::shared_ptr<ClipContainer> clipContainer();

private:
	std::shared_ptr<ClipContainer> ccClip=NULL;
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
	void playMusic();

protected:
	void keyPressEvent(QKeyEvent*);
};

#endif // MUSICTREEVIEW_H
