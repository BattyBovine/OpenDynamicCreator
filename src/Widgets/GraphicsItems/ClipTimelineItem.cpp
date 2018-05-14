#include "ClipTimelineItem.h"

void ClipTimelineItem::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
{
	p->setRenderHint(QPainter::HighQualityAntialiasing);
	p->setRenderHint(QPainter::SmoothPixmapTransform);
	const QRectF mybounds = this->boundingRect();
	const int tilecount = this->mapWaveforms[this->iWaveformResolution].size();
	if(tilecount>0) {
		const float pixwidth = mybounds.width() / tilecount;
		for(int i=0; i<tilecount; i++) {
			const QPixmap &currentmap = this->mapWaveforms[this->iWaveformResolution][i];
			const QRectF itemregion(pixwidth*i+1, mybounds.y(), pixwidth, mybounds.height());
			const QRectF pixmapbounds(0, 0, WT_MAX_TILE_LENGTH, currentmap.height());
			p->drawPixmap(itemregion, currentmap, pixmapbounds);
#ifdef QT_DEBUG
			p->setPen(Qt::red);
			p->setBrush(Qt::NoBrush);
			p->drawRect(itemregion);
#endif
		}
	}
#ifndef QT_DEBUG
	p->setPen(QColor(0, 0, 255));
	p->setBrush(QColor(0, 0, 255, 64));
	p->drawRoundedRect(mybounds, 4.0f, 4.0f);
#endif
}

void ClipTimelineItem::generateWaveform(std::shared_ptr<ClipContainer> clip)
{
	const quint8 zoomscale = roundf(this->fTimelineScale);
	if(this->fHeight<1.0f || zoomscale==this->iWaveformResolution)
		return;
	this->iWaveformResolution = zoomscale;

	float waveformlength;
	if(this->iWaveformResolution==0)
		waveformlength = this->fLength;
	else
		waveformlength = (this->fLength*this->iWaveformResolution*WT_RESOLUTION_MULTIPLIER);
	const int tilecount = ceilf(waveformlength/WT_MAX_TILE_LENGTH);

	this->mapWaveformBuffer[this->iWaveformResolution].clear();
	this->mapWaveformBuffer[this->iWaveformResolution].reserve(tilecount);
	for(int i=0; i<tilecount; i++)
		this->mapWaveformBuffer[this->iWaveformResolution].append(QPixmap());
	WaveformThread *thread = new WaveformThread(clip, this->fLength, this->fHeight, this->fTimelineScale,
												this->iWaveformResolution, tilecount);
	connect(thread, SIGNAL(tileFinished(int,int,QPixmap)), this, SLOT(getWaveformTile(int,int,QPixmap)));
	connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));
	thread->start();
}

void ClipTimelineItem::getWaveformTile(int resolution, int tile, QPixmap pix)
{
	this->mapWaveformBuffer[resolution][tile] = pix;
}

void ClipTimelineItem::threadFinished()
{
	this->mapWaveforms = this->mapWaveformBuffer;
	this->update(this->boundingRect());
	delete QObject::sender();
}
