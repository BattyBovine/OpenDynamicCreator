#include "Classes/ClipPlayer.h"

#define DR_WAV_IMPLEMENTATION
#include <Libraries/dr_wav.h>
#undef DR_WAV_IMPLEMENTATION

ClipPlayer::ClipPlayer(QUrl file)
{
	this->loadAudioFile(file);
}
ClipPlayer::~ClipPlayer()
{
	this->stop();
	if(this->aoPlayer) this->aoPlayer->deleteLater();
}

int ClipPlayer::loadAudioFile(QUrl file)
{
	this->stop();
	if(!this->loadWav(file) && !this->loadVorbis(file))
		return CLIP_FORMAT_UNRECOGNIZED;
	this->configurePlayer();
	return CLIP_OK;
}
bool ClipPlayer::loadWav(QUrl file)
{
	drwav wav;
	if(!drwav_init_file(&wav, file.toLocalFile().toStdString().c_str()))
		return false;
	this->urlFilePath = file;

	this->iSampleRate = wav.sampleRate;
	this->iChannelCount = wav.channels;
	this->iLowerBitrate = this->iNominalBitrate = this->iUpperBitrate = this->iBitrateWindow = 0;
	this->iSampleSize = wav.bitsPerSample;
	this->fSeconds = wav.totalSampleCount / float(wav.sampleRate) / float(wav.bytesPerSample);

	char *pcmbuffer = new char[wav.totalSampleCount*wav.bytesPerSample];
	size_t bytecount = drwav_read_raw(&wav, wav.totalSampleCount*wav.bytesPerSample, pcmbuffer);
	this->bufferPCMData.buffer().clear();
	this->bufferPCMData.buffer().append(pcmbuffer, bytecount);
	delete pcmbuffer;
	drwav_uninit(&wav);

	return true;
}
bool ClipPlayer::loadVorbis(QUrl file)
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
	this->iSampleSize = 16;
	this->fSeconds = ov_time_total(&vorb,-1);

	this->bufferPCMData.buffer().clear();
	char pcmbuffer[4096];
	int section;
	bool eof = false;
	while(!eof) {
		long retval = ov_read(&vorb, pcmbuffer, sizeof(pcmbuffer), 0, 2, 1, &section);
		if(retval==0) {
			eof = true;
		} else if(retval<0) {
			qDebug() << QString("Error decoding Ogg Vorbis data; attempting to ignore...");
		} else {
			this->bufferPCMData.buffer().append(pcmbuffer, retval);
		}
	}
	ov_clear(&vorb);

	return true;
}

void ClipPlayer::configurePlayer()
{
	QAudioFormat format;
	format.setSampleRate(this->iSampleRate);
	format.setChannelCount(this->iChannelCount);
	format.setSampleSize(this->iSampleSize);
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
	this->aoPlayer->setVolume(this->fVolume);
}

void ClipPlayer::play()
{
	if(!this->bufferPCMData.isOpen())
		this->bufferPCMData.open(QIODevice::ReadOnly);
	this->aoPlayer->start(&this->bufferPCMData);
}

void ClipPlayer::pause()
{
	if(this->aoPlayer) this->aoPlayer->stop();
}

void ClipPlayer::stop()
{
	this->pause();
	this->bufferPCMData.close();
}
