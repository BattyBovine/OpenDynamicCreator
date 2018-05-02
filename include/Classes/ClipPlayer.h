#ifndef PCMDATA_H
#define PCMDATA_H

#include <QDebug>
#include <QUrl>
#include <QByteArray>
#include <QBuffer>
#include <QAudioOutput>

#include <vorbis/vorbisfile.h>


class ClipPlayer
{
public:
	ClipPlayer(QUrl file=QUrl());
	~ClipPlayer();

	int loadAudioFile(QUrl);
	bool loadVorbis(QUrl, OggVorbis_File);
	void play();

	void setSampleRate(int s) { this->iSampleRate=s; }

	QByteArray *data() { return &this->baPCMData; }
	int sampleRate() { return this->iSampleRate; }
	quint8 channelCount() { return this->iChannelCount; }
	int bitrateLower() { return this->iLowerBitrate; }
	int bitrateNominal() { return this->iNominalBitrate; }
	int bitrateUpper() { return this->iUpperBitrate; }
	int bitrateWindow() { return this->iBitrateWindow; }
	qreal length() { return this->fSeconds; }

private:
	int iSampleRate=0;
	quint8 iChannelCount=0;
	int iLowerBitrate=0;
	int iNominalBitrate=0;
	int iUpperBitrate=0;
	int iBitrateWindow=0;
	qreal fSeconds=0.0f;

	QUrl urlFilePath;
	QByteArray baPCMData;
	QAudioOutput *aoPlayer=NULL;

	enum ClipError {
		CLIP_OK,
		CLIP_FILE_READ_ERROR,
		CLIP_FORMAT_UNRECOGNIZED
	};
};

#endif // PCMDATA_H
