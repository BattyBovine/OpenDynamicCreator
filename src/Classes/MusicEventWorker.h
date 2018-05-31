#ifndef MUSICEVENTWORKER_H
#define MUSICEVENTWORKER_H

#include <QThread>
#include <QMutex>
#include <QBuffer>

#include "Beat.h"
#include "StaticMusicEvent.h"

class MusicEventWorker : public QThread
{
	Q_OBJECT
public:
	MusicEventWorker(QBuffer *buf, QObject *parent=Q_NULLPTR) : QThread(parent) { this->bufferData=buf; }
public slots:
	void run() override {
		if(!this->bufferData || !this->bufferData->isOpen()) {
			emit(invalidBuffer());
			return;
		}
		while(this->bufferData->isOpen() && !this->bStopped) {
			if((quint64)this->bufferData->pos() >= this->iPosTarget) {
				this->iPosTarget = UINT64_MAX;
				emit(musicEvent(this->meEvent));
			}
		}
	}
	void setNextEvent(MusicEvent *me, quint64 pos) {
		QMutexLocker(&this->mutexLock);
		this->meEvent = me;
		this->iPosTarget = pos;
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
	QMutex mutexLock;
	QBuffer *bufferData = NULL;
	MusicEvent *meEvent = NULL;
	quint64 iPosTarget = UINT64_MAX;
	bool bStopped = false;
};

#endif // MUSICEVENTWORKER_H
