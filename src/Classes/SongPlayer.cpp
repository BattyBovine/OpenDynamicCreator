#include "SongPlayer.h"

SongPlayer::Error SongPlayer::buildSong(BaseMusicItem *selected)
{
	BaseMusicItem *track = selected;
	while(track->type()!=MIT_TRACK)
		track = (BaseMusicItem*)track->parent();
	Error errorcode = this->searchItemChildren(track);
	if(selected->type()!=MIT_TRACK)
		this->uuidActiveClip = ((BaseMusicItem*)selected)->clipContainer()->uuid();
	return errorcode;
}
SongPlayer::Error SongPlayer::searchItemChildren(BaseMusicItem *item)
{
	if(item->type()==MIT_CLIP) {
		this->addNewClip(item);
	} else {
		for(int i=0; i<item->rowCount(); i++) {
			BaseMusicItem *bmi = static_cast<BaseMusicItem*>(item->child(i));
			this->addNewClip(bmi);
			if(bmi->type()==MIT_CLIPGROUP)
				this->searchItemChildren(bmi);
		}
	}
	return Error::SP_OK;
}
void SongPlayer::addNewClip(BaseMusicItem *bmi)
{
	std::shared_ptr<ClipContainer> cc = ((ClipItem*)bmi)->clipContainer();
	if(this->mapClips.isEmpty())
		this->uuidActiveClip = cc->uuid();
	this->mapClips[cc->uuid()] = cc;
}

SongPlayer::Error SongPlayer::playSong()
{
	Error error = Error::SP_OK;
	if(this->mapClips.isEmpty())
		return Error::SP_NO_CLIPS;
	if(!this->mapClips[uuidActiveClip])
		return Error::SP_INVALID_ACTIVE_CLIP;
	if(!this->mapClips[uuidActiveClip]->isPlaying())
		this->mapClips[uuidActiveClip]->setPositionToAbsoluteZero();
	connect(this->mapClips[uuidActiveClip].get(), SIGNAL(eventFired(StaticMusicEventPtr)), this, SLOT(applyEvent(StaticMusicEventPtr)));
	this->mapClips[uuidActiveClip]->play();
	return error;
}
void SongPlayer::pauseSong()
{
	if(this->mapClips[uuidActiveClip])
		this->mapClips[uuidActiveClip]->pause();
}
void SongPlayer::stopSong()
{
	if(this->mapClips[uuidActiveClip])
		this->mapClips[uuidActiveClip]->stop();
}
void SongPlayer::applyEvent(StaticMusicEventPtr sme)
{
	QList<EventCommand*> commands = sme->musicEvent()->commands();
	foreach(EventCommand* command, commands)
		command->applyEvent(this->mapClips[uuidActiveClip].get(), sme->beat());
}
