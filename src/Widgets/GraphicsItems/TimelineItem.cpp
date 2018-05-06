#include "TimelineItem.h"

TimelineItem::TimelineItem(float topspacing)
{
	this->setZValue(1.0f);
	this->setTopSpacing(topspacing);
	this->rectBounds = QRectF(QPointF(-this->fTopSpacing/2.0f, 0.0f), QPointF(this->fTopSpacing/2.0f, TLI_MAX_HEIGHT));
}
