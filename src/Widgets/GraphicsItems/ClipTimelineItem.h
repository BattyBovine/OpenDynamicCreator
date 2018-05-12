#ifndef CLIPTIMELINEITEM_H
#define CLIPTIMELINEITEM_H

#include <QImage>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

#include "TimelineItem.h"
#include "Classes/ClipContainer.h"

#define CTI_MAX_TILE_LENGTH	256.0f
#define CTI_RESOLUTION_SUBLEVELS 2


class ClipTimelineItem : public TimelineItem
{
public:
	ClipTimelineItem(float topspacing) : TimelineItem(topspacing) { this->setZValue(-10); }

	virtual void setTimelinePos(Beat p, float s, quint8 b, quint8 u) { TimelineItem::setTimelinePos(p,s,b,u); this->updateBoundingRect(); }

	void setLength(Beat l, float s, quint8 b, quint8 u) { this->fLength=l.toTimelinePosition(s,b,u); this->updateBoundingRect(); }
	void setHeight(float h) { this->fHeight=h; this->updateBoundingRect(); }
	void setTimelineScale(qreal scale) { this->fTimelineScale=scale; this->updateBoundingRect(); }

	void generateWaveform(ClipContainer&);

protected:
	virtual QRectF boundingRect() const { return this->rectBounds; }
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);

private:
	void updateBoundingRect() { this->rectBounds = QRectF(QPointF(0.0f,this->topSpacing()),QSizeF((this->fLength*this->fTimelineScale)-1,(this->fHeight-this->topSpacing())-1)); }

	QRectF rectBounds;
	qreal fLength = 0.0f;
	qreal fHeight = 0.0f;
	qreal fTimelineScale = 1.0f;
	int iWaveformResolution = 1;
	QList<QPixmap> lWaveformPixmaps;
};

#endif // CLIPTIMELINEITEM_H
