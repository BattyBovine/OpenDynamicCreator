#include "EventCommand.h"
#include "StaticMusicEvent.h"
#include "ClipContainer.h"

JumpToMarkerCommand::JumpToMarkerCommand(StaticMusicEventPtr sme)
{
	this->smeToPosition=sme;
}
void JumpToMarkerCommand::applyEvent(ClipContainer *cc)
{
	cc->setPositionBeats(this->smeToPosition->beat());
}

void ChangeVolumeCommand::applyEvent(ClipContainer *cc)
{
	cc->setVolume(this->fVolume);
}
