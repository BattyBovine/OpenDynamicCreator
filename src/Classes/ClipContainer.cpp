#include "Classes/ClipContainer.h"

#define DR_WAV_IMPLEMENTATION
#include <Libraries/dr_wav.h>
#undef DR_WAV_IMPLEMENTATION

ClipContainer::ClipContainer(QUrl file, ClipContainer *parent)
{
	this->uuidUnique = QUuid::createUuid();
	this->ccParent = parent;
	this->loadAudioFile(file);
}
ClipContainer::ClipContainer(const ClipContainer &c)
{
	this->copy(c);
}
ClipContainer &ClipContainer::operator=(const ClipContainer &c)
{
	this->copy(c);
	return *this;
}
ClipContainer::~ClipContainer()
{
	this->bufferPCMData.close();
	this->stop();
	foreach(MusicEvent *event, this->melEvents)
		delete event;
	if(this->mewEventWorker) this->mewEventWorker->deleteLater();
	if(this->aoAudioPlayer) this->aoAudioPlayer->deleteLater();
}
void ClipContainer::copy(const ClipContainer &c)
{
	this->uuidUnique = c.uuidUnique;
	this->urlFilePath = c.urlFilePath;
	this->bufferPCMData.setData(c.bufferPCMData.buffer());
	this->iSampleRate = c.iSampleRate;
	this->iChannelCount = c.iChannelCount;
	this->iLowerBitrate = c.iLowerBitrate;
	this->iNominalBitrate = c.iNominalBitrate;
	this->iUpperBitrate = c.iUpperBitrate;
	this->iBitrateWindow = c.iBitrateWindow;
	this->iBytesPerSample = c.iBytesPerSample;
	this->fLengthSeconds = c.fLengthSeconds;
	this->beatLength = c.beatLength;
	this->fVolume = c.fVolume;
	this->melEvents = c.melEvents;
	this->ccParent = c.ccParent;
	this->configurePlayer();
}

ClipContainer::Error ClipContainer::loadAudioFile(QUrl file)
{
	if(!this->loadWav(file) && !this->loadVorbis(file))
		return CLIP_FORMAT_UNRECOGNIZED;
	this->bufferPCMData.open(QIODevice::ReadOnly);
	return this->configurePlayer();
}
bool ClipContainer::loadWav(QUrl file)
{
	drwav wav;
	if(!drwav_init_file(&wav, file.toLocalFile().toStdString().c_str()))
		return false;
	this->urlFilePath = file;

	this->iSampleRate = wav.sampleRate;
	this->iChannelCount = wav.channels;
	this->iLowerBitrate = this->iNominalBitrate = this->iUpperBitrate = this->iBitrateWindow = 0;
	this->iBytesPerSample = wav.bytesPerSample;

	char *pcmbuffer = new char[wav.totalSampleCount*wav.bytesPerSample*wav.channels];
	quint64 bytecount = drwav_read_raw(&wav, wav.totalSampleCount*wav.bytesPerSample, pcmbuffer);
	this->bufferPCMData.buffer().clear();
	this->bufferPCMData.buffer().append(pcmbuffer, bytecount);
	delete pcmbuffer;
	drwav_uninit(&wav);

	this->fLengthSeconds = this->bufferPCMData.size() / float(this->iSampleRate * this->iBytesPerSample * this->iChannelCount);

	return true;
}
bool ClipContainer::loadVorbis(QUrl file)
{
	OggVorbis_File vorb;
	if(ov_fopen(file.toLocalFile().toStdString().c_str(), &vorb) < 0)
		return false;
	this->urlFilePath = file;

	this->iSampleRate = vorb.vi->rate;
	this->iChannelCount = vorb.vi->channels;
	this->iLowerBitrate = vorb.vi->bitrate_lower;
	this->iNominalBitrate = vorb.vi->bitrate_nominal;
	this->iUpperBitrate = vorb.vi->bitrate_upper;
	this->iBitrateWindow = vorb.vi->bitrate_window;
	this->iBytesPerSample = 2;

	this->bufferPCMData.buffer().clear();
	char pcmbuffer[4096];
	int section;
	bool eof = false;
	while(!eof) {
		long retval = ov_read(&vorb, pcmbuffer, sizeof(pcmbuffer), 0, 2, 1, &section);
		if(retval==0)
			eof = true;
		else if(retval<0)
			qDebug() << QString("Error decoding Ogg Vorbis data; attempting to ignore...");
		else
			this->bufferPCMData.buffer().append(pcmbuffer, retval);
	}
	ov_clear(&vorb);

	this->fLengthSeconds = this->bufferPCMData.size() / float(this->iSampleRate * this->iBytesPerSample * this->iChannelCount);

	return true;
}

ClipContainer::Error ClipContainer::configurePlayer()
{
	QAudioFormat format;
	format.setSampleRate(this->sampleRate());
	format.setChannelCount(this->channelCount());
	format.setSampleSize(this->bytesPerSample()*CHAR_BIT);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	QSettings settings;
	QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
	int devicesetting = settings.value(KEY_OUTPUT_DEVICE).toInt();
	if(!devices[devicesetting].isFormatSupported(format))
		return Error::CLIP_FORMAT_UNRECOGNIZED;

	if(this->aoAudioPlayer) this->aoAudioPlayer->deleteLater();
	this->aoAudioPlayer = new QAudioOutput(devices[devicesetting], format);
	this->aoAudioPlayer->setVolume(this->volume());
	connect(this->aoAudioPlayer, SIGNAL(stateChanged(QAudio::State)), this, SLOT(playerState(QAudio::State)));
	connect(this, SIGNAL(volumeChanged(qreal)), this, SLOT(setPlayerVolume(qreal)));
	this->bufferPCMData.open(QIODevice::ReadOnly);
	return Error::CLIP_OK;
}



