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


SongPlayer::Error SongPlayer::buildSong(BaseMusicItem *track)
{
	if(track->type()!=MIT_TRACK)
		return Error::SP_INVALID_TRACK;
	int children = track->rowCount();
	Error error = Error::SP_OK;
	for(int i=0; i<children; i++) {
		error = this->searchItemChildren((BaseMusicItem*)track->child(i));
		if(error!=Error::SP_OK)
			break;
	}
	return error;
}
SongPlayer::Error SongPlayer::searchItemChildren(BaseMusicItem *item)
{
	ClipContainer *cc = item->clipContainer().get();
	this->hashClips[cc->uuid()] = cc;
	connect(cc, SIGNAL(volumeChanged(qreal)), this, SLOT(setVolume(qreal)));
	if(item->type()==MIT_CLIP) {
		this->hashGroupMap.insert(cc->uuid(), cc->uuid());
	} else {
		for(int i=0; i<item->rowCount(); i++) {
			BaseMusicItem *bmi = static_cast<BaseMusicItem*>(item->child(i));
			ClipContainerPtr childcc = bmi->clipContainer();
			this->hashGroupMap.insert(cc->uuid(), childcc->uuid());
			this->searchItemChildren(bmi);
		}
	}
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



SongPlayer::Error SongPlayer::playSong(QUuid &startclip)
{
	Error error = Error::SP_OK;
	if(this->hashClips.isEmpty() || this->hashGroupMap.isEmpty())
		return Error::SP_NO_CLIPS;
	if(startclip==QUuid()) {
		if(!this->cpActiveSegment)
			return Error::SP_INVALID_ACTIVE_SEGMENT;
	} else {
		if(this->cpActiveSegment) this->cpActiveSegment->deleteLater();
			this->cpActiveSegment = this->createClipPlayer(startclip);
	}
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



void SongPlayer::setVolume(qreal v)
{
	if(!this->cpActiveSegment)
		return;
	ClipContainer *cc = static_cast<ClipContainer*>(QObject::sender());
	QList<QUuid> clips = this->hashGroupMap.values(cc->uuid());
	foreach(QUuid clip, clips)
		this->cpActiveSegment->setVolume(clip, v);
}
