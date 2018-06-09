#include "EventCommand.h"
#include "StaticMusicEvent.h"
#include "ClipContainer.h"


void JumpToMarkerCommand::applyEvent(ClipContainer *cc)
{
	cc->setPositionBeats(this->smeToPosition->beat() + cc->timelineOffset());
}


void ChangeVolumeCommand::applyEvent(ClipContainer *cc)
{
	cc->setVolume(this->fVolume);
}
