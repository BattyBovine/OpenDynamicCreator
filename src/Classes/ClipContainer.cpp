#include "Classes/ClipContainer.h"

#define DR_WAV_IMPLEMENTATION
#include <Libraries/dr_wav.h>
#undef DR_WAV_IMPLEMENTATION

ClipContainer::ClipContainer(QUrl file)
{
	this->uuidUnique = QUuid::createUuid();
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
}

int ClipContainer::loadAudioFile(QUrl file)
{
	if(!this->loadWav(file) && !this->loadVorbis(file))
		return CLIP_FORMAT_UNRECOGNIZED;
	this->bufferPCMData.open(QIODevice::ReadOnly);
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



void ClipContainer::setPositionSeconds(float seconds)
{
	seconds -= this->beatTimelineOffset.toSeconds(this->fTempo, this->iBeatUnit);
	if(seconds<0.0f)
		seconds = 0.0f;
	this->bufferPCMData.seek(floorf(seconds*this->iSampleRate)*(this->iBytesPerSample*this->iChannelCount));
}

float ClipContainer::secondsElapsed()
{
	return ((this->bufferPCMData.pos() / float(this->iSampleRate*this->iChannelCount*this->iBytesPerSample)) +
			this->beatTimelineOffset.toSeconds(this->fTempo, this->iBeatUnit));
}
