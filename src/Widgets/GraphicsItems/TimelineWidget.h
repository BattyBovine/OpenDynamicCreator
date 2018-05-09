#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QGraphicsView>
#include <QScrollBar>
#include <QAction>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>

#include "Widgets/GraphicsItems/PlayMarkerItem.h"
#include "Widgets/GraphicsItems/ClipTimelineItem.h"
#include "Widgets/MusicTreeView.h"
#include "Classes/MusicEvent.h"

#define TW_MEASURE_MARKER_LENGTH	6.0f
#define TW_BEAT_MARKER_DELTA		0.75f
#define TW_SUB_BEAT_MIN_SPACING		6.0f

#define TW_MIN_SCALE				0.2f
#define TW_DEFAULT_MEASURE_SPACING	40.0f
#define TW_MAX_SCALE				20.0f


class TimelineWidget : public QGraphicsView
{
	Q_OBJECT
public:
	explicit TimelineWidget(ClipItem*, float, int, int, QAction*, QAction*, bool readonly=true, QWidget *parent=0);

	float tempo() { return this->fTempo; }
	int beatsPerMeasure() { return this->iBeatsPerMeasure; }
	int beatUnit() { return this->iBeatUnit; }
	float measureSpacing() { return this->fMeasureSpacing; }

public slots:
	void setTempo(float t) { this->fTempo=t; }
	void setBeatsPerMeasure(int b) { this->iBeatsPerMeasure=b; }
	void setBeatUnit(int b) { this->iBeatUnit=b; }
	void setMeasureSpacing(float m) { this->fMeasureSpacing=m; }
	void setZoom(float);

	void setReadOnly(bool r) { this->bReadOnly=r; }
	void setBeatUnitSnapFromCombo(int s) { this->iBeatUnitSnap=powf(2,s); }

	void togglePlayPause(bool);
	void clipStop();

	void movePlayMarkerToClipPos();

signals:
	void zoomChanged(float);

protected:
	virtual void resizeEvent(QResizeEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
	virtual void wheelEvent(QWheelEvent*);

private:
	void setTopSpacing(float t) { this->fTopSpacing=t; }
	void redrawStageElements(float scale=1.0f);

	void drawEventMarkers();
	void drawMeasureMarkers(float);
	void drawBeatMarkers(float, float, float, float, QPen&);

	float posToSeconds(float pos) const { return pos/(this->fMeasureSpacing/this->iBeatsPerMeasure)*(60.0f/this->fTempo); }
	float secondsToPos(float secs) const { return secs*(this->fTempo/60.0f)*(this->fMeasureSpacing/this->iBeatsPerMeasure); }

	QGraphicsScene *gsTimeline = NULL;
	ClipContainer ccClip;
	PlayMarkerItem *pmiPlayMarker = NULL;
	ClipTimelineItem *ctiClip = NULL;
	QList<QGraphicsLineItem*> lMeasureLines;

	float fTempo = 120.0f;
	int iBeatsPerMeasure = 4;
	int iBeatUnit = 4;
	float fTopSpacing = 0.0f;
	float fMeasureSpacing = 0.0f;
	bool bReadOnly = false;

	Beat beatPlayMarker;
	Beat beatMouseClickPos, beatMouseMovePos;
	Beat beatClipItemStart;
	quint8 iBeatUnitSnap = 4;
	bool bClickMode = false;
	bool bMoveMode = false;

	QTimer timerPlayMarker;
};

#endif // TIMELINEWIDGET_H
