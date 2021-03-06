#include "ClipPlayer.h"
#include "SongPlayer.h"
#include "ClipContainer.h"



ClipPlayer::ClipPlayer(ClipContainer *clip, QObject *parent) : QObject(parent)
{
	this->uuidMainClip = clip->uuid();
	this->hashClips[this->uuidMainClip] = clip;
	connect(clip, SIGNAL(mixedVolumeChanged(qint16)), this, SLOT(updateVolume()));
	connect(&this->timerPlayerMarkerPosition, SIGNAL(timeout()), this, SLOT(setClipPositions()));
}

ClipPlayer::~ClipPlayer()
{
	foreach(QAudioOutput *player, this->hashAudioPlayers)
		player->deleteLater();
	foreach(QBuffer *buffer, this->hashDataBuffers)
		buffer->deleteLater();
}



bool ClipPlayer::addClipData(ClipContainer *cc)
{
	QUuid &uuid = cc->uuid();
	this->hashClips[uuid] = cc;
	QAudioOutput *player = cc->createPlayer();
	if(!player)
		return false;
	this->hashAudioPlayers[uuid] = player;
	connect(player, SIGNAL(stateChanged(QAudio::State)), this, SLOT(playerState(QAudio::State)));
	connect(cc, SIGNAL(mixedVolumeChanged(qint16)), this, SLOT(updateVolume()));
	QByteArray *ba = cc->pcmData();
	QBuffer *buffer = new QBuffer(ba);
	buffer->open(QIODevice::ReadOnly);
	this->hashDataBuffers[uuid] = buffer;
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



void ClipPlayer::play(const SongPlayer *sp)
{
	this->timerPlayerMarkerPosition.start(1);
	this->startEventThread();
	connect(this->mewEvents, SIGNAL(musicEvent(MusicEvent*)), sp, SLOT(applyEvent(MusicEvent*)));
	BufferHash::Iterator bufferiter = this->hashDataBuffers.begin();
	foreach(QAudioOutput *player, this->hashAudioPlayers) {
		player->start(*bufferiter);
		bufferiter++;
	}
}

void ClipPlayer::stop()
{
	this->stopEventThread();
	foreach(QAudioOutput *player, this->hashAudioPlayers)
		player->stop();
	this->timerPlayerMarkerPosition.stop();
}

void ClipPlayer::updateVolume()
{
	ClipContainer *cc = static_cast<ClipContainer*>(QObject::sender());
	QUuid uuid = cc->uuid();
	if(this->hashAudioPlayers.contains(uuid)) {
		this->hashAudioPlayers[uuid]->setVolume(this->hashClips[uuid]->mixedVolumePercent());
	} else {
		if(this->uuidMainClip!=uuid)
			return;
		QList<QUuid> players = this->hashAudioPlayers.keys();
		foreach(QUuid player, players)
			this->hashAudioPlayers[player]->setVolume(this->hashClips[player]->mixedVolumePercent());
	}
}



void ClipPlayer::startEventThread()
{
	if(!this->mewEvents) {
		Beat smallestoffset(INT_MAX);
		QList<QUuid> &buffers = this->hashDataBuffers.keys();
		QUuid clipwithsmallestoffset;
		foreach(QUuid clipid, buffers) {
			Beat clipoffset = this->hashClips[clipid]->timelineOffset();
			if(clipoffset < smallestoffset) {
				smallestoffset = clipoffset;
				clipwithsmallestoffset = clipid;
			}
		}
		this->mewEvents = new MusicEventWorker(this->hashClips[this->uuidMainClip], this->hashDataBuffers[clipwithsmallestoffset]);
		connect(this->mewEvents, SIGNAL(musicEvent(MusicEvent*)), this, SLOT(setNextEvent()));
		connect(this->mewEvents, SIGNAL(finished()), this, SLOT(stopEventThread()));
		this->mewEvents->start(QThread::LowestPriority);
	}
	this->smeNextEvent = this->hashClips[this->uuidMainClip]->events().cbegin();
	this->setNextEvent();
}
void ClipPlayer::setNextEvent()
{
	if(this->smeNextEvent==this->hashClips[this->uuidMainClip]->events().cend())
		return;
	this->mewEvents->setNextEvent((*this->smeNextEvent).get());
	this->smeNextEvent++;
}
void ClipPlayer::stopEventThread()
{
	if(this->mewEvents) {
		if(this->mewEvents->isRunning())
			this->mewEvents->stop();
		this->mewEvents->deleteLater();
		this->mewEvents = NULL;
	}
}



void ClipPlayer::setClipPositions()
{
	QList<QUuid> &playingclips = this->hashDataBuffers.keys();
	foreach(QUuid clipid, playingclips) {
		ClipContainer *cc = this->hashClips[clipid];
		cc->setPositionSeconds(samples2secs(this->hashDataBuffers[clipid]->pos(),
											cc->sampleRate(),
											cc->bytesPerSample(),
											cc->channelCount()));
	}
}