void ClipContainer::setVolume(qreal v)
{
	if(v==this->fVolume)
		return;
	float adjustedvolume;
	this->fVolume = adjustedvolume = v;
	if(this->ccParent)
		adjustedvolume *= this->ccParent->volume();
	emit(volumeChanged(adjustedvolume));
}

void ClipContainer::setPositionSeconds(float seconds)
{
	seconds -= this->beatTimelineOffset.toSeconds(this->fTempo, this->iBeatUnit);
	if(seconds<0.0f)
		seconds = 0.0f;
	if(this->bIsGroupClip)
		foreach(std::shared_ptr<ClipContainer> cc, this->lChildClips)
			cc->setPositionSeconds(seconds);
	else
		this->bufferPCMData.seek(floorf(seconds*this->iSampleRate)*(this->iBytesPerSample*this->iChannelCount));
}

float ClipContainer::secondsElapsed()
{
	if(this->bIsGroupClip) {
		float elapsed = FLT_MAX;
		foreach(std::shared_ptr<ClipContainer> cc, this->lChildClips) {
			if(cc->secondsElapsed()<elapsed)
				elapsed = cc->secondsElapsed();
		}
		return elapsed;
	} else {
		return ((this->bufferPCMData.pos() / float(this->iSampleRate*this->iChannelCount*this->iBytesPerSample)) +
				this->beatTimelineOffset.toSeconds(this->fTempo, this->iBeatUnit));
	}
}

void ClipContainer::setPositionToAbsoluteZero()
{
	if(this->bIsGroupClip)
		foreach(std::shared_ptr<ClipContainer> cc, this->lChildClips)
			cc->setPositionToAbsoluteZero();
	else
		this->bufferPCMData.seek(0);
}



bool ClipContainer::play()
{
	if(this->bIsGroupClip) {
		foreach(std::shared_ptr<ClipContainer> cc, this->lChildClips) {
			cc->setPositionSeconds(this->secondsElapsed());
			cc->play();
		}
	} else {
		if(!this->aoAudioPlayer)
			return false;
		if(!this->bIsPlaying)
			this->setPositionToAbsoluteZero();
		this->aoAudioPlayer->setVolume(this->volume());
		connect(this->aoAudioPlayer, SIGNAL(stateChanged(QAudio::State)), this, SLOT(playerState(QAudio::State)));
		this->configureNextEvent();
		this->aoAudioPlayer->start(&this->bufferPCMData);
	}
	this->bIsPlaying = true;
	return true;
}
void ClipContainer::pause()
{
	if(this->bIsGroupClip) {
		foreach(std::shared_ptr<ClipContainer> cc, this->lChildClips)
			cc->pause();
	} else {
		if(this->aoAudioPlayer)
			this->aoAudioPlayer->suspend();
	}
	this->stopEventThread();
}
void ClipContainer::stop()
{
	if(this->bIsGroupClip) {
		foreach(std::shared_ptr<ClipContainer> cc, this->lChildClips)
			cc->stop();
	} else {
		if(this->aoAudioPlayer)
			this->aoAudioPlayer->stop();
	}
	this->stopEventThread();
	this->meNextEvent = NULL;
	this->bIsPlaying = false;
}



void ClipContainer::configureNextEvent()
{
	if(this->aoAudioPlayer) {
		for(MusicEventList::Iterator i=this->melEvents.begin(); i!=this->melEvents.end(); i++) {
			float eventsecs = ((*i)->beat()-this->beatTimelineOffset).toSeconds(this->tempo(), this->beatUnit());
			if(eventsecs < this->secondsElapsed())
				continue;
			int buffersecs = this->aoAudioPlayer->bufferSize() /
							 float(this->iSampleRate*this->iBytesPerSample*this->iChannelCount);
			this->meNextEvent = i;
			this->setNextEvent(eventsecs-buffersecs);
			break;
		}
	}
}
void ClipContainer::handleEvent()
{
	if(this->meNextEvent) {
		emit(eventFired(*this->meNextEvent));
		this->configureNextEvent();
	}
}



void ClipContainer::setPlayerVolume(qreal v)
{
	if(this->aoAudioPlayer)
		this->aoAudioPlayer->setVolume(v);
}

void ClipContainer::playerState(QAudio::State s)
{
	switch(s) {
	case QAudio::ActiveState:
		this->bIsPlaying = true;
		break;
	case QAudio::StoppedState:
		this->bIsPlaying = false;
		if(this->aoAudioPlayer->error()!=QAudio::NoError)
			qDebug() << this->aoAudioPlayer->error();
		break;
	case QAudio::IdleState:
		emit(finished());
		break;
	}
}

void ClipContainer::setNextEvent(float secs)
{
	this->stopEventThread();
	this->mewEventWorker = new MusicEventWorker(this);
	this->mewEventWorker->setBuffer(&this->bufferPCMData);
	this->mewEventWorker->setEventTime(secs, this->iSampleRate, this->iBytesPerSample, this->iChannelCount);
	connect(this->mewEventWorker, SIGNAL(musicEvent()), this, SLOT(handleEvent()));
	connect(this->mewEventWorker, SIGNAL(finished()), this->mewEventWorker, SLOT(deleteLater()));
#ifdef QT_DEBUG
	connect(this->mewEventWorker, SIGNAL(started()), this, SLOT(threadStarted()));
	connect(this->mewEventWorker, SIGNAL(finished()), this, SLOT(threadKilled()));
#endif
	this->mewEventWorker->start(QThread::LowestPriority);
}
void ClipContainer::stopEventThread()
{
	if(this->mewEventWorker) {
		if(this->mewEventWorker->isRunning())
			this->mewEventWorker->stop();
		this->mewEventWorker = NULL;
	}
}
