#include "Widgets/GraphicsItems/TimelineWidget.h"

TimelineWidget::TimelineWidget(BaseMusicItem *musicitem, float tempo, int beatspermeasure, int beatunit, QAction *playpause, QAction *stop, bool readonly, QWidget *parent) : QGraphicsView(parent)
{
	this->setMusicItem(musicitem);
	this->setTempo(tempo);
	this->setBeatsPerMeasure(beatspermeasure);
	this->setBeatUnit(beatunit);
	this->setTopSpacing(5.0f);
	this->setMeasureSpacing(TW_DEFAULT_MEASURE_SPACING);
	this->setReadOnly(readonly);

	this->setFrameShape(Shape::NoFrame);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->viewport()->setAutoFillBackground(false);

	this->gsTimeline = new QGraphicsScene(this);
	this->gsTimeline->setSceneRect(0,0,this->bmiMusicItem->length().toTimelinePosition(this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit),100);
	this->setScene(this->gsTimeline);
	this->redrawStageElements();

	this->pmiPlayMarker = new PlayMarkerItem(this->fTopSpacing);
	this->pmiPlayMarker->setTimelinePos(0.0f);
	this->pmiPlayMarker->setFlag(QGraphicsItem::ItemIgnoresTransformations);
	this->gsTimeline->addItem(this->pmiPlayMarker);

	if(musicitem) {
		this->ctiClip = new ClipTimelineItem(this->fTopSpacing);
		this->ctiClip->setTimelinePos(Beat().toTimelinePosition(this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit));
		this->ctiClip->setLength(this->secondsToPos(musicitem->seconds()));
		this->gsTimeline->addItem(this->ctiClip);
	}

	connect(playpause, SIGNAL(toggled(bool)), this, SLOT(togglePlayPause(bool)));
	connect(stop, SIGNAL(triggered(bool)), this, SLOT(clipStop()));
}



void TimelineWidget::resizeEvent(QResizeEvent *e)
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	this->setAlignment(Qt::AlignTop|Qt::AlignLeft);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->ctiClip->setHeight(100.0f);
	QGraphicsView::resizeEvent(e);
}

void TimelineWidget::mousePressEvent(QMouseEvent *e)
{
	this->beatMouseClickPos = Beat::fromTimelinePosition(this->mapToScene(e->pos()).x(), this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit, this->iBeatUnitSnap);
	QWidget::mousePressEvent(e);
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *e)
{
	if(!this->bReadOnly) {
		this->beatMouseMovePos = Beat::fromTimelinePosition(this->mapToScene(e->pos()).x(), this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit, this->iBeatUnitSnap);
		if(abs((this->beatMouseClickPos-this->beatMouseMovePos).beat()) >= Beat::quarterNote())
			this->bMoveMode = true;
	}
	QWidget::mouseMoveEvent(e);
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *e)
{
	switch(e->button()) {
	case Qt::LeftButton:
		if(!this->bMoveMode)
			this->beatPlayMarker = this->beatMouseClickPos;
		break;
	case Qt::RightButton:
		this->bmiMusicItem->addEvent(MusicEvent(this->beatMouseClickPos));
		break;
	}
	this->bMoveMode = false;
	this->pmiPlayMarker->setTimelinePos(this->beatPlayMarker.toTimelinePosition(this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit));
	QWidget::mouseReleaseEvent(e);
}

void TimelineWidget::wheelEvent(QWheelEvent *e)
{
	float scalefactor = 0;
	int scroll = e->pixelDelta().y();
	if(scroll==0)	scalefactor = (e->angleDelta().y()/2400.0f);
	else			scalefactor = (scroll/20.0f);

	QTransform transform = this->transform();
	float scale = transform.m11()+scalefactor;
	scale = std::max(TW_MIN_SCALE, std::min(scale, TW_MAX_SCALE));
	transform.setMatrix(scale,				transform.m12(),	transform.m13(),
						transform.m21(),	transform.m22(),	transform.m23(),
						transform.m31(),	transform.m32(),	transform.m33());
	this->setTransform(transform);
	this->redrawStageElements(scale);
}



