#include "ClipPlayer.h"
#include "ClipContainer.h"


ClipPlayer::~ClipPlayer()
{
	foreach(MusicEventWorker *mew, this->hashEventThreads)
		mew->stop();
	foreach(QAudioOutput *player, this->hashAudioPlayers)
		player->deleteLater();
	foreach(QBuffer *buffer, this->hashDataBuffers)
		buffer->deleteLater();
}

bool ClipPlayer::addAudioData(ClipContainer *clip)
{
	QUuid &uuid = clip->uuid();
	QAudioOutput *player = clip->createPlayer();
	if(!player)
		return false;
	this->hashAudioPlayers[uuid] = player;
	connect(player, SIGNAL(stateChanged(QAudio::State)), this, SLOT(playerState(QAudio::State)));
	QByteArray *ba = clip->pcmData();
	this->hashDataBuffers[uuid] = new QBuffer(ba);
	this->hashEventThreads[uuid] = new MusicEventWorker(this->hashDataBuffers[uuid]);
	return true;
}

void ClipPlayer::playerState(QAudio::State s)
{
//	QAudioOutput *sender = (QAudioOutput*)QObject::sender();
	switch(s) {
//	case QAudio::ActiveState:
//		this->bIsPlaying = true;
//		break;
	case QAudio::StoppedState:
//		if(sender->error()!=QAudio::NoError)
//			qDebug() << sender->error();
		break;
	case QAudio::IdleState:
		emit(finished());
		break;
	}
}
