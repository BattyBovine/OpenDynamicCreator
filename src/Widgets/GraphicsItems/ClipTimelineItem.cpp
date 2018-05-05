#include "ClipTimelineItem.h"

void ClipTimelineItem::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
{
	p->setPen(QColor(0, 0, 255));
	p->setBrush(QColor(0, 0, 255, 64));
	p->drawRoundedRect(this->boundingRect(), 4.0f, 4.0f);
}
