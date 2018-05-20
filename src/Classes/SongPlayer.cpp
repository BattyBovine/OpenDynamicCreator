#include "SongPlayer.h"

int SongPlayer::buildSong(TrackItem *track)
{
	if(track->type()!=MIT_TRACK)
		return Error::SP_NOT_TRACK;
	this->searchItemChildren(track);
	return Error::SP_OK;
}
void SongPlayer::searchItemChildren(BaseMusicItem *item)
{
	for(int i=0; i<item->rowCount(); i++) {
		BaseMusicItem *bmi = static_cast<BaseMusicItem*>(item->child(i));
		if(bmi->type()==MIT_CLIPGROUP) {
			this->searchItemChildren(bmi);
		} else if(bmi->type()==MIT_CLIP) {
			std::shared_ptr<ClipContainer> cc = ((ClipItem*)bmi)->clipContainer();
			if(this->mapClips.isEmpty())
				this->uuidActiveClip = cc->uuid();
			this->mapClips[cc->uuid()] = cc;
		}
	}
}

void SongPlayer::playSong()
{
	if(this->mapClips.isEmpty())
		return;
	if(!this->bIsPlaying) {
		this->configurePlayer(this->uuidActiveClip);
		this->mapClips[uuidActiveClip]->setPositionSeconds(0.0f);
	}
	this->aoPlayer->start(this->mapClips[uuidActiveClip]->buffer());
	this->bIsPlaying = true;
}
void SongPlayer::pauseSong()
{
	this->aoPlayer->stop();
}
void SongPlayer::stopSong()
{
	this->bIsPlaying = false;
	if(this->aoPlayer) {
		this->aoPlayer->stop();
		this->aoPlayer->deleteLater();
		this->aoPlayer = NULL;
	}
}



void SongPlayer::configurePlayer(QUuid clip)
{
	QAudioFormat format;
	format.setSampleRate(this->mapClips[clip]->sampleRate());
	format.setChannelCount(this->mapClips[clip]->channelCount());
	format.setSampleSize(this->mapClips[clip]->bytesPerSample()*CHAR_BIT);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	QAudioDeviceInfo device;
	QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
	foreach(device, devices) {
		if(device.isFormatSupported(format))
			break;
	}
	if(this->aoPlayer) this->aoPlayer->deleteLater();
	this->aoPlayer = new QAudioOutput(device, format);
}
