#include "ClipContainer.h"
#include "MusicEventWorker.h"

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

void ClipContainer::copy(const ClipContainer &c)
{
	this->uuidUnique = c.uuidUnique;
	this->urlFilePath = c.urlFilePath;
	this->baPCMData = c.baPCMData;
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
	this->smelEvents = c.smelEvents;
	this->ccParent = c.ccParent;
}

ClipContainer::Error ClipContainer::loadAudioFile(QUrl file)
{
	if(!this->loadWav(file) && !this->loadVorbis(file))
		return CLIP_FORMAT_UNRECOGNIZED;
	return CLIP_OK;
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
	this->baPCMData.clear();
	this->baPCMData.append(pcmbuffer, bytecount);
	delete pcmbuffer;
	drwav_uninit(&wav);

	this->fLengthSeconds = this->baPCMData.size() / float(this->iSampleRate * this->iBytesPerSample * this->iChannelCount);

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

	this->baPCMData.clear();
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
			this->baPCMData.append(pcmbuffer, retval);
	}
	ov_clear(&vorb);

	this->fLengthSeconds = this->baPCMData.size() / float(this->iSampleRate * this->iBytesPerSample * this->iChannelCount);

	return true;
}

QAudioOutput *ClipContainer::createPlayer()
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
	if(devices[devicesetting].isFormatSupported(format)) {
		QAudioOutput *out = new QAudioOutput(devices[devicesetting], format);
		out->setVolume(this->volume());
		return out;
	}

	return NULL;
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
