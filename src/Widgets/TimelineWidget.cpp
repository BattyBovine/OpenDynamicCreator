#include "Widgets/TimelineWidget.h"

TimelineWidget::TimelineWidget(QWidget *parent, BaseMusicItem *musicitem, int beatspermeasure) : QWidget(parent)
{
	this->setMusicItem(musicitem);
	this->setMeasureSpacing(40.0f);
	this->setBeatsPerMeasure(beatspermeasure);
}

void TimelineWidget::mousePressEvent(QMouseEvent *e)
{
//	switch(e->button()) {
//	case Qt::LeftButton:
//	case Qt::RightButton:
//		this->fCursorPosition = e->pos().x();
//		break;
//	}
	QWidget::mousePressEvent(e);
//	this->repaint();
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *e)
{
//	switch(e->button()) {
//	case Qt::LeftButton:
//	case Qt::RightButton:
//		this->iCursorPosition = e->pos().x();
//		break;
//	}
	QWidget::mouseMoveEvent(e);
//	this->repaint();
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *e)
{
	switch(e->button()) {
	case Qt::LeftButton:
	case Qt::RightButton:
		if(this->bSnapToNote)
			this->iCursorPosition = this->getNearestBeat(e->pos().x()) * this->measureSpacing()/this->iBeatsPerMeasure;
		else
			this->iCursorPosition = e->pos().x();
		break;
	}
	QWidget::mouseReleaseEvent(e);
	this->repaint();
}

void TimelineWidget::paintEvent(QPaintEvent*)
{
	QPainter painter(this);

	this->drawPlayMarker(painter, this->iCursorPosition);

	painter.setPen(this->palette().foreground().color());
	painter.drawLine(QLineF(	// Beat 0 marker
						 QPointF(0.0f, this->fTopSpacing),
						 QPointF(0.0f, this->fTopSpacing+(TW_BEAT_MARKER_LENGTH*TW_MEASURE_MARKER_MULT)))
					 );
	this->drawMeasureMarkers(painter, 20);
	this->drawMeasureMarkers(painter, -1);
}



void TimelineWidget::drawPlayMarker(QPainter &p, float pos)
{
	QBrush playbrush(QColor(0, 255, 0));
	p.setBrush(playbrush);
	p.setPen(Qt::NoPen);
	p.drawRect(QRectF(QPointF(pos-(this->fTopSpacing/2.0f), 0.0f), QPointF(pos+(this->fTopSpacing/2.0f), this->fTopSpacing)));
	p.setPen(QPen(playbrush, 1.0f));
	p.drawLine(QPointF(pos, this->fTopSpacing), QPointF(pos, this->height()));
}

void TimelineWidget::drawMeasureMarkers(QPainter &p, int count)
{
	if(count<0)	p.setPen(QColor(255, 0, 0));
	else		p.setPen(this->palette().foreground().color());
	float beatspacing = this->fMeasureSpacing/this->iBeatsPerMeasure;
	unsigned int measurecount = abs(count);
	for(unsigned int measure=0; measure<measurecount; measure++) {
		for(int beat=1; beat<=this->iBeatsPerMeasure; beat++) {
			float xpos = (this->fMeasureSpacing*measure) + (beat*beatspacing);
			if(count<0)	xpos = -xpos;
			p.drawLine(	QPointF(xpos, this->fTopSpacing),
						QPointF(xpos, this->fTopSpacing+(TW_BEAT_MARKER_LENGTH*((beat==this->iBeatsPerMeasure)?TW_MEASURE_MARKER_MULT:1.0f)))
						);
		}
	}
}



int TimelineWidget::getNearestBeat(float pos)
{
	return roundf(pos/((this->fMeasureSpacing/this->iBeatsPerMeasure)));
}
