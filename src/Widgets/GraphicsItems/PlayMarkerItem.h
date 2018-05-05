#ifndef PLAYMARKERITEM_H
#define PLAYMARKERITEM_H

#include <QGraphicsItem>
#include <QPainter>

#include "TimelineItem.h"


class PlayMarkerItem : public TimelineItem
{
public:
	PlayMarkerItem(float topspacing) : TimelineItem(topspacing){}

protected:
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);
};

#endif // PLAYMARKERITEM_H
