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
	EventMarkerItem(std::shared_ptr<MusicEvent> me, float topspacing) : TimelineItem(topspacing) { this->meEvent=me; this->setZValue(10.0f); }
	void setTimelinePos(Beat p, float s, quint8 b, quint8 u) override { this->meEvent->setPosition(p); TimelineItem::setTimelinePos(p,s,b,u); }


protected:
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);
	std::shared_ptr<MusicEvent> meEvent;
};

#endif // EVENTMARKERITEM_H
