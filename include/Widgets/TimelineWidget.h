#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "Widgets/MusicTreeView.h"

#define TW_BEAT_MARKER_LENGTH	3.0f
#define TW_MEASURE_MARKER_MULT	2.0f

class TimelineWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TimelineWidget(TrackItem *track=NULL, ClipItem *clip=NULL, QWidget *parent=0);

	TrackItem *track() { return this->tiTrack; }
	ClipItem *clip() { return this->ciClip; }
	float beatSpacing() { return this->fBeatSpacing; }

public slots:
	void setTrack(TrackItem *i) { this->tiTrack=i; }
	void setClip(ClipItem *i) { this->ciClip=i; }
	void setBeatSpacing(float b) { this->fBeatSpacing=b; }

protected:
	void paintEvent(QPaintEvent*);

private:
	void drawMeasures(QPainter&, int, float);

	TrackItem *tiTrack = NULL;
	ClipItem *ciClip = NULL;

	float fBeatSpacing = 20.0f;
};

#endif // TIMELINEWIDGET_H
