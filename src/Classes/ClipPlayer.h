#ifndef CLIPPLAYER_H
#define CLIPPLAYER_H

#include <QDebug>

#include <QObject>
#include <QUuid>
#include <QAudioOutput>
#include <QBuffer>
#include <QHash>

#include "Classes/MusicEventWorker.h"


class SongPlayer;

typedef QHash<QUuid,QAudioOutput*> PlayerHash;
typedef QHash<QUuid,QBuffer*> BufferHash;
typedef QHash<QUuid,ClipContainer*> ClipHash;
class ClipPlayer : public QObject
{
	Q_OBJECT
public:
	ClipPlayer(ClipContainer *parentclip, QObject *parent=NULL);
	~ClipPlayer();
	bool addClipData(ClipContainer*);
	bool configureEventWorker(ClipContainer*);

public slots:
	void play(const SongPlayer*);
	void pause() { foreach(QAudioOutput *player, this->hashAudioPlayers) player->suspend(); }
	void stop();
	void updateVolume();

private slots:
	void setNextEvent();
	void stopEventThread();
	void playerState(QAudio::State);
	void setClipPositions();

signals:
	void finished();

private:
	void startEventThread();

	PlayerHash hashAudioPlayers;
	BufferHash hashDataBuffers;
	ClipHash hashClips;

	QUuid uuidMainClip;
	MusicEventWorker *mewEvents = NULL;
	qreal fGlobalVolume = 1.0f;

	QTimer timerPlayerMarkerPosition;
	StaticMusicEventList::ConstIterator smeNextEvent = NULL;
};

#endif // CLIPPLAYER_H
