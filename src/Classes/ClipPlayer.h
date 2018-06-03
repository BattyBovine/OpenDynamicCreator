#ifndef CLIPPLAYER_H
#define CLIPPLAYER_H

#include <QObject>
#include <QAudioOutput>
#include <QBuffer>

#include "Classes/MusicEventWorker.h"


typedef QHash<QUuid,QAudioOutput*> PlayerHash;
typedef QHash<QUuid,QBuffer*> BufferHash;
typedef QHash<QUuid,MusicEventWorker*> EventWorkerHash;
class ClipPlayer : public QObject
{
	Q_OBJECT
public:
	~ClipPlayer();
	bool getAudioData(QUuid);
public slots:
	void play() {
		BufferList::Iterator bufferiter = this->lDataBuffers.begin();
		foreach(QAudioOutput *player, this->lAudioPlayers) {
			player->start(*bufferiter);
			bufferiter++;
		}
	}
	void pause() { foreach(QAudioOutput *player, this->lAudioPlayers) player->suspend(); }
	void stop() { foreach(QAudioOutput *player, this->lAudioPlayers) player->stop(); }
private slots:
	void playerState(QAudio::State);
private:
	PlayerHash hashAudioPlayers;
	BufferHash hashDataBuffers;
	EventWorkerHash hashEventThreads;

	qreal fGlobalVolume = 1.0f;
};

#endif // CLIPPLAYER_H
