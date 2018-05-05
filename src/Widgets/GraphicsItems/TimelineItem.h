#ifndef TIMELINEITEM_H
#define TIMELINEITEM_H

#include <QGraphicsItem>
#include <QPainter>

#include "Classes/MusicEvent.h"

#define TLI_MAX_HEIGHT	2048.0f


class TimelineItem : public QGraphicsItem
{
public:
	TimelineItem(float topspacing);

	void setTopSpacing(float s) { this->fTopSpacing=s; }
	void setTimelineBeat(Beat p, float s, quint8 b, quint8 u) { this->beatPosition=p; this->setTimelinePos(p.toTimelinePosition(s,b,u)); }
	void setTimelinePos(float p) { this->setPos(p,0.0f); }

	float topSpacing() { return this->fTopSpacing; }
	Beat timelineBeat() { return this->beatPosition; }
	float timelinePos() { return this->pos().x(); }

protected:
	void setBoundingRect(QRectF b) { this->rectBounds=b; }
	QRectF boundingRect() const;

private:
	QRectF rectBounds;
	Beat beatPosition;
	float fTopSpacing=8.0f;
};

#endif // TIMELINEITEM_H
