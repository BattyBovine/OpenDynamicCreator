#include "EventMarkerItem.h"

void EventMarkerItem::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
{
	QSettings settings;
	p->setBrush(Qt::NoBrush);
	p->setPen(settings.value(KEY_EVENT_MARKER_COLOUR).value<QColor>());
	QVector<QPointF> polyline;
	const float topspacing = this->topSpacing();
	polyline << QPointF(0.0f, 0.0f)
			 << QPointF(-(topspacing/2.0f), (topspacing/2.0f))
			 << QPointF(0.0f, topspacing)
			 << QPointF((topspacing/2.0f), (topspacing/2.0f))
			 << QPointF(0.0f, 0.0f);
	p->drawPolyline(QPolygonF(polyline));
	p->drawLine(QPointF(0.0f, topspacing), QPointF(0.0f, TLI_MAX_HEIGHT));
}
