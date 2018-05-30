#ifndef MUSICEVENTWORKER_H
#define MUSICEVENTWORKER_H

#include <QThread>
#include <QMutex>
#include <QBuffer>

#include "Beat.h"
#include "ClipContainer.h"
#include "StaticMusicEvent.h"

class MusicEventWorker : public QThread
{
	Q_OBJECT
public:
	MusicEventWorker(QObject *parent=Q_NULLPTR) : QThread(parent){}
	void setBuffer(QBuffer *buf) { this->bufferData=buf; }
	void setClip(ClipContainer *cc) { this->ccClip=cc; }
	void setEventList(StaticMusicEventList &events) {
		QMutexLocker(&this->mutexLock);
		this->smelEvents = events;
		this->smeNextEvent = this->smelEvents.begin();
	}
public slots:
	void run() override {
		if(!this->bufferData || !this->bufferData->isOpen()) {
			emit(invalidBuffer());
			return;
		}
		if(!this->getNextTarget())
			return;
		while(this->bufferData->isOpen() && !this->bStopped) {
			if((quint64)this->bufferData->pos() >= this->iPosTarget) {
				emit(musicEvent(this->smeNextEvent->get()->musicEvent().get()));
				this->smeNextEvent++;
				if(!this->getNextTarget())
					return;
			}
		}
	}
	void stop() {
		QMutexLocker(&this->mutexLock);
		this->bStopped = true;
	}
signals:
	void musicEvent(MusicEvent*);
	void invalidBuffer();
	void stopped();
private:
	bool getNextTarget();

	QMutex mutexLock;
	QBuffer *bufferData = NULL;
	ClipContainer *ccClip = NULL;
	StaticMusicEventList smelEvents;
	StaticMusicEventList::Iterator smeNextEvent;
	quint64 iPosTarget = 0;
	bool bStopped = false;
};

#endif // MUSICEVENTWORKER_H
