#ifndef MUSICEVENT_H
#define MUSICEVENT_H

#include <QObject>
#include <QUuid>

#include "Beat.h"


class MusicEvent : public QObject
{
	Q_OBJECT
public:
	MusicEvent(Beat &b=Beat()) { this->setBeat(b); this->uuid=QUuid::createUuid(); }
	MusicEvent(const MusicEvent &e) { this->copy(e); }
	MusicEvent &operator=(const MusicEvent &e)
	{
		this->copy(e);
		return *this;
	}
	void copy(const MusicEvent &e)
	{
		this->uuid = e.uuid;
		this->beatPos = e.beatPos;
		this->bActive = e.bActive;
	}

	void setBeat(Beat &b) { this->beatPos=b; }
	void setName(QString &n) { this->sName=n; }
	void setActive(bool a) { this->bActive=a; }

	Beat beat() const { return this->beatPos; }
	QString name() const { return this->sName; }
	bool active() const { return this->bActive; }

	bool operator==(const MusicEvent &e) const	{ return (beatPos==e.beatPos); }
	bool operator!=(const MusicEvent &e) const	{ return !(*this==e); }
	bool operator<(const MusicEvent &e) const	{ return (beatPos<e.beatPos); }
	bool operator<=(const MusicEvent &e) const	{ return (beatPos<=e.beatPos); }
	bool operator>(const MusicEvent &e) const	{ return (beatPos>e.beatPos); }
	bool operator>=(const MusicEvent &e) const	{ return (beatPos>=e.beatPos); }

private:
	QUuid uuid;
	Beat beatPos;
	QString sName;
	bool bActive = true;
};
typedef QVector<MusicEvent> MusicEventList;


class EventCommand
{
public:
	void setClip(QUuid id) { this->uuidClip=id; }
	QUuid clip() { return this->uuidClip; }
private:
	QUuid uuidClip;
};
class JumpToCommand : public EventCommand
{
public:
	void setJumpToEvent(MusicEvent *e) { this->meEvent=e; }
	MusicEvent *jumpPosition() { return this->meEvent; }
private:
	MusicEvent *meEvent=NULL;
};

#endif // MUSICEVENT_H
