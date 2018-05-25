#ifndef MUSICEVENT_H
#define MUSICEVENT_H

#include <memory>

#include <QObject>
#include <QUuid>
#include <QBuffer>
#include <QAudio>

#include "Beat.h"

class ClipContainer;


class EventCommand
{
public:
	EventCommand(Beat transitionlength=Beat()) { this->beatTransitionLength=transitionlength; }
	virtual void applyEvent(ClipContainer*){}
protected:
	Beat beatTransitionLength;
};
class JumpToPositionCommand : public EventCommand
{
public:
	JumpToPositionCommand(Beat pos) { this->setPosition(pos); }
	void setPosition(Beat pos) { this->beatToPosition=pos; }
	virtual void applyEvent(ClipContainer*);
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
	virtual void applyEvent(ClipContainer*);
private:
	qreal fVolume = 0.0f;
};


class MusicEvent : public QObject
{
	Q_OBJECT
public:
	MusicEvent(Beat &b=Beat()) { this->setBeat(b); this->uuid=QUuid::createUuid(); }
	~MusicEvent();

	void setBeat(Beat &b) { this->beatPos=b; }
	void setName(QString &n) { this->sName=n; }
	void setActive(bool a) { this->bActive=a; }
	void addCommand(EventCommand *e) { this->lCommands.append(e); }

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


class MusicEventWorker : public QObject
{
	Q_OBJECT
public:
	MusicEventWorker(QBuffer *buf){ this->bufferData=buf; }
	void setEventTime(quint64 ms, int samplerate, quint8 bytespersample, quint8 channels) { this->setEventTime(ms/1000.0f,samplerate,bytespersample,channels); }
	void setEventTime(float secs, int samplerate, quint8 bytespersample, quint8 channels) { this->iPosition = roundf(secs * samplerate) * (bytespersample*channels); }
public slots:
	void start() {
		if(!this->bufferData || !this->bufferData->isOpen()) {
			emit(invalidBuffer());
			return;
		}
		while(this->bufferData->isOpen() && this->bufferData->pos()<this->iPosition)
			continue;
		emit(musicEvent());
	}
	void kill() { emit(finished()); }
signals:
	void musicEvent();
	void invalidBuffer();
	void finished();
private:
	QBuffer *bufferData = NULL;
	qint64 iPosition = 0;
};

#endif // MUSICEVENT_H
