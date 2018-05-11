#include "ClipTimelineItem.h"

void ClipTimelineItem::paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*)
{
	p->setPen(QColor(0, 0, 255));
	p->setBrush(QColor(0, 0, 255, 64));
	p->setRenderHint(QPainter::HighQualityAntialiasing);
	p->setRenderHint(QPainter::SmoothPixmapTransform);
	QRectF mybounds = this->boundingRect();
	float pixwidth = mybounds.width() / this->lWaveformPixmaps.size();
	for(int i=0; i<this->lWaveformPixmaps.size(); i++) {
		QRectF itemregion(pixwidth*i, mybounds.y(),
						   pixwidth, mybounds.height());
		QRectF pixmapbounds(0,0,this->lWaveformPixmaps[i].width(),this->lWaveformPixmaps[i].height());
		p->drawPixmap(itemregion, this->lWaveformPixmaps[i],
					  pixmapbounds);
	}
	p->drawRoundedRect(mybounds, 4.0f, 4.0f);
}

void ClipTimelineItem::generateWaveform(ClipContainer &clip)
{
	if(this->fHeight<1.0f || floorf(this->fTimelineScale*CTI_RESOLUTION_SUBLEVELS)==this->iWaveformResolution)
		return;
	this->iWaveformResolution = floorf(this->fTimelineScale*CTI_RESOLUTION_SUBLEVELS);

	const char *pcm = clip.rawData();
	const quint64 datalength = clip.rawDataLength();
	const quint8 bytespersample = clip.bytesPerSample();
	const quint8 channels = clip.channelCount();

	const int zeropoint = roundf(this->fHeight/2.0f);
	const int samplesize = (bytespersample*channels);
	const int samplecount = (datalength/samplesize);

	const float waveformlength = (this->fLength*this->fTimelineScale);
	int tilewidth = waveformlength;
	int tilecount = 1;
	while(tilewidth>CTI_MAX_TILE_LENGTH)
		tilewidth = (waveformlength/(++tilecount));
	const int samplesperpixel = (samplecount/(tilewidth*tilecount));

	QString outpath = QString("%1/%2/%3/%4/")
					  .arg(QDir::tempPath())
					  .arg("OpenDynamicCreator")
					  .arg(clip.uuidString())
					  .arg(this->iWaveformResolution);
	QDir path(outpath);
	path.mkpath(outpath);

	this->lWaveformPixmaps.clear();
	this->lWaveformPixmaps.reserve(tilecount);
	QPoint previouspoint(0,zeropoint);
	for(int tile=0; tile<tilecount; tile++) {
		QFile out(path.absolutePath()+QString("/%1.bmp").arg(tile));
		QImage waveform;
		if(out.exists()) {
			waveform.load(&out,"BMP");
			this->lWaveformPixmaps.append(QPixmap::fromImage(waveform,Qt::MonoOnly));
			continue;
		}
		waveform = QImage(tilewidth, this->fHeight, QImage::Format_Mono);
		QPainter *paint = new QPainter(&waveform);
		paint->setBrush(QColor(255,255,255));
		paint->setPen(Qt::NoPen);
		paint->drawRect(0,0,tilewidth,this->fHeight);
		paint->setPen(QColor(0,0,255));
		for(int x=0; x<tilewidth; x++) {
			quint64 dataposition = ((tile*tilewidth)+x)*samplesperpixel*samplesize;
			Q_ASSERT(dataposition<=datalength);
			quint32 samplevalue = 0;
			switch(bytespersample) {
			case 4:	samplevalue |= quint32(pcm[dataposition+3] << 24);
			case 3:	samplevalue |= quint32(pcm[dataposition+2] << 16);
			case 2:	samplevalue |= quint16(pcm[dataposition+1] << 8);
			case 1:	samplevalue |= quint8 (pcm[dataposition]);
			}
			int convertedvalue = 0;
			switch(bytespersample) {
			case 4:	convertedvalue = qint32(samplevalue); break;
			case 3:	convertedvalue = qint32(samplevalue); break;
			case 2:	convertedvalue = qint16(samplevalue); break;
			case 1:	convertedvalue = qint8 (samplevalue); break;
			}
			float pixelvalue = 0;
			switch(bytespersample) {
			case 3:	pixelvalue = (convertedvalue/float(INT_MAX)); break;
			case 4:	pixelvalue = (convertedvalue/float(INT_MAX)); break;
			case 2:	pixelvalue = (convertedvalue/float(SHRT_MAX)); break;
			case 1:	pixelvalue = (convertedvalue/float(CHAR_MAX)); break;
			}
			pixelvalue *= zeropoint;
			QPoint nextpoint(x,zeropoint-roundf(pixelvalue));
			paint->drawLine(previouspoint,nextpoint);
			previouspoint=nextpoint;
		}
		delete paint;
		this->lWaveformPixmaps.append(QPixmap::fromImage(waveform,Qt::MonoOnly));
		out.open(QFile::WriteOnly);
		waveform.save(&out);
		out.close();
		previouspoint.setX(0);
	}
}
