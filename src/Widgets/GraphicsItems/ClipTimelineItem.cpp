#include "ClipTimelineItem.h"

void ClipTimelineItem::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
{
	p->setRenderHint(QPainter::HighQualityAntialiasing);
	p->setRenderHint(QPainter::SmoothPixmapTransform);
	const QRectF mybounds = this->boundingRect();
#ifndef QT_DEBUG
	p->setPen(Qt::NoPen);
	p->setBrush(QColor(0, 0, 255, 64));
	p->drawRect(mybounds);
#endif
	const int tilecount = this->mapWaveforms[this->iWaveformResolution].size();
	if(tilecount>0) {
		const float pixwidth = (mybounds.width()-1) / tilecount;
		for(int i=0; i<tilecount; i++) {
			const QPixmap &currentmap = this->mapWaveforms[this->iWaveformResolution][i];
			const QRectF itemregion(pixwidth*i, mybounds.y(), pixwidth+0.75f, mybounds.height());
			const QRectF pixmapbounds(0, 0, WT_MAX_TILE_LENGTH, currentmap.height());
			p->drawPixmap(itemregion, currentmap, pixmapbounds);
#ifdef QT_DEBUG
			p->setPen(QPen(QBrush(Qt::red),0.0f));
			p->setBrush(Qt::NoBrush);
			p->drawRect(itemregion);
#else
			p->setPen(QColor(0, 0, 255));
			p->setBrush(Qt::NoBrush);
			p->drawRoundedRect(mybounds,2.0f,2.0f);
#endif
		}
	}
}

void ClipTimelineItem::generateWaveform()
{
	const quint8 zoomscale = roundf(this->fTimelineScale+0.5f);
	if(zoomscale==this->iWaveformResolution || this->fHeight<1.0f)
		return;
	this->iWaveformResolution = zoomscale;

	const float waveformlength = this->fLength * (this->iWaveformResolution+1);
	const int tilecount = ceilf(waveformlength/WT_MAX_TILE_LENGTH);
	while(this->mapWaveforms[this->iWaveformResolution].size() < tilecount)
		this->mapWaveforms[this->iWaveformResolution].append(QPixmap());
	WaveformThread *thread = new WaveformThread(this->ccClip, this->fLength, this->fHeight, this->fTimelineScale,
												this->iWaveformResolution, tilecount);
	connect(thread, SIGNAL(tileFinished(int,int,QPixmap)), this, SLOT(getWaveformTile(int,int,QPixmap)));
	connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));
	thread->start();
}

void ClipTimelineItem::getWaveformTile(int resolution, int tile, QPixmap pix)
{
	this->mapWaveforms[resolution].replace(tile, pix);
	this->update(this->boundingRect());
}

void ClipTimelineItem::threadFinished()
{
	QObject::sender()->deleteLater();
}
