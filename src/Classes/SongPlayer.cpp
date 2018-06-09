#include "SongPlayer.h"


SongPlayer::~SongPlayer()
{
	if(this->cpActiveSegment)
		this->cpActiveSegment->deleteLater();
	if(this->cpTransitionSegment)
		this->cpTransitionSegment->deleteLater();
	while(!this->queueSegments.isEmpty())
		this->queueSegments.dequeue()->deleteLater();
}


SongPlayer::Error SongPlayer::buildSong(BaseMusicItem *selected)
{
	BaseMusicItem *track = selected;
	while(track->type()!=MIT_TRACK)
		track = (BaseMusicItem*)track->parent();
	Error errorcode = this->searchItemChildren(track);
	if(!this->cpActiveSegment)
		errorcode = Error::SP_INVALID_ACTIVE_SEGMENT;
	return errorcode;
}
SongPlayer::Error SongPlayer::searchItemChildren(BaseMusicItem *item)
{
	ClipContainerPtr cc = item->clipContainer();
	this->hashClips[cc->uuid()] = cc.get();
	if(item->type()==MIT_CLIP) {
		this->hashGroupMap.insert(cc->uuid(), cc->uuid());
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
	ClipPlayer *cplay = new ClipPlayer(this->hashClips[clipgroup], this);
	QList<QUuid> groupclips = this->hashGroupMap.values(clipgroup);
	foreach(QUuid clip, groupclips) {
		ClipContainer *cc = this->hashClips[clip];
		cplay->addClipData(cc);
	}
	return cplay;
}



SongPlayer::Error SongPlayer::playSong()
{
	Error error = Error::SP_OK;
	if(this->hashClips.isEmpty() || this->hashGroupMap.isEmpty())
		return Error::SP_NO_CLIPS;
	if(!this->cpActiveSegment)
		return Error::SP_INVALID_ACTIVE_SEGMENT;
	this->cpActiveSegment->play(this);
	return error;
}

void SongPlayer::pauseSong()
{
	if(this->cpActiveSegment)
		this->cpActiveSegment->pause();
}

void SongPlayer::stopSong()
{
	if(this->cpActiveSegment)
		this->cpActiveSegment->stop();
}

void SongPlayer::applyEvent(MusicEvent *me)
{
	qDebug() << QString("Event fired: %1").arg(me->uuidString());
	QList<EventCommand*> commands = me->commands();
//	QList<QUuid> cliplist = this->hashGroupMap.values(uuid);
//	foreach(QUuid clipid, cliplist) {
	foreach(EventCommand* command, commands)
		command->applyEvent(this->hashClips[command->clip()]);
//	}
}
