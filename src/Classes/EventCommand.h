#ifndef EVENTCOMMAND_H
#define EVENTCOMMAND_H

#include <memory>
#include <QAudio>
#include "Beat.h"

class ClipContainer;
class MusicEvent;
class StaticMusicEvent;
typedef std::shared_ptr<StaticMusicEvent> StaticMusicEventPtr;

class EventCommand
{
	friend class MusicEvent;
public:
	EventCommand(Beat transitionlength=Beat(), MusicEvent *parent=NULL) { this->beatTransitionLength=transitionlength; this->setParent(parent); }
	virtual void applyEvent(ClipContainer*){}
protected:
	void setParent(MusicEvent *p) { this->meParent=p; }
	MusicEvent *meParent=NULL;
	Beat beatTransitionLength;
};
class JumpToMarkerCommand : public EventCommand
{
public:
	JumpToMarkerCommand(StaticMusicEventPtr);
	void applyEvent(ClipContainer*) override;
private:
	StaticMusicEventPtr smeToPosition;
};
class ChangeVolumeCommand : public EventCommand
{
public:
	ChangeVolumeCommand(Beat transitionlength=Beat()) : EventCommand(transitionlength){}
	ChangeVolumeCommand(qreal v, Beat transitionlength=Beat()) : EventCommand(transitionlength) { this->setVolume(v); }
	ChangeVolumeCommand(int d, Beat transitionlength=Beat()) : EventCommand(transitionlength) { this->setVolume(QAudio::convertVolume(d, QAudio::DecibelVolumeScale, QAudio::LogarithmicVolumeScale)); }
	void setVolume(qreal v) { this->fVolume=v; }
	void applyEvent(ClipContainer*) override;
private:
	qreal fVolume = 0.0f;
};

#endif // EVENTCOMMAND_H
