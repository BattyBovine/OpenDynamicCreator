#include "SongPlayer.h"

SongPlayer::Error SongPlayer::buildSong(BaseMusicItem *selected)
{
	BaseMusicItem *track = selected;
	while(track->type()!=MIT_TRACK)
		track = (BaseMusicItem*)track->parent();
	Error errorcode = this->searchItemChildren(track);
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
	if(!this->mapClips[uuidActiveClip]->isPlaying()) {
		error = this->configurePlayer(this->uuidActiveClip);
		if(error != Error::SP_OK)
			return error;
		this->mapClips[uuidActiveClip]->setPositionToAbsoluteZero();
	}
	this->aoPlayer->setVolume(this->mapClips[uuidActiveClip]->volume());
	connect(this->aoPlayer, SIGNAL(stateChanged(QAudio::State)), this, SLOT(playerState(QAudio::State)));
	this->aoPlayer->start(this->mapClips[uuidActiveClip]->buffer());
	this->mapClips[uuidActiveClip]->setIsPlaying(true);
	return error;
}
void SongPlayer::pauseSong()
{
	if(this->aoPlayer)
		this->aoPlayer->suspend();
}
void SongPlayer::stopSong()
{
	if(this->mapClips.contains(uuidActiveClip))
		this->mapClips[uuidActiveClip]->setIsPlaying(false);
	if(this->aoPlayer) {
		this->aoPlayer->stop();
		this->aoPlayer->deleteLater();
		this->aoPlayer = NULL;
	}
}



SongPlayer::Error SongPlayer::configurePlayer(QUuid clip)
{
	QAudioFormat format;
	format.setSampleRate(this->mapClips[clip]->sampleRate());
	format.setChannelCount(this->mapClips[clip]->channelCount());
	format.setSampleSize(this->mapClips[clip]->bytesPerSample()*CHAR_BIT);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	QSettings settings;
	QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
	int devicesetting = settings.value(KEY_OUTPUT_DEVICE).toInt();
	if(devices[devicesetting].isFormatSupported(format)) {
		if(this->aoPlayer) this->aoPlayer->deleteLater();
		this->aoPlayer = new QAudioOutput(devices[devicesetting], format);
	} else {
		return Error::SP_INVALID_DEVICE;
	}
	return Error::SP_OK;
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
