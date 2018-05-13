#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <memory>

#include <QGraphicsView>
#include <QOpenGLWidget>
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
#define TW_DEFAULT_TOP_SPACING		5.0f


class TimelineWidget : public QGraphicsView
{
	Q_OBJECT
public:
	explicit TimelineWidget(std::shared_ptr<ClipContainer>, QAction*, QAction*, bool readonly=true, QWidget *parent=0);
	float measureSpacing() { return this->fMeasureSpacing; }

public slots:
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
	void redrawStageElements();

	void drawEventMarkers();
	void drawMeasureMarkers();
	void drawBeatMarkers(float, float, float, float, QPen&);

	float posToSeconds(float pos) const { return pos/(this->fMeasureSpacing/this->ccClip->beatsPerMeasure())*(60.0f/this->ccClip->tempo()); }
	float secondsToPos(float secs) const { return secs*(this->ccClip->tempo()/60.0f)*(this->fMeasureSpacing/this->ccClip->beatsPerMeasure()); }

	QGraphicsScene *gsTimeline = NULL;
	std::shared_ptr<ClipContainer> ccClip;
	PlayMarkerItem *pmiPlayMarker = NULL;
	ClipTimelineItem *ctiClip = NULL;
	QList<QGraphicsLineItem*> lMeasureLines;

	float fScale = 1.0f;
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