#ifndef CLIPTIMELINEITEM_H
#define CLIPTIMELINEITEM_H

#include "TimelineItem.h"


class ClipTimelineItem : public TimelineItem
{
public:
	ClipTimelineItem(float topspacing) : TimelineItem(topspacing){}

	void setBoundingRect() { TimelineItem::setBoundingRect(QRectF(QPointF(this->timelinePos(), 0.0f), QPointF(this->fLength-1, this->fHeight-1))); }
	void setLength(Beat l, float m, quint8 b, quint8 u) { this->beatLength=l; this->setLength(l.toTimelinePosition(m,b,u)); this->setBoundingRect(); }
	void setLength(float l) { this->fLength=l; this->setBoundingRect(); }
	void setHeight(float h) { this->fHeight=h; this->setBoundingRect(); }

protected:
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);

private:
	Beat beatLength;
	qreal fLength = 0.0f;
	qreal fHeight = 0.0f;
};

#endif // CLIPTIMELINEITEM_H
