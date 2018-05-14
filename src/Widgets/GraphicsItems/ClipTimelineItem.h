#ifndef CLIPTIMELINEITEM_H
#define CLIPTIMELINEITEM_H

#include <memory>

#include <QImage>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

#include "TimelineItem.h"
#include "Classes/ClipContainer.h"
#include "Classes/WaveformThread.h"

typedef QMap<unsigned int,QList<QPixmap> > WaveformMap;


class ClipTimelineItem : public TimelineItem
{
	Q_OBJECT
public:
	ClipTimelineItem(float topspacing) : TimelineItem(topspacing) { this->setZValue(-10); }

	virtual void setTimelinePos(Beat p, float s, quint8 b, quint8 u) { TimelineItem::setTimelinePos(p,s,b,u); this->updateBoundingRect(); }

	void setLength(Beat l, float s, quint8 b, quint8 u) { this->fLength=l.toTimelinePosition(s,b,u); this->updateBoundingRect(); }
	void setHeight(float h) { this->fHeight=h; this->updateBoundingRect(); }
	void setTimelineScale(qreal scale) { this->fTimelineScale=scale; this->updateBoundingRect(); }

	void generateWaveform(std::shared_ptr<ClipContainer>);

private slots:
	void getWaveformTile(int,int,QPixmap);
	void threadFinished();

protected:
	virtual QRectF boundingRect() const { return this->rectBounds; }
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);

private:
	void updateBoundingRect() { this->rectBounds = QRectF(QPointF(0.0f,this->topSpacing()),QSizeF((this->fLength*this->fTimelineScale)-1,(this->fHeight-this->topSpacing())-1)); }

	QRectF rectBounds;
	qreal fLength = 0.0f;
	qreal fHeight = 0.0f;
	qreal fTimelineScale = 1.0f;
	quint8 iWaveformResolution = 1;
	WaveformMap mapWaveforms;
	WaveformMap mapWaveformBuffer;
};

#endif // CLIPTIMELINEITEM_H
