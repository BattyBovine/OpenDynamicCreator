#ifndef CLIPCONTAINER_H
#define CLIPCONTAINER_H

#include <QDebug>

#include <memory>
#include <vorbis/vorbisfile.h>

#include <QSettings>
#include <QUrl>
#include <QAudioOutput>
#include <QByteArray>
#include <QBuffer>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QUuid>

#include "PreferencesDialog.h"
#include "StaticMusicEvent.h"


class MusicEventWorker;
class ClipContainer;
typedef std::shared_ptr<ClipContainer> ClipContainerPtr;

class ClipContainer : public QObject
{
	Q_OBJECT
public:
	enum Error {
		CLIP_OK,
		CLIP_FILE_READ_ERROR,
		CLIP_FORMAT_UNRECOGNIZED
	};

	ClipContainer(QUrl file=QUrl(), ClipContainer *parent=NULL);
	ClipContainer(const ClipContainer&);
	~ClipContainer();
	ClipContainer &operator=(const ClipContainer&);
	void copy(const ClipContainer&);

	bool isPlaying() { return this->bIsPlaying; }

	Error loadAudioFile(QUrl);
	bool loadWav(QUrl);
	bool loadVorbis(QUrl);

	void setSampleRate(int s) { this->iSampleRate=s; }
	void setName(QString n) { this->sName=n; }
	void setTimeInfo(qreal t, quint8 b, quint8 u) {
		this->setTempo(t);
		this->setBeatsPerMeasure(b);
		this->setBeatUnit(u);
		this->setBeatLength(Beat::fromSeconds(this->length(), this->fTempo, this->iBeatUnit));
	}

	QUuid uuid() { return this->uuidUnique; }
	QString uuidString() { return this->uuidUnique.toString(); }
	int sampleRate() { return this->iSampleRate; }
	quint8 channelCount() { return this->iChannelCount; }
	int bitrateLower() { return this->iLowerBitrate; }
	int bitrateNominal() { return this->iNominalBitrate; }
	int bitrateUpper() { return this->iUpperBitrate; }
	int bitrateWindow() { return this->iBitrateWindow; }
	int bytesPerSample() { return this->iBytesPerSample; }
	bool isGroupClip() { return this->bIsGroupClip; }
	const char *rawData() { return this->bufferPCMData.data().data(); }
	const quint64 rawDataLength() { return this->bufferPCMData.size(); }
	QAudioOutput *audioPlayer() { return this->aoAudioPlayer; }
	QBuffer *buffer() { return &this->bufferPCMData; }
	QByteArray *pcmData() { return &this->baPCMData; }

	void setVolume(qreal);
	void setPositionBeats(Beat b=Beat()) { this->setPositionSeconds(b.toSeconds(this->fTempo, this->iBeatUnit)); }
	void setPositionSeconds(float);
	void setPositionToAbsoluteZero();

	void setTimelineOffset(Beat b) { this->beatTimelineOffset=b; }
	qreal length() { return this->fLengthSeconds; }
	Beat beats() { return this->beatLength; }

	QString name() { return this->sName; }
	qreal tempo() { return this->fTempo; }
	quint8 beatsPerMeasure() { return this->iBeatsPerMeasure; }
	quint8 beatUnit() { return this->iBeatUnit; }
	Beat timelineOffset() { return this->beatTimelineOffset; }
	qreal volume() { return this->fVolume; }
	float secondsElapsed();

	void addEvent(MusicEventPtr e, Beat pos) {
		StaticMusicEventPtr sme = std::make_shared<StaticMusicEvent>(StaticMusicEvent(e,pos));
		this->smelEvents.append(sme);
		std::sort(this->smelEvents.begin(), this->smelEvents.end());
		emit(eventAdded(sme));
	}
	StaticMusicEventList &events() { return this->smelEvents; }

	void addSubClip(ClipContainerPtr clip) { clip->setParent(this); this->lChildClips.append(clip); this->bIsGroupClip=true; }

	bool play();
	void pause();
	void stop();

private slots:
	void handleEvent(MusicEvent *me) { emit(eventFired(me)); this->setNextEvent(); }
	void playerState(QAudio::State);
	void setPlayerVolume(qreal);
	void setNextEvent();
	void stopEventThread();

signals:
	void finished();
	void volumeChanged(qreal);
	void eventAdded(StaticMusicEventPtr);
	void eventFired(MusicEvent*);

private:
	void setParent(ClipContainer *cc) { this->ccParent=cc; }
	Error configurePlayer();
	void setTempo(qreal t) { this->fTempo=t; }
	void setBeatsPerMeasure(quint8 b) { this->iBeatsPerMeasure=b; }
	void setBeatUnit(quint8 u) { this->iBeatUnit=u; }
	void setBeatLength(Beat b) { this->beatLength=b; }

	void startEventThread();

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
	QByteArray baPCMData;
	QBuffer bufferPCMData;
	bool bIsPlaying = false;
	MusicEventWorker *mewEventWorker = NULL;

	QString sName;
	QAudioOutput *aoAudioPlayer = NULL;
	qreal fTempo = 0.0f;
	quint8 iBeatsPerMeasure = 0;
	quint8 iBeatUnit = 0;
	qreal fVolume = 1.0f;
	Beat beatTimelineOffset;

	ClipContainer *ccParent = NULL;
	QList<ClipContainerPtr> lChildClips;
	StaticMusicEventList smelEvents;
	bool bIsGroupClip = false;
};

#endif // CLIPCONTAINER_H
