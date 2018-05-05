#include "TimelineItem.h"

TimelineItem::TimelineItem(float topspacing)
{
	this->setZValue(1.0f);
	this->setTopSpacing(topspacing);
	this->setBoundingRect(QRectF(QPointF(-this->fTopSpacing/2.0f, 0.0f), QPointF(this->fTopSpacing/2.0f, TLI_MAX_HEIGHT)));
}

QRectF TimelineItem::boundingRect() const
{
	QRectF bounds = this->rectBounds;
	bounds.setHeight(TLI_MAX_HEIGHT);
	return bounds;
}
