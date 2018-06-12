#include "EventCommand.h"
#include "StaticMusicEvent.h"
#include "ClipContainer.h"


QUuid JumpToMarkerCommand::applyEvent(ClipContainer *cc)
{
	cc->setPositionBeats(this->smeToPosition->beat() + cc->timelineOffset());
	return EventCommand::applyEvent(cc);
}


QUuid ChangeVolumeCommand::applyEvent(ClipContainer *cc)
{
	cc->setVolume(this->fVolume);
	return EventCommand::applyEvent(cc);
}
