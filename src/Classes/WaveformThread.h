#ifndef WAVEFORMTHREAD_H
#define WAVEFORMTHREAD_H

#include <QDebug>

#include <memory>

#include <QThread>
#include <QPixmap>
#include <QPainter>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QCoreApplication>

#include "PreferencesDialog.h"
#include "ClipContainer.h"

#define WT_MAX_TILE_LENGTH			256.0f
#define WT_RESOLUTION_MULTIPLIER	2

class WaveformThread : public QThread
{
	Q_OBJECT
public:
	WaveformThread(std::shared_ptr<ClipContainer>,float,float,float,int,int);

signals:
	void tileFinished(int,int,QPixmap);

protected:
	virtual void run();

private:
	std::shared_ptr<ClipContainer> ccClip;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	float fScale = 0.0f;
	int iResolution = 0;
	int iTileCount = 0;
};

#endif // WAVEFORMTHREAD_H
