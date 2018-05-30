#ifndef EVENTMARKERITEM_H
#define EVENTMARKERITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QSettings>

#include "TimelineItem.h"
#include "PreferencesDialog.h"
#include "Classes/StaticMusicEvent.h"


class EventMarkerItem : public TimelineItem
{
public:
	EventMarkerItem(StaticMusicEventPtr sme, float topspacing) : TimelineItem(topspacing) { this->smeEvent=sme; this->setZValue(10.0f); }
	void setTimelinePos(Beat p, float s, quint8 b, quint8 u) { this->smeEvent->setBeat(p); TimelineItem::setTimelinePos(p,s,b,u); }

protected:
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);
	StaticMusicEventPtr smeEvent;
};

#endif // EVENTMARKERITEM_H
