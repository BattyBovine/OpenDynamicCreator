#include "ClipTimelineItem.h"

void ClipTimelineItem::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
{
	p->setRenderHint(QPainter::HighQualityAntialiasing);
	p->setRenderHint(QPainter::SmoothPixmapTransform);
	const QRectF mybounds = this->boundingRect();
	const int tilecount = this->mapWaveforms[this->iWaveformResolution].size();
	const float pixwidth = mybounds.width() / tilecount;
	for(int i=0; i<tilecount; i++) {
		QRectF itemregion(pixwidth*i, mybounds.y(), pixwidth, mybounds.height());
		QRectF pixmapbounds(0, 0, WT_MAX_TILE_LENGTH, this->mapWaveforms[this->iWaveformResolution][i].height());
		p->drawPixmap(itemregion, this->mapWaveforms[this->iWaveformResolution][i], pixmapbounds);
#ifdef QT_DEBUG
		p->setPen(Qt::red);
		p->setBrush(Qt::NoBrush);
		p->drawRect(itemregion);
#endif
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

	this->mapWaveforms[this->iWaveformResolution].clear();
	this->mapWaveforms[this->iWaveformResolution].reserve(tilecount);
	for(int i=0; i<tilecount; i++)
		this->mapWaveforms[this->iWaveformResolution].append(QPixmap());
	WaveformThread *thread = new WaveformThread(clip, this->fLength, this->fHeight, this->fTimelineScale,
												this->iWaveformResolution, tilecount);
	connect(thread, SIGNAL(tileFinished(int,int,QPixmap)), this, SLOT(getWaveformTile(int,int,QPixmap)));
	connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));
	thread->start();
}

void ClipTimelineItem::getWaveformTile(int resolution, int tile, QPixmap pix)
{
	this->mapWaveforms[resolution][tile] = pix;
	if(resolution==this->iWaveformResolution)
		this->update(this->boundingRect());
//		this->update(WT_MAX_TILE_LENGTH*tile,0.0f,pix.width(),this->fHeight);
}

void ClipTimelineItem::threadFinished()
{
	delete QObject::sender();
}
