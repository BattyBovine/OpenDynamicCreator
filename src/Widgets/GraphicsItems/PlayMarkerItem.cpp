#include "PlayMarkerItem.h"

void PlayMarkerItem::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
{
	QSettings settings;
	p->setPen(Qt::NoPen);
	QBrush playbrush(settings.value(KEY_PLAY_MARKER_COLOUR).value<QColor>());
	p->setBrush(playbrush);
	QRectF top = this->boundingRect();
	top.setHeight(top.width());
	p->drawRect(top);
	p->setPen(QPen(playbrush, 1.0f));
	p->drawLine(QPointF(0.0f, this->topSpacing()), QPointF(0.0f, TLI_MAX_HEIGHT));
}
