#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include <memory>

#include <QObject>
#include <QUuid>
#include <QSettings>
#include <QQueue>

#include "PreferencesDialog.h"
#include "ClipContainer.h"
#include "Classes/ClipPlayer.h"
#include "Widgets/MusicTreeView.h"


class SongPlayer : public QObject
{
	Q_OBJECT
public:
	enum Error {
		SP_OK,
		SP_NO_CLIPS,
		SP_INVALID_DEVICE,
		SP_INVALID_ACTIVE_CLIP
	};

	explicit SongPlayer(QObject *parent=Q_NULLPTR) : QObject(parent) {}

	Error buildSong(BaseMusicItem*);

public slots:
	Error playSong();
	void pauseSong();
	void stopSong();
	void applyEvent(MusicEvent*);

signals:
	void finished();

private:
	Error searchItemChildren(BaseMusicItem*);
	ClipPlayer* createClipPlayer(QUuid&);

	QHash<QUuid,ClipContainer*> hashClips;
	QMultiHash<QUuid,QUuid> hashGroupMap;

	ClipPlayer *cpActiveSegment = NULL;
	ClipPlayer *cpTransitionSegment = NULL;
	QQueue<ClipPlayer*> queueSegments;
};

#endif // SONGPLAYER_H
