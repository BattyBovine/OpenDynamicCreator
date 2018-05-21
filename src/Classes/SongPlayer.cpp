#include "SongPlayer.h"

SongPlayer::Error SongPlayer::buildSong(BaseMusicItem *selected)
{
	BaseMusicItem *track = selected;
	while(track->type()!=MIT_TRACK)
		track = (BaseMusicItem*)track->parent();
	Error errorcode = this->searchItemChildren(track);
	if(errorcode!=Error::SP_OK)
		return errorcode;
	if(selected->type()==MIT_CLIP)
		this->uuidActiveClip = ((ClipItem*)selected)->clipContainer()->uuid();
	return errorcode;
}
SongPlayer::Error SongPlayer::searchItemChildren(BaseMusicItem *item)
{
	if(item->type()==MIT_CLIP) {
		this->addNewClip(item);
	} else {
		for(int i=0; i<item->rowCount(); i++) {
			BaseMusicItem *bmi = static_cast<BaseMusicItem*>(item->child(i));
			if(bmi->type()==MIT_CLIP)
				this->addNewClip(bmi);
			else
				this->searchItemChildren(bmi);
		}
	}
	if(this->mapClips.isEmpty())
		return Error::SP_NO_CLIPS;
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
	if(!this->mapClips[uuidActiveClip]->isPlaying())
		this->mapClips[uuidActiveClip]->setPositionToAbsoluteZero();
	this->mapClips[uuidActiveClip]->play();
	return error;
}
void SongPlayer::pauseSong()
{
	this->mapClips[uuidActiveClip]->pause();
}
void SongPlayer::stopSong()
{
	this->mapClips[uuidActiveClip]->stop();
}
