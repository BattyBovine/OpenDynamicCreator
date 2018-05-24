#ifndef MUSICEVENT_H
#define MUSICEVENT_H

#include <QObject>
#include <QUuid>

#include "Beat.h"

class ClipContainer;


class EventCommand
{
public:
	EventCommand(Beat transitionlength) { this->beatTransitionLength=transitionlength; }
	virtual void applyEvent(ClipContainer*, float){}
protected:
	Beat beatTransitionLength;
};
class JumpToPositionCommand : public EventCommand
{
public:
	void setPosition(Beat pos) { this->beatToPosition=pos; }
	virtual void applyEvent(ClipContainer *cc, float delta=0.0f);
private:
	Beat beatToPosition;
};


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
	void addCommand(EventCommand e) { this->lCommands.append(e); }

	Beat beat() const { return this->beatPos; }
	QString name() const { return this->sName; }
	bool active() const { return this->bActive; }
	const QList<EventCommand> &commands() { return this->lCommands; }

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
	QList<EventCommand> lCommands;
};
typedef QVector<MusicEvent> MusicEventList;

#endif // MUSICEVENT_H
