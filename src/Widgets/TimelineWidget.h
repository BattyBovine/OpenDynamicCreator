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

#include "GraphicsItems/PlayMarkerItem.h"
#include "GraphicsItems/EventMarkerItem.h"
#include "GraphicsItems/ClipTimelineItem.h"
#include "GraphicsItems/InvertedLineItem.h"
#include "MusicTreeView.h"
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
	explicit TimelineWidget(ClipContainerPtr, bool readonly=true, QWidget *parent=0);
	~TimelineWidget();
	ClipContainerPtr clip() { return this->ccClip; }
	float measureSpacing() { return this->fMeasureSpacing; }

public slots:
	void setClip(ClipContainerPtr c);
	void setTopSpacing(float t) { this->fTopSpacing=t; }
	void setMeasureSpacing(float m) { this->fMeasureSpacing=m; }
	void setViewportBounds();
	void setZoom(float);

	void setReadOnly(bool r) { this->bReadOnly=r; }
	void setBeatUnitSnapFromCombo(int s) { this->iBeatUnitSnap=powf(2,s); }
	void movePlayMarkerToClipPos(qreal);

signals:
	void zoomChanged(float);

protected:
	virtual void resizeEvent(QResizeEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
	virtual void wheelEvent(QWheelEvent*);

private slots:
	void addEventMarker(StaticMusicEventPtr);
	void moveEventMarkers(Beat&);

private:
	void redrawStageElements();

	void createMeasureMarkers();
	void createBeatMarkers(int, float, float, float, float, QPen&);
	void drawMeasureMarkers();

	float posToSeconds(float pos) const { return pos/(this->fMeasureSpacing/this->ccClip->beatsPerMeasure())*(60.0f/this->ccClip->tempo()); }
	float secondsToPos(float secs) const { return secs*(this->ccClip->tempo()/60.0f)*(this->fMeasureSpacing/this->ccClip->beatsPerMeasure()); }

	QGraphicsScene *gsTimeline = NULL;
	ClipContainerPtr ccClip;
	PlayMarkerItem *pmiPlayMarker = NULL;
	ClipTimelineItem *ctiClip = NULL;
	TimelineItem *tiEventMarkerParent;
	QList<EventMarkerItem*> lEventMarkers;
	QMap<unsigned int,QList<InvertedLineItem*> > mapMeasureLines;

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
};

#endif // TIMELINEWIDGET_H
