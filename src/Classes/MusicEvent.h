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
	virtual void applyEvent(ClipContainer*,Beat){}
protected:
	void setParent(MusicEvent *p) { this->meParent=p; }
	MusicEvent *meParent;
	Beat beatTransitionLength;
};
class JumpBackCommand : public EventCommand
{
public:
	JumpBackCommand(Beat pos) { this->setPosition(pos); }
	void setPosition(Beat pos) { this->beatToPosition=pos; }
	void applyEvent(ClipContainer*,Beat) override;
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
	void applyEvent(ClipContainer*,Beat) override;
private:
	qreal fVolume = 0.0f;
};


class MusicEvent
{
public:
	MusicEvent() { this->uuid=QUuid::createUuid(); }
	~MusicEvent();

	void setName(QString &n) { this->sName=n; }
	void setActive(bool a) { this->bActive=a; }
	void addCommand(EventCommand *e) { e->setParent(this); this->lCommands.append(e); }

	QString name() const { return this->sName; }
	bool active() const { return this->bActive; }
	const QList<EventCommand*> &commands() { return this->lCommands; }

private:
	QUuid uuid;
	QString sName;
	bool bActive = true;
	QList<EventCommand*> lCommands;
};
typedef std::shared_ptr<MusicEvent> MusicEventPtr;

class StaticMusicEvent
{
public:
	StaticMusicEvent(){}
	StaticMusicEvent(MusicEventPtr me, Beat pos=Beat()) { this->setEvent(me); this->setBeat(pos); }
	void setEvent(MusicEventPtr me) { this->meEvent=me; }
	void setBeat(Beat pos) { this->beatPos=pos; }

	MusicEventPtr musicEvent() { return this->meEvent; }
	Beat beat() { return this->beatPos; }

	bool operator==(const StaticMusicEvent &e) const	{ return (beatPos==e.beatPos); }
	bool operator!=(const StaticMusicEvent &e) const	{ return !(*this==e); }
	bool operator<(const StaticMusicEvent &e) const		{ return (beatPos<e.beatPos); }
	bool operator<=(const StaticMusicEvent &e) const	{ return (beatPos<=e.beatPos); }
	bool operator>(const StaticMusicEvent &e) const		{ return (beatPos>e.beatPos); }
	bool operator>=(const StaticMusicEvent &e) const	{ return (beatPos>=e.beatPos); }
private:
	MusicEventPtr meEvent = NULL;
	Beat beatPos;
};
typedef std::shared_ptr<StaticMusicEvent> StaticMusicEventPtr;
typedef QVector<StaticMusicEventPtr> StaticMusicEventList;


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
