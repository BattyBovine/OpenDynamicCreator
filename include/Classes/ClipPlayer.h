#ifndef CLIPPLAYER_H
#define CLIPPLAYER_H

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
	bool loadWav(QUrl);
	bool loadVorbis(QUrl);
	void play();
	void stop();

	void setSampleRate(int s) { this->iSampleRate=s; }

	void setVolume(float v) { this->fVolume=v; if(this->aoPlayer) this->aoPlayer->setVolume(v); }

	float volume() { return this->fVolume; }

	int sampleRate() { return this->iSampleRate; }
	quint8 channelCount() { return this->iChannelCount; }
	int bitrateLower() { return this->iLowerBitrate; }
	int bitrateNominal() { return this->iNominalBitrate; }
	int bitrateUpper() { return this->iUpperBitrate; }
	int bitrateWindow() { return this->iBitrateWindow; }
	qreal length() { return this->fSeconds; }

private:
	void configurePlayer();

	int iSampleRate=0;
	quint8 iChannelCount=0;
	int iLowerBitrate=0;
	int iNominalBitrate=0;
	int iUpperBitrate=0;
	int iBitrateWindow=0;
	quint8 iSampleSize=0;
	qreal fSeconds=0.0f;

	QUrl urlFilePath;
	QBuffer bufferPCMData;
	QAudioOutput *aoPlayer=NULL;

	qreal fVolume=1.0f;

	enum ClipError {
		CLIP_OK,
		CLIP_FILE_READ_ERROR,
		CLIP_FORMAT_UNRECOGNIZED
	};
};

#endif // CLIPPLAYER_H
