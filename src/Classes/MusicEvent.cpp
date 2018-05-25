#include "MusicEvent.h"
#include "ClipContainer.h"

MusicEvent::~MusicEvent()
{
	foreach(EventCommand *command, this->lCommands)
		delete command;
}

void JumpToPositionCommand::applyEvent(ClipContainer *cc)
{
	cc->setPositionBeats(this->beatToPosition);
}

void ChangeVolumeCommand::applyEvent(ClipContainer *cc)
{
	cc->setVolume(this->fVolume);
}
