#include "SongPlayer.h"

SongPlayer::Error SongPlayer::buildSong(BaseMusicItem *selected)
{
	BaseMusicItem *track = selected;
	while(track->type()!=MIT_TRACK)
		track = (BaseMusicItem*)track->parent();
	Error errorcode = this->searchItemChildren(track);
	if(!this->cpActiveSegment)
		errorcode = Error::SP_INVALID_ACTIVE_CLIP;
	return errorcode;
}
SongPlayer::Error SongPlayer::searchItemChildren(BaseMusicItem *item)
{
	ClipContainerPtr cc = item->clipContainer();
	if(item->type()==MIT_CLIP) {
		this->hashGroupMap.insert(cc->uuid(), cc->uuid());
		this->hashClips[cc->uuid()] = cc.get();
	} else {
		for(int i=0; i<item->rowCount(); i++) {
			BaseMusicItem *bmi = static_cast<BaseMusicItem*>(item->child(i));
			if(bmi->type()==MIT_CLIPGROUP) {
				ClipContainerPtr childcc = bmi->clipContainer();
				this->hashGroupMap.insert(cc->uuid(), childcc->uuid());
			}
			this->searchItemChildren(bmi);
		}
	}
	if(item->type()!=MIT_TRACK && !this->cpActiveSegment)
		this->cpActiveSegment = this->createClipPlayer(cc->uuid());
	return Error::SP_OK;
}
ClipPlayer *SongPlayer::createClipPlayer(QUuid &clipgroup)
{
	ClipPlayer *cplay = new ClipPlayer();
	QList<QUuid> groupclips = this->hashGroupMap.values(clipgroup);
	foreach(QUuid clip, groupclips)
		cplay->addAudioData(this->hashClips[clip]);
	return cplay;
}

SongPlayer::Error SongPlayer::playSong()
{
	Error error = Error::SP_OK;
//	if(this->mapClips.isEmpty())
//		return Error::SP_NO_CLIPS;
//	if(!this->mapClips[uuidActiveClip])
//		return Error::SP_INVALID_ACTIVE_CLIP;
//	if(!this->mapClips[uuidActiveClip]->isPlaying())
//		this->mapClips[uuidActiveClip]->setPositionToAbsoluteZero();
//	connect(this->mapClips[uuidActiveClip].get(), SIGNAL(eventFired(MusicEvent*)), this, SLOT(applyEvent(MusicEvent*)));
//	this->mapClips[uuidActiveClip]->play();
	return error;
}
void SongPlayer::pauseSong()
{
//	if(this->mapClips[uuidActiveClip])
//		this->mapClips[uuidActiveClip]->pause();
}
void SongPlayer::stopSong()
{
//	if(this->mapClips[uuidActiveClip])
//		this->mapClips[uuidActiveClip]->stop();
}
void SongPlayer::applyEvent(MusicEvent *me)
{
	Q_UNUSED(me);
//	QList<EventCommand*> commands = me->commands();
//	foreach(EventCommand* command, commands)
//		command->applyEvent(this->mapClips[uuidActiveClip].get());
}
