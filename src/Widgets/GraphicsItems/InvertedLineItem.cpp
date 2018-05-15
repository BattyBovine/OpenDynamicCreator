#include "InvertedLineItem.h"

void InvertedLineItem::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
{
	p->setCompositionMode(QPainter::RasterOp_SourceXorDestination);
	QPen pen;
	pen.setColor(QColor(255,255,255));
	pen.setWidth(0);
	p->setPen(pen);
	p->drawLine(this->line());
}
