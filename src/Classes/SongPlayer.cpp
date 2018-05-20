#include "SongPlayer.h"

int SongPlayer::buildSong(BaseMusicItem *selected)
{
	BaseMusicItem *track = selected;
	while(track->type()!=MIT_TRACK)
		track = (BaseMusicItem*)track->parent();
	int errorcode = this->searchItemChildren(track);
	if(selected->type()==MIT_CLIP)
		this->uuidActiveClip = ((ClipItem*)selected)->clipContainer()->uuid();
	return errorcode;
}
int SongPlayer::searchItemChildren(BaseMusicItem *item)
{
	if(item->type()==MIT_CLIP) {
		this->addNewClip(item);
	} else {
		for(int i=0; i<item->rowCount(); i++) {
			BaseMusicItem *bmi = static_cast<BaseMusicItem*>(item->child(i));
			if(bmi->type()==MIT_CLIPGROUP) {
				this->searchItemChildren(bmi);
			} else if(bmi->type()==MIT_CLIP) {
				this->addNewClip(bmi);
			}
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

void SongPlayer::playSong()
{
	if(this->mapClips.isEmpty())
		return;
	if(!this->mapClips[uuidActiveClip]->isPlaying()) {
		this->configurePlayer(this->uuidActiveClip);
		this->mapClips[uuidActiveClip]->setPositionToAbsoluteZero();
	}
	this->aoPlayer->setVolume(this->mapClips[uuidActiveClip]->volume());
	this->aoPlayer->start(this->mapClips[uuidActiveClip]->buffer());
	this->mapClips[uuidActiveClip]->setIsPlaying(true);
}
void SongPlayer::pauseSong()
{
	if(this->aoPlayer)
		this->aoPlayer->suspend();
}
void SongPlayer::stopSong()
{
	if(this->mapClips.size()>0)
		this->mapClips[uuidActiveClip]->setIsPlaying(false);
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
	this->aoPlayer->setBufferSize(0);
}
