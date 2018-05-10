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

void ClipTimelineItem::generateWaveform(ClipContainer *clip, float scale)
{
	if(floorf(scale)==this->iWaveformScale)
		return;
	this->iWaveformScale=floorf(scale);

	const char *pcm = clip->rawData();
	const quint64 datalength = clip->rawDataLength();
	const quint8 bytespersample = clip->bytesPerSample();
	const quint8 channels = clip->channelCount();

	const int zeropoint = roundf(this->fHeight/2.0f);
	const int samplesize = (bytespersample*channels);
	const int samplecount = (datalength/samplesize);
	const float waveformlength = this->fLength*this->fTimelineScale;
	float tilewidth = waveformlength;
	int tilecount=1;
	while(tilewidth>CTI_MAX_TILE_LENGTH)
		tilewidth = waveformlength/float(++tilecount);
	const float samplesperpixel = samplecount / tilewidth;

	QString outpath = QString("%1/%2/%3/%4/")
					  .arg(QDir::tempPath())
					  .arg("OpenDynamicCreator")
					  .arg(clip->uuidString())
					  .arg(this->iWaveformScale);
	QDir path(outpath);
	path.mkpath(outpath);

	this->lWaveformPixmaps.clear();
	this->lWaveformPixmaps.reserve(tilecount);
	QPoint previouspoint(0,zeropoint);
	for(int tile=0; tile<tilecount; tile++) {
		QFile out(path.absolutePath()+QString("/%1").arg(tile));
		if(out.exists()) {
			this->lWaveformPixmaps.append(QPixmap(path.absolutePath()+QString("/%1").arg(tile)));
			continue;
		}
		int roundedwidth;
		if(tile>0)	roundedwidth = roundf(roundf(tilewidth*tile)/tile);
		else		roundedwidth = roundf(tilewidth);
		QPixmap waveform(roundedwidth, this->fHeight);
		QPainter *paint = new QPainter(&waveform);
		paint->setBrush(QColor(255,255,255));
		paint->setPen(Qt::NoPen);
		paint->drawRect(0,0,roundedwidth,this->fHeight);
		paint->setPen(QColor(0,0,255));
		for(int x=0; x<roundedwidth; x++) {
			quint64 dataposition = roundf(((tile*tilewidth*samplesperpixel)+(x*samplesperpixel))/tilecount)*samplesize;
//			int pixelvalue = 0;
			for(int s=0; s<=samplesperpixel; s++) {
				quint64 dataoffset = dataposition+(s*samplesize);
				if(dataoffset>datalength) break;
				quint32 samplevalue = 0;
				switch(bytespersample) {
				case 4:	samplevalue |= quint32(pcm[dataoffset+3] << 24);
				case 3:	samplevalue |= quint32(pcm[dataoffset+2] << 16);
				case 2:	samplevalue |= quint16(pcm[dataoffset+1] << 8);
				case 1:	samplevalue |= quint8 (pcm[dataoffset]);
				}
				int convertedvalue = 0;
				switch(bytespersample) {
				case 4:	convertedvalue = qint32(samplevalue); break;
				case 3:	convertedvalue = qint32(samplevalue); break;
				case 2:	convertedvalue = qint16(samplevalue); break;
				case 1:	convertedvalue = qint8 (samplevalue); break;
				}
				convertedvalue = roundf((convertedvalue/float(SHRT_MAX))*zeropoint);
//				if(abs(convertedvalue)<=abs(pixelvalue))
//					continue;
//				pixelvalue = convertedvalue;
				QPoint nextpoint(x,zeropoint-convertedvalue);
				paint->drawLine(previouspoint,nextpoint);
				previouspoint=nextpoint;
			}
		}
		delete paint;
		this->lWaveformPixmaps.append(waveform);
		out.open(QFile::WriteOnly);
		waveform.save(&out, "BMP");
		out.close();
		previouspoint.setX(0);
	}
}
