#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include <memory>

#include <QObject>
#include <QUuid>
#include <QSettings>

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
	void addNewClip(BaseMusicItem*);

	QMap<QUuid, ClipContainerPtr> mapClips;
	QUuid uuidActiveClip;
};

#endif // SONGPLAYER_H
