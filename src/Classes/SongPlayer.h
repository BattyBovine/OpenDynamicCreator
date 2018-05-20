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

	int buildSong(BaseMusicItem*);

public slots:
	void playSong();
	void pauseSong();
	void stopSong();

private:
	int searchItemChildren(BaseMusicItem*);
	void addNewClip(BaseMusicItem*);
	void configurePlayer(QUuid);

	QMap<QUuid, std::shared_ptr<ClipContainer> > mapClips;
	QAudioOutput *aoPlayer = NULL;
	QUuid uuidActiveClip;
};

#endif // SONGPLAYER_H
