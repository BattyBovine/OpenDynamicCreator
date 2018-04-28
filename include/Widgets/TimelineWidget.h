#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include "Widgets/MusicTreeView.h"

#define TW_BEAT_MARKER_LENGTH		3.0f
#define TW_MEASURE_MARKER_MULT		2.0f

#define TW_MIN_MEASURE_SPACING		8.0f
#define TW_DEFAULT_MEASURE_SPACING	40.0f
#define TW_MAX_MEASURE_SPACING		480.0f

class TimelineWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TimelineWidget(BaseMusicItem*, float, int, int, bool, QWidget *parent=0);

	BaseMusicItem *musicItem() { return this->bmiMusicItem; }
	float tempo() { return this->fTempo; }
	int beatsPerMeasure() { return this->iBeatsPerMeasure; }
	int beatUnit() { return this->iBeatUnit; }
	float measureSpacing() { return this->fMeasureSpacing; }

public slots:
	void setMusicItem(BaseMusicItem *i) { this->bmiMusicItem=i; }
	void setTempo(float t) { this->fTempo=t; }
	void setBeatsPerMeasure(int b) { this->iBeatsPerMeasure=b; }
	void setBeatUnit(int b) { this->iBeatUnit=b; }
	void setMeasureSpacing(float m) { this->fMeasureSpacing=m; }

	void setReadOnly(bool r) { this->bReadOnly=r; }
	void setBeatUnitSnap(int s) { this->iBeatUnitSnap=s; }
	void setBeatUnitSnapFromCombo(int s) { this->iBeatUnitSnap=powf(2,s); }

protected:
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
	virtual void wheelEvent(QWheelEvent*);
	virtual void paintEvent(QPaintEvent*);

private:
	void setTopSpacing(float t) { this->fTopSpacing=t; }

	void drawClip(QPainter&);
	void drawPlayMarker(QPainter&);
	void drawEventMarkers(QPainter&);
	void drawMeasureMarkers(QPainter&);

	float posToSeconds(float pos) const { return (pos/(this->fMeasureSpacing/this->iBeatsPerMeasure)) / (this->fTempo/60.0f); }
	float secondsToPos(float secs) const { return (secs*(this->fMeasureSpacing/this->iBeatsPerMeasure)) * (this->fTempo/60.0f); }
	float beatToPos(Beat beat) const { return this->secondsToPos(Beat::toSeconds(beat, (this->iBeatUnitSnap/float(this->iBeatUnit)))); }
	Beat posToBeat(float pos) const { return Beat::fromSeconds(this->posToSeconds(pos), this->fTempo, this->iBeatsPerMeasure, this->iBeatUnitSnap, (this->iBeatUnitSnap/float(this->iBeatUnit))); }

	BaseMusicItem *bmiMusicItem = NULL;

	float fTempo;
	int iBeatsPerMeasure;
	int iBeatUnit;
	float fTopSpacing = 0.0f;
	float fMeasureSpacing = 0.0f;

	Beat beatPlayMarker;
	Beat beatMouseClickPos, beatMouseMovePos;
	quint8 iBeatUnitSnap = 4;
	bool bReadOnly = false;
	bool bMoveMode = false;
};

#endif // TIMELINEWIDGET_H
