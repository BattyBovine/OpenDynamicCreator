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



void SongPlayer::playerState(QAudio::State s)
{
//	QAudioOutput *out = (QAudioOutput*)QObject::sender();
	switch(s) {
	case QAudio::IdleState:
		emit(finished());
		break;
//	case QAudio::StoppedState:
//		if(out->error()!=QAudio::NoError)
//			qDebug() << out->error();
//		break;
	}
}
