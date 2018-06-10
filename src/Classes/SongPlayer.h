#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include <QDebug>

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
		SP_INVALID_TRACK,
		SP_INVALID_DEVICE,
		SP_INVALID_ACTIVE_SEGMENT
	};

	explicit SongPlayer(QObject *parent=Q_NULLPTR) : QObject(parent){}
	~SongPlayer();

	Error buildSong(BaseMusicItem*);

public slots:
	Error playSong(QUuid &startclip=QUuid());
	void pauseSong();
	void stopSong();
	void applyEvent(MusicEvent*);

private slots:
	void setVolume(qreal);

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
