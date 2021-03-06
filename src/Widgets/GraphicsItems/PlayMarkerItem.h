#ifndef PLAYMARKERITEM_H
#define PLAYMARKERITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QSettings>

#include "TimelineItem.h"
#include "PreferencesDialog.h"


class PlayMarkerItem : public TimelineItem
{
public:
	PlayMarkerItem(float topspacing) : TimelineItem(topspacing) { this->setZValue(20.0f); }

protected:
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);
};

#endif // PLAYMARKERITEM_H
