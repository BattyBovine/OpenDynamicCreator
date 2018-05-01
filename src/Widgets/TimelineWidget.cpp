#include "Widgets/TimelineWidget.h"

TimelineWidget::TimelineWidget(BaseMusicItem *musicitem, float tempo, int beatspermeasure, int beatunit, bool readonly, QWidget *parent) : QWidget(parent)
{
	this->setMusicItem(musicitem);
	this->setTempo(tempo);
	this->setBeatsPerMeasure(beatspermeasure);
	this->setBeatUnit(beatunit);
	this->setTopSpacing(5.0f);
	this->setMeasureSpacing(TW_DEFAULT_MEASURE_SPACING);
	this->setReadOnly(readonly);
}

void TimelineWidget::mousePressEvent(QMouseEvent *e)
{
	this->beatMouseClickPos = Beat::fromTimelinePosition(e->pos().x(), this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit, this->iBeatUnitSnap);
	QWidget::mousePressEvent(e);
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *e)
{
	if(!this->bReadOnly) {
		this->beatMouseMovePos = Beat::fromTimelinePosition(e->pos().x(), this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit, this->iBeatUnitSnap);
		if(abs((this->beatMouseClickPos-this->beatMouseMovePos).beat()) >= Beat::quarterNote())
			this->bMoveMode = true;
	}
	QWidget::mouseMoveEvent(e);
	this->repaint();
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
	QWidget::mouseReleaseEvent(e);
	this->repaint();
}

void TimelineWidget::wheelEvent(QWheelEvent *e)
{
	float newspacing = 0;
	int scroll = e->pixelDelta().y();
	if(scroll==0)	newspacing = (this->fMeasureSpacing+(e->angleDelta().y()/60.0f));
	else			newspacing = (this->fMeasureSpacing+scroll);
	this->setMeasureSpacing(std::max(TW_MIN_MEASURE_SPACING, std::min(newspacing, TW_MAX_MEASURE_SPACING)));
	this->repaint();
	emit(zoomChanged(this->fMeasureSpacing));
}

void TimelineWidget::paintEvent(QPaintEvent*)
{
	QPainter painter(this);

	this->drawClip(painter);

	if(this->bMoveMode) {
		painter.setPen(QColor(0, 0, 255));
		painter.setBrush(QColor(0, 0, 255, 64));
		painter.drawRect(QRectF(
						QPointF(this->beatMouseClickPos.toTimelinePosition(this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit), this->fTopSpacing),
						QPointF(this->beatMouseMovePos.toTimelinePosition(this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit), this->height()-1)
						));
	}

	this->drawEventMarkers(painter);
	this->drawPlayMarker(painter);

	painter.setPen(this->palette().foreground().color());
	painter.drawLine(QLineF(	// Beat 0 marker
						 QPointF(0.0f, this->fTopSpacing),
						 QPointF(0.0f, this->fTopSpacing+(TW_BEAT_MARKER_LENGTH*TW_MEASURE_MARKER_MULT)))
					 );
	this->drawMeasureMarkers(painter);
}



void TimelineWidget::drawClip(QPainter &p)
{
	ClipItem *clip = static_cast<ClipItem*>(this->bmiMusicItem);
	if(!clip) return;
	float pos = this->secondsToPos(clip->seconds());
	p.setPen(QColor(0, 0, 255));
	p.drawLine(QPointF(pos, this->fTopSpacing), QPointF(pos, this->height()));
}

void TimelineWidget::drawPlayMarker(QPainter &p)
{
	float pos = this->beatPlayMarker.toTimelinePosition(this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit);
	p.setPen(Qt::NoPen);
	QBrush playbrush(QColor(0, 255, 0));
	p.setBrush(playbrush);
	p.drawRect(QRectF(QPointF(pos-(this->fTopSpacing/2.0f), 0.0f), QPointF(pos+(this->fTopSpacing/2.0f), this->fTopSpacing)));
	p.setPen(QPen(playbrush, 1.0f));
	p.drawLine(QPointF(pos, this->fTopSpacing), QPointF(pos, this->height()));
}

void TimelineWidget::drawEventMarkers(QPainter &p)
{
	p.setBrush(Qt::NoBrush);
	p.setPen(QColor(255, 0, 0));
	foreach(MusicEvent event, this->bmiMusicItem->events()) {
		float pos = event.beat().toTimelinePosition(this->fMeasureSpacing, this->iBeatsPerMeasure, this->iBeatUnit);
		QVector<QPointF> polyline;
		polyline << QPointF(pos, 0.0f)
				 << QPointF(pos-(this->fTopSpacing/2.0f), (this->fTopSpacing/2.0f))
				 << QPointF(pos, this->fTopSpacing)
				 << QPointF(pos+(this->fTopSpacing/2.0f), (this->fTopSpacing/2.0f))
				 << QPointF(pos, 0.0f);
		p.drawPolyline(QPolygonF(polyline));
		p.drawLine(QPointF(pos, this->fTopSpacing), QPointF(pos, this->height()));
	}
}

void TimelineWidget::drawMeasureMarkers(QPainter &p)
{
	p.setBrush(Qt::NoBrush);
//	if(count<0)
//		p.setPen(QColor(255, 0, 0));
//	else
		p.setPen(this->palette().foreground().color());
	unsigned int measurecount = static_cast<ClipItem*>(this->bmiMusicItem)->length().measureCount();
	for(unsigned int measure=0; measure<measurecount; measure++) {
		for(int beat=1; beat<=this->iBeatsPerMeasure; beat++) {
			float spacing = this->fMeasureSpacing/this->iBeatsPerMeasure;
			float xpos = (this->fMeasureSpacing*measure) + (spacing*beat);
			if(beat!=this->iBeatsPerMeasure) {
				p.drawLine(	QPointF(xpos, this->fTopSpacing),
							QPointF(xpos, this->fTopSpacing+TW_BEAT_MARKER_LENGTH)
							);
			}
			spacing /= 2.0f;
			if(spacing<=TW_SUB_BEAT_SPACING)
				continue;
			this->drawBeatMarkers(xpos-spacing, spacing, TW_BEAT_MARKER_LENGTH*TW_BEAT_MARKER_MULT, p);
			if(xpos>this->width())
				break;
		}
		p.drawLine(	QPointF(this->fMeasureSpacing*(measure+1), this->fTopSpacing),
					QPointF(this->fMeasureSpacing*(measure+1), this->fTopSpacing+(TW_BEAT_MARKER_LENGTH*TW_MEASURE_MARKER_MULT))
					);
	}
}
void TimelineWidget::drawBeatMarkers(float pos, float spacing, float length, QPainter &p)
{
	p.drawLine(	QPointF(pos, this->fTopSpacing),
				QPointF(pos, this->fTopSpacing+length)
				);
	spacing /= 2.0f;
	if(spacing<TW_SUB_BEAT_SPACING)
		return;
	this->drawBeatMarkers(pos-spacing, spacing, length*TW_BEAT_MARKER_MULT, p);
	this->drawBeatMarkers(pos+spacing, spacing, length*TW_BEAT_MARKER_MULT, p);
}
