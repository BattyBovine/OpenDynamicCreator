#ifndef EVENTCOMMAND_H
#define EVENTCOMMAND_H

#include <memory>
#include <QAudio>
#include <QUuid>

#include "Beat.h"


class ClipContainer;
class MusicEvent;
class StaticMusicEvent;
typedef std::shared_ptr<StaticMusicEvent> StaticMusicEventPtr;


class EventCommand
{
	friend class MusicEvent;
public:
	EventCommand(QUuid clip, Beat transitionlength=Beat(), MusicEvent *parent=NULL) {
		this->uuidClip = clip;
		this->beatTransitionLength=transitionlength;
		this->setParent(parent);
	}
	virtual void applyEvent(ClipContainer*){}

	QUuid clip() { return this->uuidClip; }

protected:
	void setParent(MusicEvent *p) { this->meParent=p; }
	QUuid uuidClip;
	MusicEvent *meParent=NULL;
	Beat beatTransitionLength;
};

class JumpToMarkerCommand : public EventCommand
{
public:
	JumpToMarkerCommand(QUuid clip, Beat transitionlength=Beat()) : EventCommand(clip,transitionlength){}
	JumpToMarkerCommand(StaticMusicEventPtr sme, QUuid clip, Beat transitionlength=Beat()) : EventCommand(clip,transitionlength) { this->smeToPosition=sme; }
	void applyEvent(ClipContainer*) override;

private:
	StaticMusicEventPtr smeToPosition;
};

class ChangeVolumeCommand : public EventCommand
{
public:
	ChangeVolumeCommand(QUuid clip, Beat transitionlength=Beat()) : EventCommand(clip,transitionlength){}
	ChangeVolumeCommand(qreal v, QUuid clip, Beat transitionlength=Beat()) : EventCommand(clip,transitionlength) { this->setVolume(v); }
	ChangeVolumeCommand(int d, QUuid clip, Beat transitionlength=Beat()) : EventCommand(clip,transitionlength) { this->setVolume(QAudio::convertVolume(d, QAudio::DecibelVolumeScale, QAudio::LogarithmicVolumeScale)); }
	void setVolume(qreal v) { this->fVolume=v; }
	void applyEvent(ClipContainer*) override;

private:
	qreal fVolume = 0.0f;
};

#endif // EVENTCOMMAND_H
