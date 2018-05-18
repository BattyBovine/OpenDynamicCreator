#ifndef EVENTMARKERITEM_H
#define EVENTMARKERITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QSettings>

#include "TimelineItem.h"
#include "PreferencesDialog.h"


class EventMarkerItem : public TimelineItem
{
public:
	EventMarkerItem(float topspacing) : TimelineItem(topspacing) { this->setZValue(10.0f); }

protected:
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);
};

#endif // EVENTMARKERITEM_H