void TimelineWidget::drawClip()
{
}

void TimelineWidget::drawEventMarkers()
{
//	p.setBrush(Qt::NoBrush);
//	p.setPen(QColor(255, 0, 0));
//	foreach(MusicEvent event, this->bmiMusicItem->events()) {
//		float pos = event.beat().toTimelinePosition(this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit);
//		QVector<QPointF> polyline;
//		polyline << QPointF(pos, 0.0f)
//				 << QPointF(pos-(this->fTopSpacing/2.0f), (this->fTopSpacing/2.0f))
//				 << QPointF(pos, this->fTopSpacing)
//				 << QPointF(pos+(this->fTopSpacing/2.0f), (this->fTopSpacing/2.0f))
//				 << QPointF(pos, 0.0f);
//		p.drawPolyline(QPolygonF(polyline));
//		p.drawLine(QPointF(pos, this->fTopSpacing), QPointF(pos, this->height()));
//	}
}

void TimelineWidget::drawMeasureMarkers(float scale)
{
	foreach(QGraphicsLineItem *line, this->lMeasureLines) {
		this->gsTimeline->removeItem(line);
		delete line;
	}
	this->lMeasureLines.clear();

	QPen pen(this->palette().foreground().color());
	pen.setWidth(0);
	this->lMeasureLines.append(this->gsTimeline->addLine(0.0f, this->fTopSpacing,
														 0.0f, this->fTopSpacing+TW_MEASURE_MARKER_LENGTH,
														 pen));
	unsigned int measurecount = static_cast<ClipItem*>(this->bmiMusicItem)->length().measureCount();
	for(unsigned int measure=0; measure<measurecount; measure++) {
		for(int beat=1; beat<=this->iBeatsPerMeasure; beat++) {
			float spacing = this->fMeasureSpacing/this->iBeatsPerMeasure;
			float xpos = (this->fMeasureSpacing*measure) + (spacing*beat);
			float markerlength = (TW_MEASURE_MARKER_LENGTH*TW_BEAT_MARKER_DELTA);
			if(beat!=this->iBeatsPerMeasure) {
				this->lMeasureLines.append(this->gsTimeline->addLine(xpos, this->fTopSpacing,
																	 xpos, this->fTopSpacing+markerlength,
																	 pen));
			}
			spacing /= 2.0f;
			if((spacing*scale)<=TW_SUB_BEAT_MIN_SPACING)
				continue;
			this->drawBeatMarkers(xpos-spacing, spacing, scale, markerlength*TW_BEAT_MARKER_DELTA, pen);
		}
		this->lMeasureLines.append(this->gsTimeline->addLine(this->fMeasureSpacing*(measure+1), this->fTopSpacing,
															 this->fMeasureSpacing*(measure+1), this->fTopSpacing+TW_MEASURE_MARKER_LENGTH,
															 pen));
	}
}
void TimelineWidget::drawBeatMarkers(float pos, float spacing, float scale, float length, QPen &pen)
{
	this->lMeasureLines.append(this->gsTimeline->addLine(pos, this->fTopSpacing,
														 pos, this->fTopSpacing+length,
														 pen));
	spacing /= 2.0f;
	if((spacing*scale)<TW_SUB_BEAT_MIN_SPACING)
		return;
	this->drawBeatMarkers(pos-spacing, spacing, scale, length*TW_BEAT_MARKER_DELTA, pen);
	this->drawBeatMarkers(pos+spacing, spacing, scale, length*TW_BEAT_MARKER_DELTA, pen);
}



void TimelineWidget::togglePlayPause(bool play)
{
	if(play)	this->bmiMusicItem->play();
	else		this->bmiMusicItem->pause();
}

void TimelineWidget::clipStop()
{
	this->bmiMusicItem->stop();
}



void TimelineWidget::redrawStageElements(float scale)
{
	this->drawMeasureMarkers(scale);
}
