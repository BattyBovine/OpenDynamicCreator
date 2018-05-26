#include "MusicEvent.h"
#include "ClipContainer.h"

MusicEvent::~MusicEvent()
{
	foreach(EventCommand *command, this->lCommands)
		delete command;
}

Beat EventCommand::parentPosition()
{
	return this->meParent->beat();
}



void JumpBackCommand::applyEvent(ClipContainer *cc)
{
	cc->setPositionBeats(this->parentPosition()-this->beatToPosition);
}

void ChangeVolumeCommand::applyEvent(ClipContainer *cc)
{
	cc->setVolume(this->fVolume);
}
