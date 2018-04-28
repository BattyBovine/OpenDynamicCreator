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
	this->beatMouseClickPos = this->posToBeat(e->pos().x());
	QWidget::mousePressEvent(e);
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *e)
{
	if(!this->bReadOnly) {
		this->beatMouseMovePos = this->posToBeat(e->pos().x());
//		if(abs(this->beatMouseClick.beat()-this->beatMouseMove.beat())>=2)
//			this->bMoveMode = true;
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
}

void TimelineWidget::paintEvent(QPaintEvent*)
{
	QPainter painter(this);

	if(this->bMoveMode) {
		painter.setPen(QColor(0, 0, 255));
		painter.setBrush(QColor(0, 0, 255, 64));
		painter.drawRect(QRectF(
						QPointF(this->beatToPos(this->beatMouseClickPos), this->fTopSpacing),
						QPointF(this->beatToPos(this->beatMouseMovePos), this->height()-1)
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
	p.setPen(QColor(255, 255, 0));
	p.drawLine(QPointF(pos, this->fTopSpacing), QPointF(pos, this->height()));
}

void TimelineWidget::drawPlayMarker(QPainter &p)
{
	float pos = this->beatToPos(this->beatPlayMarker);
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
		float pos = this->beatToPos(event.beat());
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
	unsigned int measurecount = static_cast<ClipItem*>(this->bmiMusicItem)->measureCount();
	for(unsigned int measure=0; measure<measurecount; measure++) {
		for(int beat=1; beat<=this->iBeatsPerMeasure; beat++) {
			float beatspacing = this->fMeasureSpacing/this->iBeatsPerMeasure;
			float xpos = (this->fMeasureSpacing*measure) + (beatspacing*beat);
//			if(count<0)	xpos = -xpos;
			p.drawLine(	QPointF(xpos, this->fTopSpacing),
						QPointF(xpos, this->fTopSpacing+(TW_BEAT_MARKER_LENGTH*((beat==this->iBeatsPerMeasure)?TW_MEASURE_MARKER_MULT:1.0f)))
						);
		}
	}
}
