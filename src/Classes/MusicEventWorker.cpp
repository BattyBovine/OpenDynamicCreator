#include "MusicEventWorker.h"

bool MusicEventWorker::getNextTarget()
{
	if(this->smeNextEvent==this->smelEvents.end())
		return false;
	this->iPosTarget = roundf(this->smeNextEvent->get()->beat().toSeconds(this->ccClip->tempo(), this->ccClip->beatUnit()) *
							 this->ccClip->sampleRate()) * (this->ccClip->bytesPerSample()*this->ccClip->channelCount());
	return true;

	for(StaticMusicEventList::Iterator i=this->smeNextEvent; i!=this->smelEvents.end(); i++) {
		float eventsecs = (i->get()->beat()/*-this->beatTimelineOffset*/).toSeconds(this->ccClip->tempo(), this->ccClip->beatUnit());
		if(eventsecs < this->ccClip->secondsElapsed())
			continue;
		this->smeNextEvent = i;
		break;
	}
	return false;
}
