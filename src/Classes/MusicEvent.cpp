#include "MusicEvent.h"
#include "ClipContainer.h"
#include "EventCommand.h"

MusicEvent::~MusicEvent()
{
	foreach(EventCommand *command, this->lCommands)
		delete command;
}

void MusicEvent::addCommand(EventCommand *e)
{
	e->setParent(this);
	this->lCommands.append(e);
}
