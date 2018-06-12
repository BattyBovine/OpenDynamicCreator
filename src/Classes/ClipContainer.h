#ifndef CLIPCONTAINER_H
#define CLIPCONTAINER_H

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
	ClipContainer &operator=(const ClipContainer&);
	void copy(const ClipContainer&);

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

	QUuid &uuid() { return this->uuidUnique; }
	QString uuidString() { return this->uuidUnique.toString(); }
	int sampleRate() { return this->iSampleRate; }
	quint8 channelCount() { return this->iChannelCount; }
	int bitrateLower() { return this->iLowerBitrate; }
	int bitrateNominal() { return this->iNominalBitrate; }
	int bitrateUpper() { return this->iUpperBitrate; }
	int bitrateWindow() { return this->iBitrateWindow; }
	int bytesPerSample() { return this->iBytesPerSample; }
	bool isGroupClip() { return this->bIsGroupClip; }
	const char *rawData() { return this->baPCMData; }
	const quint64 rawDataLength() { return this->baPCMData.size(); }
	QAudioOutput *createPlayer();
	QByteArray *pcmData() { return &this->baPCMData; }

	void setVolume(qint16 v) { this->iVolume=v; emit(volumeChanged(this->volumedB())); emit(mixedVolumeChanged(this->mixedVolumedB())); }
	void setPositionBeats(Beat b=Beat()) { this->setPositionSeconds(b.toSeconds(this->fTempo,this->iBeatUnit)); }
	void setPositionSeconds(qreal s) { this->fPositionSeconds=s; emit(secondsPositionChanged(s)); }

	void setTimelineOffset(Beat b) { this->beatTimelineOffset=b; }
	qreal length() { return this->fLengthSeconds; }
	Beat beats() { return this->beatLength; }

	QString name() { return this->sName; }
	qreal tempo() { return this->fTempo; }
	quint8 beatsPerMeasure() { return this->iBeatsPerMeasure; }
	quint8 beatUnit() { return this->iBeatUnit; }
	Beat timelineOffset() { return this->beatTimelineOffset; }
	qint16 volumedB() { return this->iVolume; }
	qint16 mixedVolumedB() { return (this->iVolume + (this->ccParent ? this->ccParent->mixedVolumedB() : 0)); }
	qreal volumePercent() { return QAudio::convertVolume(this->volumedB(), QAudio::DecibelVolumeScale, QAudio::LogarithmicVolumeScale); }
	qreal mixedVolumePercent() { return QAudio::convertVolume(this->mixedVolumedB(), QAudio::DecibelVolumeScale, QAudio::LogarithmicVolumeScale); }
	float secondsElapsed() { return this->fPositionSeconds; }

	void addEvent(MusicEventPtr e, Beat pos) {
		StaticMusicEventPtr sme = std::make_shared<StaticMusicEvent>(StaticMusicEvent(e,pos));
		this->smelEvents.append(sme);
		std::sort(this->smelEvents.begin(), this->smelEvents.end(), [](StaticMusicEventPtr a, StaticMusicEventPtr b){return *a<*b;});
		emit(eventAdded(sme));
	}
	StaticMusicEventList &events() { return this->smelEvents; }

	void addSubClip(ClipContainerPtr clip) { clip->setParent(this); this->lChildClips.append(clip); this->bIsGroupClip=true; }

signals:
	void finished();
	void secondsPositionChanged(qreal);
	void volumeChanged(qint16);
	void mixedVolumeChanged(qint16);
	void eventAdded(StaticMusicEventPtr);
	void eventFired(MusicEvent*);

private:
	void setParent(ClipContainer *cc) { this->ccParent=cc; }
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
	qreal fPositionSeconds = 0.0f;
	Beat beatLength;

	QUrl urlFilePath;
	QByteArray baPCMData;

	QString sName;
	qreal fTempo = 0.0f;
	quint8 iBeatsPerMeasure = 0;
	quint8 iBeatUnit = 0;
	qint16 iVolume = 0;
	Beat beatTimelineOffset;

	ClipContainer *ccParent = NULL;
	QList<ClipContainerPtr> lChildClips;
	StaticMusicEventList smelEvents;
	bool bIsGroupClip = false;
};

#endif // CLIPCONTAINER_H
