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
	WaveformThread(ClipContainerPtr,float,float,float,int,int);

signals:
	void tileFinished(int,int,QPixmap);

protected:
	virtual void run();

private:
	QPoint intToPixel(qint32 val, int x, quint8 bytespersample, float zeropoint) {
		float pixelvalue;
		switch(bytespersample) {
		case 4:	pixelvalue = *(float*)&val; break;	// Interpret 32 bit values as floats
		case 3:	pixelvalue = (val/float(INT_MAX)); break;
		case 2:	pixelvalue = (val/float(SHRT_MAX)); break;
		case 1:	pixelvalue = (val/float(CHAR_MAX)); break;
		}
		pixelvalue *= zeropoint;
		return QPoint(x,zeropoint-roundf(pixelvalue));
	}

	ClipContainerPtr ccClip;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	float fScale = 0.0f;
	int iResolution = 0;
	int iTileCount = 0;
};

#endif // WAVEFORMTHREAD_H
