#ifndef STATICMUSICEVENT_H
#define STATICMUSICEVENT_H

#include <memory>

#include <QVector>

#include "MusicEvent.h"

class StaticMusicEvent
{
public:
	StaticMusicEvent(MusicEventPtr me, Beat pos=Beat()) { this->setEvent(me); this->setBeat(pos); }
	void setEvent(MusicEventPtr me) { this->meEvent=me; }
	void setBeat(Beat pos) { this->beatPos=pos; }

	MusicEventPtr musicEvent() { return this->meEvent; }
	Beat beat() { return this->beatPos; }

	bool operator==(const StaticMusicEvent &e) const	{ return (beatPos==e.beatPos); }
	bool operator!=(const StaticMusicEvent &e) const	{ return !(*this==e); }
	bool operator<(const StaticMusicEvent &e) const		{ return (beatPos<e.beatPos); }
	bool operator<=(const StaticMusicEvent &e) const	{ return (beatPos<=e.beatPos); }
	bool operator>(const StaticMusicEvent &e) const		{ return (beatPos>e.beatPos); }
	bool operator>=(const StaticMusicEvent &e) const	{ return (beatPos>=e.beatPos); }
private:
	MusicEventPtr meEvent;
	Beat beatPos;
};
typedef std::shared_ptr<StaticMusicEvent> StaticMusicEventPtr;
typedef QVector<StaticMusicEventPtr> StaticMusicEventList;

#endif // STATICMUSICEVENT_H
