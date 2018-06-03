#include "ClipPlayer.h"


ClipPlayer::~ClipPlayer()
{
	foreach(QAudioOutput *player, this->lAudioPlayers)
		player->deleteLater();
}

bool ClipPlayer::getAudioData(ClipContainer *clip)
{
	QAudioOutput *player = clip->createPlayer();
	if(!player)
		return false;
	this->lAudioPlayers.append(player);
	connect(player, SIGNAL(stateChanged(QAudio::State)), this, SLOT(playerState(QAudio::State)));
	QByteArray *ba = clip->pcmData();
	this->lDataBuffers.append(new QBuffer(ba));
	this->lEventThreads.append(new MusicEventWorker(ba));
	this->lAudioPlayers.last()->setVolume(clip->volume());
	return true;
}

void ClipPlayer::playerState(QAudio::State s)
{
	QAudioOutput *sender = (QAudioOutput*)QObject::sender();
	switch(s) {
//	case QAudio::ActiveState:
//		this->bIsPlaying = true;
//		break;
	case QAudio::StoppedState:
		if(sender->error()!=QAudio::NoError)
			qDebug() << sender->error();
		break;
	case QAudio::IdleState:
		emit(finished());
		break;
	}
}
