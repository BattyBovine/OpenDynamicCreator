#ifndef TIMELINEITEM_H
#define TIMELINEITEM_H

#include <QGraphicsItem>
#include <QPainter>

#include "Classes/MusicEvent.h"

#define TLI_MAX_HEIGHT	2048.0f


class TimelineItem : public QGraphicsObject
{
	Q_OBJECT
public:
	TimelineItem(float topspacing);

	virtual void setTopSpacing(float s) { this->fTopSpacing=s; }
	virtual void setTimelinePos(Beat p, float s, quint8 b, quint8 u) { this->beatPosition=p; this->setTimelinePos(p.toTimelinePosition(s,b,u)); emit(moved(p)); }
	virtual void setTimelinePos(float p) { this->setPos(p,0.0f); }

	float topSpacing() { return this->fTopSpacing; }
	Beat timelineBeat() { return this->beatPosition; }
	float timelinePos() { return this->pos().x(); }

signals:
	void moved(Beat&);

protected:
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*){}
	virtual QRectF boundingRect() const { return this->rectBounds; }

private:
	QRectF rectBounds;
	Beat beatPosition;
	float fTopSpacing=8.0f;
};

#endif // TIMELINEITEM_H
