#include "Widgets/GraphicsItems/TimelineWidget.h"

TimelineWidget::TimelineWidget(ClipItem *clip, float tempo, int beatspermeasure, int beatunit, QAction *playpause, QAction *stop, bool readonly, QWidget *parent) : QGraphicsView(parent)
{
	this->setTempo(tempo);
	this->setBeatsPerMeasure(beatspermeasure);
	this->setBeatUnit(beatunit);
	this->setTopSpacing(5.0f);
	this->setMeasureSpacing(TW_DEFAULT_MEASURE_SPACING);
	this->setReadOnly(readonly);

	this->setFrameShape(Shape::NoFrame);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	this->viewport()->setAutoFillBackground(false);

	this->ccClip = clip->clipContainer();

	this->gsTimeline = new QGraphicsScene(this);
	this->gsTimeline->setSceneRect(0,0,this->ccClip.beats().toTimelinePosition(this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit),100);
	this->setScene(this->gsTimeline);

	if(clip) {
		this->ctiClip = new ClipTimelineItem(this->fTopSpacing);
		this->ctiClip->setTimelinePos(Beat(), this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit);
		this->ctiClip->setLength(this->ccClip.beats(), this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit);
		this->ctiClip->setFlag(QGraphicsItem::ItemIgnoresTransformations);
		this->gsTimeline->addItem(this->ctiClip);
	}

	this->pmiPlayMarker = new PlayMarkerItem(this->fTopSpacing);
	this->pmiPlayMarker->setTimelinePos(0.0f);
	this->pmiPlayMarker->setFlag(QGraphicsItem::ItemIgnoresTransformations);
	this->gsTimeline->addItem(this->pmiPlayMarker);

	this->setZoom(-1000.0f);

	connect(playpause, SIGNAL(toggled(bool)), this, SLOT(togglePlayPause(bool)));
	connect(stop, SIGNAL(triggered(bool)), this, SLOT(clipStop()));
	connect(&this->timerPlayMarker, SIGNAL(timeout()), this, SLOT(movePlayMarkerToClipPos()));
}



void TimelineWidget::resizeEvent(QResizeEvent *e)
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	this->setAlignment(Qt::AlignTop|Qt::AlignLeft);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->ctiClip->setHeight(this->height()-this->horizontalScrollBar()->height());
	this->redrawStageElements();
	QGraphicsView::resizeEvent(e);
}

void TimelineWidget::mousePressEvent(QMouseEvent *e)
{
	if(e->button()==Qt::LeftButton) {
		this->bClickMode = true;
		this->beatMouseClickPos = Beat::fromTimelinePosition(this->mapToScene(e->pos()).x(), this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit, this->iBeatUnitSnap);
		this->beatClipItemStart = this->ctiClip->timelineBeat();
	}
	QGraphicsView::mousePressEvent(e);
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *e)
{
	if(this->bClickMode && !this->bReadOnly) {
		this->beatMouseMovePos = Beat::fromTimelinePosition(this->mapToScene(e->pos()).x(), this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit, this->iBeatUnitSnap);
		if(abs((this->beatMouseClickPos-this->beatMouseMovePos).beat()) >= Beat::fromUnit(this->iBeatUnitSnap))
			this->bMoveMode = true;
		if(this->bMoveMode)
			this->ctiClip->setTimelinePos(this->beatClipItemStart+(this->beatMouseMovePos-this->beatMouseClickPos), this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit);
	}
	QGraphicsView::mouseMoveEvent(e);
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *e)
{
	switch(e->button()) {
	case Qt::LeftButton:
		if(!this->bMoveMode)
			this->pmiPlayMarker->setTimelinePos(this->beatMouseClickPos, this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit);
		break;
	case Qt::RightButton:
//		this->ciClipItem->addEvent(MusicEvent(this->beatMouseClickPos));
		break;
	}
	this->bClickMode = this->bMoveMode = false;
	QGraphicsView::mouseReleaseEvent(e);
}

void TimelineWidget::wheelEvent(QWheelEvent *e)
{
	this->setTransformationAnchor(QGraphicsView::NoAnchor);
	this->setResizeAnchor(QGraphicsView::NoAnchor);

	float zoomdelta = 0.0f;
	int scroll = e->pixelDelta().y();
	if(scroll==0)	zoomdelta = (e->angleDelta().y()/2400.0f);
	else			zoomdelta = (scroll/20.0f);
	this->setZoom(zoomdelta);
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

void TimelineWidget::drawMeasureMarkers()
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
	unsigned int measurecount = this->ccClip.beats().measureCount(this->iBeatsPerMeasure, this->iBeatUnit);
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
			if((spacing*this->fScale)<=TW_SUB_BEAT_MIN_SPACING)
				continue;
			this->drawBeatMarkers(xpos-spacing, spacing, this->fScale, markerlength*TW_BEAT_MARKER_DELTA, pen);
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
	if(play) {
		this->timerPlayMarker.start(16);
		this->ccClip.play();
	} else {
		this->timerPlayMarker.stop();
		this->ccClip.pause();
	}
}

void TimelineWidget::clipStop()
{
	this->timerPlayMarker.stop();
	this->ccClip.stop();
}



void TimelineWidget::setZoom(float zoomdelta)
{
	QTransform transform = this->transform();
	float scale = transform.m11()+zoomdelta;
	scale = std::max(TW_MIN_SCALE, std::min(scale, TW_MAX_SCALE));
	transform.setMatrix(scale,				transform.m12(),	transform.m13(),
						transform.m21(),	transform.m22(),	transform.m23(),
						transform.m31(),	transform.m32(),	transform.m33());
	this->setTransform(transform);
	if(this->pmiPlayMarker)
		this->centerOn(this->pmiPlayMarker->x(),0.0f);
	else
		this->centerOn(0.0f,0.0f);
	this->fScale = scale;
	this->redrawStageElements();
}

void TimelineWidget::redrawStageElements()
{
	this->drawMeasureMarkers();
	if(this->ctiClip) {
		this->ctiClip->setTimelineScale(this->fScale);
		this->ctiClip->generateWaveform(this->ccClip);
	}
}



void TimelineWidget::movePlayMarkerToClipPos()
{
	if(!this->ccClip.isPlaying())
		return;
	Beat pos = Beat::fromSeconds(this->ccClip.secondsElapsed(), this->fTempo, this->iBeatUnit);
	this->pmiPlayMarker->setTimelinePos(pos.toTimelinePosition(this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit));
}
