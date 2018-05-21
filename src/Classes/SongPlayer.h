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

signals:
	void finished();

private:
	Error searchItemChildren(BaseMusicItem*);
	void addNewClip(BaseMusicItem*);

	QMap<QUuid, std::shared_ptr<ClipContainer> > mapClips;
	QUuid uuidActiveClip;
};

#endif // SONGPLAYER_H
