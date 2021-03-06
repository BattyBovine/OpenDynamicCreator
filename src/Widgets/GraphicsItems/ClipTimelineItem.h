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
	ClipTimelineItem(ClipContainerPtr clip, float measurespacing, float topspacing) : TimelineItem(topspacing) {
		this->ccClip = clip;
		TimelineItem::setTimelinePos(clip->timelineOffset(),measurespacing,clip->beatsPerMeasure(),clip->beatUnit());
		this->updateLength(measurespacing);
		this->setZValue(-10.0f);
	}

	virtual void setTimelinePos(Beat p, float s) { TimelineItem::setTimelinePos(p,s,this->ccClip->beatsPerMeasure(),this->ccClip->beatUnit()); this->ccClip->setTimelineOffset(this->timelineBeat()); this->updateBoundingRect(); }

	void updateLength(float s) { this->fLength=this->ccClip->beats().toTimelinePosition(s,this->ccClip->beatsPerMeasure(),this->ccClip->beatUnit()); this->updateBoundingRect(); }
	void setHeight(float h) { this->fHeight=h; this->updateBoundingRect(); }
	void setTimelineScale(qreal scale) { this->fTimelineScale=scale; this->generateWaveform(); this->updateBoundingRect(); }

	void generateWaveform();

private slots:
	void getWaveformTile(int,int,QPixmap);
	void threadFinished();

protected:
	virtual QRectF boundingRect() const { return this->rectBounds; }
	void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);

private:
	void updateBoundingRect() { this->rectBounds = QRectF(QPointF(0.0f,this->topSpacing()),QSizeF((this->fLength*this->fTimelineScale)-1,(this->fHeight-this->topSpacing())-1)); }

	ClipContainerPtr ccClip;
	QRectF rectBounds;
	qreal fLength = 0.0f;
	qreal fHeight = 0.0f;
	qreal fTimelineScale = 1.0f;
	quint8 iWaveformResolution = 0;
	WaveformMap mapWaveforms;
};

#endif // CLIPTIMELINEITEM_H
