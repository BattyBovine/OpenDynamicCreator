#ifndef CLIPPLAYER_H
#define CLIPPLAYER_H

#include <QObject>
#include <QUuid>
#include <QAudioOutput>
#include <QBuffer>
#include <QHash>

#include "Classes/MusicEventWorker.h"


typedef QHash<QUuid,QAudioOutput*> PlayerHash;
typedef QHash<QUuid,QBuffer*> BufferHash;
typedef QHash<QUuid,QByteArray*> ByteArrayHash;
typedef QHash<QUuid,MusicEventWorker*> EventWorkerHash;
class ClipPlayer : public QObject
{
	Q_OBJECT
public:
	~ClipPlayer();
	bool addAudioData(ClipContainer*);
public slots:
	void play() {
		BufferHash::Iterator bufferiter = this->hashDataBuffers.begin();
		foreach(QAudioOutput *player, this->hashAudioPlayers) {
			player->start(*bufferiter);
			bufferiter++;
		}
	}
	void pause() { foreach(QAudioOutput *player, this->hashAudioPlayers) player->suspend(); }
	void stop() { foreach(QAudioOutput *player, this->hashAudioPlayers) player->stop(); }
private slots:
	void playerState(QAudio::State);
signals:
	void finished();
private:
	PlayerHash hashAudioPlayers;
	BufferHash hashDataBuffers;
	EventWorkerHash hashEventThreads;

	qreal fGlobalVolume = 1.0f;
};

#endif // CLIPPLAYER_H
