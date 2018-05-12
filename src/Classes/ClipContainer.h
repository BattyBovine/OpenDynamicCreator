#ifndef CLIPCONTAINER_H
#define CLIPCONTAINER_H

#include <QDebug>
#include <QUrl>
#include <QByteArray>
#include <QBuffer>
#include <QAudioOutput>
#include <QPixmap>
#include <QPainter>
#include <QUuid>

#include <vorbis/vorbisfile.h>

#include "MusicEvent.h"


class ClipContainer : public QObject
{
	Q_OBJECT
public:
	ClipContainer(QUrl file=QUrl());
	ClipContainer(const ClipContainer&);
	~ClipContainer();
	ClipContainer &operator=(const ClipContainer&);
	void copy(const ClipContainer&);

	int loadAudioFile(QUrl);
	bool loadWav(QUrl);
	bool loadVorbis(QUrl);

	void play();
	void pause();
	void stop();

	bool isPlaying() { return this->bIsPlaying; }

	void setSampleRate(int s) { this->iSampleRate=s; }
	void setName(QString n) { this->sName=n; }
	void setTimeInfo(qreal t, quint8 b, quint8 u) {
		this->setTempo(t);
		this->setBeatsPerMeasure(b);
		this->setBeatUnit(u);
		this->setBeatLength(Beat::fromSeconds(this->length(), this->fTempo, this->iBeatUnit));
	}
	void setVolume(float v) { this->fVolume=v; if(this->aoPlayer) this->aoPlayer->setVolume(v); }

	QUuid uuid() { return this->uuidUnique; }
	QString uuidString() { return this->uuidUnique.toString(); }
	int sampleRate() { return this->iSampleRate; }
	quint8 channelCount() { return this->iChannelCount; }
	int bitrateLower() { return this->iLowerBitrate; }
	int bitrateNominal() { return this->iNominalBitrate; }
	int bitrateUpper() { return this->iUpperBitrate; }
	int bitrateWindow() { return this->iBitrateWindow; }
	int bytesPerSample() { return this->iBytesPerSample; }
	qreal length() { return this->fLengthSeconds; }
	Beat beats() { return this->beatLength; }
	const char *rawData() { return this->bufferPCMData.data().data(); }
	const quint64 rawDataLength() { return this->bufferPCMData.size(); }

	QString name() { return this->sName; }
	qreal tempo() { return this->fTempo; }
	quint8 beatsPerMeasure() { return this->iBeatsPerMeasure; }
	quint8 beatUnit() { return this->iBeatUnit; }
	float volume() { return this->fVolume; }
	float secondsElapsed() { return (this->bufferPCMData.pos() / float(this->iSampleRate*this->iChannelCount*this->iBytesPerSample)); }

private:
	void configurePlayer();
	void setTempo(qreal t) { this->fTempo=t; }
	void setBeatsPerMeasure(quint8 b) { this->iBeatsPerMeasure=b; }
	void setBeatUnit(quint8 u) { this->iBeatUnit=u; }
	void setBeatLength(Beat b) { this->beatLength=b; }

	QUuid uuidUnique;
	int iSampleRate = 0;
	quint8 iChannelCount = 0;
	int iLowerBitrate = 0;
	int iNominalBitrate = 0;
	int iUpperBitrate = 0;
	int iBitrateWindow = 0;
	quint8 iBytesPerSample = 0;
	qreal fLengthSeconds = 0.0f;
	Beat beatLength;

	QUrl urlFilePath;
	QBuffer bufferPCMData;
	QAudioOutput *aoPlayer = NULL;
	bool bIsPlaying = false;

	QString sName;
	qreal fTempo = 0.0f;
	quint8 iBeatsPerMeasure=0;
	quint8 iBeatUnit=0;
	qreal fVolume = 1.0f;
	qreal fPlayOffsetSeconds = 0.0f;

	enum ClipError {
		CLIP_OK,
		CLIP_FILE_READ_ERROR,
		CLIP_FORMAT_UNRECOGNIZED
	};
};

#endif // CLIPCONTAINER_H
