#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include <memory>

#include <QObject>
#include <QUuid>

#include "ClipContainer.h"
#include "Widgets/MusicTreeView.h"


class SongPlayer : public QObject
{
	Q_OBJECT
public:
	enum Error {
		SP_OK,
		SP_NOT_TRACK
	};

	explicit SongPlayer(QObject *parent=Q_NULLPTR) : QObject(parent) {}

	int buildSong(TrackItem*);

public slots:
	void playSong();
	void pauseSong();
	void stopSong();

private:
	void searchItemChildren(BaseMusicItem*);
	void configurePlayer(QUuid);

	QMap<QUuid, std::shared_ptr<ClipContainer> > mapClips;
	QAudioOutput *aoPlayer = NULL;
	QUuid uuidActiveClip;
	bool bIsPlaying = false;
};

#endif // SONGPLAYER_H
