#ifndef MUSICEVENTWORKER_H
#define MUSICEVENTWORKER_H

#include <QDebug>

#include <QThread>
#include <QMutex>
#include <QBuffer>

#include "Beat.h"
#include "StaticMusicEvent.h"
#include "ClipContainer.h"

#define samples2secs(s,sr,bps,ch)	(s/bps/float(ch))/sr
#define secs2samples(sec,sr,bps,ch)	(sr*bps*ch)*sec


class MusicEventWorker : public QThread
{
	Q_OBJECT
public:
	MusicEventWorker(ClipContainer *cc, QBuffer *buf, QObject *parent=Q_NULLPTR) : QThread(parent)
	{
		this->ccParent = cc;
		this->bufferClip = buf;
		this->iOffsetPos = roundf(secs2samples(cc->timelineOffset().toSeconds(cc->tempo(),cc->beatUnit()),
											   cc->sampleRate(),
											   cc->bytesPerSample(),
											   cc->channelCount()));
		this->iBeatLength = secs2samples(Beat::fromUnit(cc->beatUnit()).toSeconds(cc->tempo(),cc->beatUnit()),
										 cc->sampleRate(),
										 cc->bytesPerSample(),
										 cc->channelCount());
		this->iMeasureLength = this->iBeatLength * cc->beatsPerMeasure();
	}
public slots:
	void run() override {
		if(!this->bufferClip || !this->bufferClip->isOpen()) {
			emit(invalidBuffer());
			return;
		}
		while(this->bufferClip->isOpen() && !this->bStopped) {
			const quint64 bufferpos = (quint64)this->bufferClip->pos();
			if(bufferpos >= this->iPosTarget) {
				emit(musicEvent(this->meEvent));
				this->iPosTarget = UINT64_MAX;
			}
			const qint64 bufferoffsetpos = (bufferpos+this->iOffsetPos);
			const int beatcount = floorf(bufferoffsetpos / float(this->iBeatLength));
			if(beatcount>=0 && this->iBeatValue!=beatcount) {
				this->iBeatValue = beatcount;
				emit(beat(beatcount));
			}
			const int measurecount = floorf(bufferoffsetpos / float(this->iMeasureLength));
			if(measurecount>=0 && this->iMeasureValue!=measurecount) {
				qDebug() << QString::number(measurecount);
				this->iMeasureValue = measurecount;
				emit(measure(measurecount));
			}
		}
	}
	void setNextEvent(StaticMusicEvent *sme) {
		QMutexLocker(&this->mutexLock);
		this->meEvent = sme->musicEvent().get();
		this->iPosTarget = roundf(secs2samples(sme->beat().toSeconds(this->ccParent->tempo(),this->ccParent->beatUnit()),
											   this->ccParent->sampleRate(),
											   this->ccParent->bytesPerSample(),
											   this->ccParent->channelCount()));
	}
	void stop() {
		QMutexLocker(&this->mutexLock);
		this->bStopped = true;
	}
signals:
	void musicEvent(MusicEvent*);
	void beat(quint16);
	void measure(quint16);
	void invalidBuffer();
	void stopped();
private:
	ClipContainer *ccParent = NULL;
	QBuffer *bufferClip = NULL;
	qint64 iOffsetPos = UINT64_MAX;
	quint32 iBeatLength = 0xFFFFFFFF;
	quint32 iMeasureLength = 0xFFFFFFFF;
	quint16 iBeatValue = -1;
	quint16 iMeasureValue = -1;

	QMutex mutexLock;
	MusicEvent *meEvent = NULL;
	quint64 iPosTarget = UINT64_MAX;
	bool bStopped = false;
};

#endif // MUSICEVENTWORKER_H
