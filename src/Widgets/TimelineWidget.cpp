#include "Widgets/TimelineWidget.h"

TimelineWidget::TimelineWidget(QWidget *parent) : QWidget(parent)
{

}

void TimelineWidget::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.drawLine(QLineF(QPointF(0.0f, 0.0f), QPointF(0.0f, TW_BEAT_MARKER_LENGTH*TW_MEASURE_MARKER_MULT)));
	this->drawMeasures(painter, 20);
	this->drawMeasures(painter, -1);
}



void TimelineWidget::drawMeasures(QPainter &p, int count)
{
	const int beatspacing = 20;									// PLACEHOLDER
	const int beatspermeasure = 4;								// PLACEHOLDER
	const int measurespacing = beatspacing * beatspermeasure;	// PLACEHOLDER

	if(count<0)
		p.setPen(QColor(255, 0, 0));
	else
		p.setPen(this->palette().foreground().color());

	unsigned int measurecount = abs(count);
	for(unsigned int measure=0; measure<measurecount; measure++) {
		for(int beat=1; beat<=beatspermeasure; beat++) {
			float xpos = (measurespacing*measure) + (beat*beatspacing);
			if(count<0)	xpos = -xpos;
			p.drawLine(QLineF(QPointF(xpos, 0.0f), QPointF(xpos, TW_BEAT_MARKER_LENGTH*((beat==4)?TW_MEASURE_MARKER_MULT:1.0f))));
		}
	}
}
