#include "Widgets/TimelineWidget.h"

TimelineWidget::TimelineWidget(ClipContainerPtr clip, bool readonly, QWidget *parent) : QGraphicsView(parent)
{
	this->gsTimeline = new QGraphicsScene(this);
	this->setScene(this->gsTimeline);

	this->setTopSpacing(TW_DEFAULT_TOP_SPACING);
	this->setMeasureSpacing(TW_DEFAULT_MEASURE_SPACING);
	this->setReadOnly(readonly);

	this->setClip(clip);

	this->setFrameShape(Shape::NoFrame);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	this->viewport()->setAutoFillBackground(false);

	this->createMeasureMarkers();

	this->pmiPlayMarker = new PlayMarkerItem(this->fTopSpacing);
	this->pmiPlayMarker->setTimelinePos(0.0f);
	this->pmiPlayMarker->setFlag(QGraphicsItem::ItemIgnoresTransformations);
	this->gsTimeline->addItem(this->pmiPlayMarker);

	this->setViewportBounds();
	this->setZoom(-1000.0f);

	connect(this->ccClip.get(), SIGNAL(secondsPositionChanged(qreal)), this, SLOT(movePlayMarkerToClipPos(qreal)));
}

TimelineWidget::~TimelineWidget()
{
	foreach(QList<InvertedLineItem*> list, this->mapMeasureLines) {
		foreach(InvertedLineItem *l, list) {
			delete l;
		}
	}
	foreach(EventMarkerItem *i, this->lEventMarkers)
		i->deleteLater();
	this->pmiPlayMarker->deleteLater();
	this->ctiClip->deleteLater();
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
	this->beatMouseClickPos = Beat::fromTimelinePosition(this->mapToScene(e->pos()).x(), this->fMeasureSpacing, this->ccClip->beatsPerMeasure(), this->ccClip->beatUnit(), this->iBeatUnitSnap);
	if(e->button()==Qt::LeftButton) {
		this->bClickMode = true;
		this->beatClipItemStart = this->ctiClip->timelineBeat();
	}
	QGraphicsView::mousePressEvent(e);
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *e)
{
	if(this->bClickMode && !this->bReadOnly) {
		this->beatMouseMovePos = Beat::fromTimelinePosition(this->mapToScene(e->pos()).x(), this->fMeasureSpacing, this->ccClip->beatsPerMeasure(), this->ccClip->beatUnit(), this->iBeatUnitSnap);
		if(abs((this->beatMouseClickPos-this->beatMouseMovePos).tick()) >= Beat::fromUnit(this->iBeatUnitSnap))
			this->bMoveMode = true;
		if(this->bMoveMode) {
			if(this->ctiClip->isSelected()) {
				Beat offset = this->beatClipItemStart+(this->beatMouseMovePos-this->beatMouseClickPos);
				this->ctiClip->setTimelinePos(offset, this->fMeasureSpacing);
				this->ccClip->setTimelineOffset(this->ctiClip->timelineBeat());
			}
			foreach(EventMarkerItem *emi, this->lEventMarkers) {
				if(emi->isSelected())
					emi->setTimelinePos(this->beatMouseMovePos-this->tiEventMarkerParent->timelineBeat(), this->fMeasureSpacing, this->ccClip->beatsPerMeasure(), this->ccClip->beatUnit());
			}
		}
	}
	QGraphicsView::mouseMoveEvent(e);
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *e)
{
	switch(e->button()) {
	case Qt::LeftButton:
		if(!this->bMoveMode && !this->bReadOnly)
			this->ccClip->setPositionSeconds(this->beatMouseClickPos.toSeconds(this->ccClip->tempo(), this->ccClip->beatUnit()));
		this->setViewportBounds();
		break;
	case Qt::RightButton:
		if(!this->bReadOnly) {
			MusicEventPtr event = std::make_shared<MusicEvent>(MusicEvent());
			if(this->ccClip->events().size()>0)
				event->addCommand(new JumpToMarkerCommand(this->ccClip->events().first(), this->ccClip->uuid()));
			this->ccClip->addEvent(event, this->beatMouseClickPos-this->tiEventMarkerParent->timelineBeat());
		}
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



void TimelineWidget::addEventMarker(StaticMusicEventPtr sme)
{
	EventMarkerItem *emi = new EventMarkerItem(sme, this->fTopSpacing);
	emi->setTimelinePos(sme->beat(), this->fMeasureSpacing, this->ccClip->beatsPerMeasure(), this->ccClip->beatUnit());
	emi->setFlag(QGraphicsItem::ItemIgnoresTransformations);
	if(!this->bReadOnly)
		emi->setFlag(QGraphicsItem::ItemIsSelectable);
	emi->setParentItem(this->tiEventMarkerParent);
	this->lEventMarkers.append(emi);
}

void TimelineWidget::moveEventMarkers(Beat &b)
{
	this->tiEventMarkerParent->setTimelinePos(b, this->fMeasureSpacing, this->ccClip->beatsPerMeasure(), this->ccClip->beatUnit());
}

void TimelineWidget::createMeasureMarkers()
{
	QPen pen(this->palette().foreground().color());
	pen.setWidth(0);
	InvertedLineItem *line = new InvertedLineItem(0.0f, this->fTopSpacing,
												  0.0f, this->fTopSpacing+TW_MEASURE_MARKER_LENGTH);
	this->mapMeasureLines[1].append(line);
	const quint8 beatspermeasure = this->ccClip->beatsPerMeasure();
	unsigned int measurecount = this->ccClip->beats().measureCount(beatspermeasure, this->ccClip->beatUnit());
	for(unsigned int measure=0; measure<measurecount; measure++) {
		for(int beat=1; beat<=beatspermeasure; beat++) {
			float spacing = this->fMeasureSpacing/beatspermeasure;
			float xpos = (this->fMeasureSpacing*measure) + (spacing*beat);
			float markerlength = (TW_MEASURE_MARKER_LENGTH*TW_BEAT_MARKER_DELTA);
			if(beat!=beatspermeasure) {
				line = new InvertedLineItem(xpos, this->fTopSpacing,
											xpos, this->fTopSpacing+markerlength);
				this->mapMeasureLines[4].append(line);
			}
			spacing /= 2.0f;
			this->createBeatMarkers(8, xpos-spacing, spacing, this->fScale, markerlength*TW_BEAT_MARKER_DELTA, pen);
		}
		line = new InvertedLineItem(this->fMeasureSpacing*(measure+1), this->fTopSpacing,
									this->fMeasureSpacing*(measure+1), this->fTopSpacing+TW_MEASURE_MARKER_LENGTH);
		this->mapMeasureLines[1].append(line);
	}
}
void TimelineWidget::createBeatMarkers(int unit, float pos, float spacing, float scale, float length, QPen &pen)
{
	if(unit>=128) return;
	InvertedLineItem *subbeatline = new InvertedLineItem(pos, this->fTopSpacing,
														 pos, this->fTopSpacing+length);
	this->mapMeasureLines[unit].append(subbeatline);
	spacing /= 2.0f;
	this->createBeatMarkers(unit*2, pos-spacing, spacing, scale, length*TW_BEAT_MARKER_DELTA, pen);
	this->createBeatMarkers(unit*2, pos+spacing, spacing, scale, length*TW_BEAT_MARKER_DELTA, pen);
}
void TimelineWidget::drawMeasureMarkers()
{
	QList<QGraphicsItem*> activelines = this->gsTimeline->items();
	if(!this->mapMeasureLines.contains(1)) return;
	if(!activelines.contains(this->mapMeasureLines[1].first())) {
		foreach(InvertedLineItem *measureline, this->mapMeasureLines[1])
			this->gsTimeline->addItem(measureline);
	}
	if(!this->mapMeasureLines.contains(4)) return;
	if(!activelines.contains(this->mapMeasureLines[4].first())) {
		foreach(InvertedLineItem *beatline, this->mapMeasureLines[4])
			this->gsTimeline->addItem(beatline);
	}

	float spacing = this->fMeasureSpacing/this->ccClip->beatsPerMeasure();
	int unit = 8;
	while(true) {
		if(unit>=128) break;
		if(!this->mapMeasureLines.contains(unit)) return;
		spacing /= 2.0f;
		if((spacing*this->fScale)<=TW_SUB_BEAT_MIN_SPACING) {
			if(activelines.contains(this->mapMeasureLines[unit].first())) {
				foreach(InvertedLineItem *beatline, this->mapMeasureLines[unit])
					this->gsTimeline->removeItem(beatline);
			}
		} else {
			if(!activelines.contains(this->mapMeasureLines[unit].first())) {
				foreach(InvertedLineItem *beatline, this->mapMeasureLines[unit])
					this->gsTimeline->addItem(beatline);
			}
		}
		unit*=2;
	}
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
	this->centerOn(this->pmiPlayMarker->x(),0.0f);
	this->fScale = scale;
	this->redrawStageElements();
}

void TimelineWidget::redrawStageElements()
{
	this->drawMeasureMarkers();
	if(this->ctiClip)
		this->ctiClip->setTimelineScale(this->fScale);
}

void TimelineWidget::setClip(ClipContainerPtr cc)
{
	this->ccClip=cc;

	this->ctiClip = new ClipTimelineItem(cc, this->fMeasureSpacing, this->fTopSpacing+(TW_MEASURE_MARKER_LENGTH*TW_BEAT_MARKER_DELTA));
	this->ctiClip->setFlag(QGraphicsItem::ItemIgnoresTransformations);
	if(!this->bReadOnly)
		this->ctiClip->setFlag(QGraphicsItem::ItemIsSelectable);
	this->gsTimeline->addItem(this->ctiClip);

	this->tiEventMarkerParent = new TimelineItem(this->fTopSpacing);
	this->tiEventMarkerParent->setTimelinePos(this->ctiClip->timelineBeat(), this->fMeasureSpacing, cc->beatsPerMeasure(), cc->beatUnit());
	connect(this->ctiClip, SIGNAL(moved(Beat&)), this, SLOT(moveEventMarkers(Beat&)));
	if(!this->bReadOnly) {
		StaticMusicEventList &events = cc->events();
		foreach(StaticMusicEventPtr e, events)
			this->addEventMarker(e);
		connect(cc.get(), SIGNAL(eventAdded(StaticMusicEventPtr)), this, SLOT(addEventMarker(StaticMusicEventPtr)));
	}
	this->gsTimeline->addItem(this->tiEventMarkerParent);
}

void TimelineWidget::setViewportBounds()
{
	const quint8 beatspermeasure = this->ccClip->beatsPerMeasure();
	const quint8 beatunit = this->ccClip->beatUnit();
	Beat startposition = this->ccClip->timelineOffset();
	int endposition = (this->ccClip->beats()+this->ccClip->timelineOffset()).measureCount(beatspermeasure, beatunit) * this->fMeasureSpacing;
	endposition -= startposition.toTimelinePosition(this->fMeasureSpacing, beatspermeasure, beatunit);
	if(startposition>0)
		startposition.setTick(0);
	this->gsTimeline->setSceneRect(startposition.toTimelinePosition(this->fMeasureSpacing, beatspermeasure, beatunit), 0,
								   endposition, 100);
}



void TimelineWidget::movePlayMarkerToClipPos(qreal secs)
{
	Beat pos = Beat::fromSeconds(secs, this->ccClip->tempo(), this->ccClip->beatUnit());
	pos += this->ccClip->timelineOffset();
	this->pmiPlayMarker->setTimelinePos(pos.toTimelinePosition(this->fMeasureSpacing, this->ccClip->beatsPerMeasure(), this->ccClip->beatUnit()));
	this->centerOn(this->pmiPlayMarker->x(),0.0f);
}
