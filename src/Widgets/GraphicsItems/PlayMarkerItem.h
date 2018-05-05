#ifndef PLAYMARKERITEM_H
#define PLAYMARKERITEM_H

#include <QGraphicsItem>
#include <QPainter>

#include "Classes/MusicEvent.h"

#define PMI_MAX_HEIGHT	2048.0f


class PlayMarkerItem : public QGraphicsItem
{
public:
	PlayMarkerItem(float topspacing=8.0f);

	void setTopSpacing(float s) { this->fTopSpacing=s; }
	void setTimelinePos(float p) { this->setPos(p,0.0f); }

	float topSpacing() { return this->fTopSpacing; }

protected:
	QRectF boundingRect() const;
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);

private:
	QRectF rectBounds;
	float fTopSpacing=8.0f;
};

#endif // PLAYMARKERITEM_H
