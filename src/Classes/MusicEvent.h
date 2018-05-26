#ifndef MUSICEVENT_H
#define MUSICEVENT_H

#include <memory>

#include <QObject>
#include <QThread>
#include <QUuid>
#include <QBuffer>
#include <QAudio>

#include "Beat.h"

class ClipContainer;
class MusicEvent;


class EventCommand
{
	friend class MusicEvent;
public:
	EventCommand(Beat transitionlength=Beat(), MusicEvent *parent=NULL) { this->beatTransitionLength=transitionlength; this->setParent(parent); }
	virtual void applyEvent(ClipContainer*){}
protected:
	void setParent(MusicEvent *p) { this->meParent=p; }
	Beat parentPosition();
	MusicEvent *meParent;
	Beat beatTransitionLength;
};
class JumpBackCommand : public EventCommand
{
public:
	JumpBackCommand(Beat pos) { this->setPosition(pos); }
	void setPosition(Beat pos) { this->beatToPosition=pos; }
	void applyEvent(ClipContainer*) override;
private:
	Beat beatToPosition;
};
class ChangeVolumeCommand : public EventCommand
{
public:
	ChangeVolumeCommand(Beat transitionlength=Beat()) : EventCommand(transitionlength){}
	ChangeVolumeCommand(qreal v, Beat transitionlength=Beat()) : EventCommand(transitionlength) { this->setVolume(v); }
	ChangeVolumeCommand(int d, Beat transitionlength=Beat()) : EventCommand(transitionlength) { this->setVolume(QAudio::convertVolume(d, QAudio::DecibelVolumeScale, QAudio::LogarithmicVolumeScale)); }
	void setVolume(qreal v) { this->fVolume=v; }
	void applyEvent(ClipContainer*) override;
private:
	qreal fVolume = 0.0f;
};


class MusicEvent : public QObject
{
	Q_OBJECT
public:
	MusicEvent(Beat &b=Beat()) { this->setPosition(b); this->uuid=QUuid::createUuid(); }
	~MusicEvent();

	void setPosition(Beat &b) { this->beatPos=b; }
	void setName(QString &n) { this->sName=n; }
	void setActive(bool a) { this->bActive=a; }
	void addCommand(EventCommand *e) { e->setParent(this); this->lCommands.append(e); }

	Beat beat() const { return this->beatPos; }
	QString name() const { return this->sName; }
	bool active() const { return this->bActive; }
	const QList<EventCommand*> &commands() { return this->lCommands; }

	bool operator==(const MusicEvent &e) const	{ return (beatPos==e.beatPos); }
	bool operator!=(const MusicEvent &e) const	{ return !(*this==e); }
	bool operator<(const MusicEvent &e) const	{ return (beatPos<e.beatPos); }
	bool operator<=(const MusicEvent &e) const	{ return (beatPos<=e.beatPos); }
	bool operator>(const MusicEvent &e) const	{ return (beatPos>e.beatPos); }
	bool operator>=(const MusicEvent &e) const	{ return (beatPos>=e.beatPos); }

private:
	QUuid uuid;
	Beat beatPos;
	QString sName;
	bool bActive = true;
	QList<EventCommand*> lCommands;
};
typedef QVector<MusicEvent*> MusicEventList;


class MusicEventWorker : public QThread
{
	Q_OBJECT
public:
	MusicEventWorker(QObject *parent=Q_NULLPTR) : QThread(parent){}
	void setBuffer(QBuffer *buf) { this->bufferData=buf; }
	void setEventTime(quint64 ms, int samplerate, quint8 bytespersample, quint8 channels) { this->setEventTime(ms/1000.0f,samplerate,bytespersample,channels); }
	void setEventTime(float secs, int samplerate, quint8 bytespersample, quint8 channels) { this->iPosTarget = roundf(secs * samplerate) * (bytespersample*channels); }
public slots:
	void run() override {
		if(!this->bufferData || !this->bufferData->isOpen()) {
			emit(invalidBuffer());
			return;
		}
		while(this->bufferData->isOpen() &&
			  !this->bStopped &&
			  ((quint64)this->bufferData->pos() < this->iPosTarget)) {
			continue;
		}
		if(!this->bStopped)
			emit(musicEvent());
	}
	void stop() {
		this->bStopped = true;
	}
signals:
	void musicEvent();
	void invalidBuffer();
	void stopped();
private:
	QBuffer *bufferData = NULL;
	quint64 iPosTarget = 0;
	bool bStopped = false;
};

#endif // MUSICEVENT_H
