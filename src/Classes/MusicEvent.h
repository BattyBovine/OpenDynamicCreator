#ifndef MUSICEVENT_H
#define MUSICEVENT_H

#include <memory>

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QVector>
#include <QUuid>
#include <QBuffer>
#include <QAudio>

#include "EventCommand.h"

class MusicEvent
{
public:
	MusicEvent() { this->uuid=QUuid::createUuid(); }
	~MusicEvent();

	void setName(QString &n) { this->sName=n; }
	void setActive(bool a) { this->bActive=a; }
	void addCommand(EventCommand *e);

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

#endif // MUSICEVENT_H
