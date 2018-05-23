#include "MusicEvent.h"

void JumpToPositionCommand::applyEvent(ClipContainer *cc, float)
{
	cc->setPositionBeats(this->beatToPosition);
}
