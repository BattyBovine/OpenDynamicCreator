#include "MusicEvent.h"
#include "ClipContainer.h"

MusicEvent::~MusicEvent()
{
	foreach(EventCommand *command, this->lCommands)
		delete command;
}



void JumpBackCommand::applyEvent(ClipContainer *cc, Beat eventpos)
{
	cc->setPositionBeats(eventpos-this->beatToPosition);
}

void ChangeVolumeCommand::applyEvent(ClipContainer *cc, Beat eventpos)
{
	Q_UNUSED(eventpos);
	cc->setVolume(this->fVolume);
}
