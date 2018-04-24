#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include "Widgets/MusicTreeView.h"

#define TW_BEAT_MARKER_LENGTH	3.0f
#define TW_MEASURE_MARKER_MULT	2.0f

class TimelineWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TimelineWidget(QWidget *parent=0, BaseMusicItem *musicitem=NULL, int beatsPerMeasure=4);

	BaseMusicItem *musicItem() { return this->bmiMusicItem; }
	int beatsPerMeasure() { return this->iBeatsPerMeasure; }
	float measureSpacing() { return this->fMeasureSpacing; }
	float beatSpacing() { return this->fBeatSpacing; }

	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);

public slots:
	void setMusicItem(BaseMusicItem *i) { this->bmiMusicItem=i; }
	void setBeatsPerMeasure(int b) { this->iBeatsPerMeasure=b; }
	void setMeasureSpacing(float m) { this->fMeasureSpacing=m; this->fBeatSpacing=(m/this->iBeatsPerMeasure); }
	void setBeatSpacing(float b) { this->fBeatSpacing=b; this->fMeasureSpacing=(b*this->iBeatsPerMeasure); }

protected:
	void paintEvent(QPaintEvent*);

private:
	void drawPlayMarker(QPainter&, float);
	void drawMeasureMarkers(QPainter&, int);

	inline int getNearestBeat(float);

	BaseMusicItem *bmiMusicItem = NULL;

	int iBeatsPerMeasure = 4;
	float fTopSpacing = 5.0f;
	float fMeasureSpacing = 40.0f;
	float fBeatSpacing = 10.0f;

	int iCursorPosition = 0;
	bool bSnapToNote = true;
};

#endif // TIMELINEWIDGET_H
