#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include <memory>

#include <QObject>
#include <QUuid>
#include <QSettings>

#include "PreferencesDialog.h"
#include "ClipContainer.h"
#include "Widgets/MusicTreeView.h"


class SongPlayer : public QObject
{
	Q_OBJECT
public:
	enum Error {
		SP_OK,
		SP_NO_CLIPS,
		SP_INVALID_DEVICE
	};

	explicit SongPlayer(QObject *parent=Q_NULLPTR) : QObject(parent) {}

	Error buildSong(BaseMusicItem*);

public slots:
	Error playSong();
	void pauseSong();
	void stopSong();

private slots:
	void playerState(QAudio::State);
	void setVolume(qreal);

signals:
	void finished();

private:
	Error searchItemChildren(BaseMusicItem*);
	void addNewClip(BaseMusicItem*);
	Error configurePlayer(QUuid);

	QMap<QUuid, std::shared_ptr<ClipContainer> > mapClips;
	QMap<QUuid, QAudioOutput*> mapAudioPlayers;
	QUuid uuidActiveClip;
};

#endif // SONGPLAYER_H
