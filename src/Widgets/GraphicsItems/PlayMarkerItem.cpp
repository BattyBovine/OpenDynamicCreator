#include "PlayMarkerItem.h"

PlayMarkerItem::PlayMarkerItem(float topspacing)
{
	this->setZValue(1.0f);
	this->setTopSpacing(topspacing);

	this->rectBounds = QRectF(QPointF(-this->fTopSpacing/2.0f, 0.0f), QPointF(this->fTopSpacing/2.0f, this->fTopSpacing));
}



QRectF PlayMarkerItem::boundingRect() const
{
	QRectF bounds = this->rectBounds;
	bounds.setHeight(PMI_MAX_HEIGHT);
	return bounds;
}
void PlayMarkerItem::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
{
	p->setPen(Qt::NoPen);
	QBrush playbrush(QColor(0,255,0));
	p->setBrush(playbrush);
	p->drawRect(this->rectBounds);
	p->setPen(QPen(playbrush, 1.0f));
	p->drawLine(QPointF(0.0f, this->fTopSpacing), QPointF(0.0f, PMI_MAX_HEIGHT));
}
