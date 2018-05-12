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
	const quint8 zoomscale = roundf(this->fTimelineScale*CTI_RESOLUTION_SUBLEVELS);
	if(this->fHeight<1.0f || zoomscale==this->iWaveformResolution)
		return;
	this->iWaveformResolution = zoomscale;

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
					  .arg(QCoreApplication::applicationName())
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
			int hivalue=0, lovalue=0;
			for(int s=0; s<samplesperpixel; s++) {
				for(int c=0; c<channels; c++) {
					quint64 dataoffset = dataposition+(s*samplesize);
					Q_ASSERT(dataposition<=datalength);
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
					if(convertedvalue>hivalue || hivalue==0)
						hivalue = convertedvalue;
					if(convertedvalue<lovalue || lovalue==0)
						lovalue = convertedvalue;
				}
			}
			float hipixelvalue=0.0f,lopixelvalue=0.0f;
			if(hivalue!=0) {
				switch(bytespersample) {
				case 3:	hipixelvalue = (hivalue/float(INT_MAX)); break;
				case 4:	hipixelvalue = (hivalue/float(INT_MAX)); break;
				case 2:	hipixelvalue = (hivalue/float(SHRT_MAX)); break;
				case 1:	hipixelvalue = (hivalue/float(CHAR_MAX)); break;
				}
				hipixelvalue *= zeropoint;
				QPoint nextpoint(x,zeropoint-roundf(hipixelvalue));
				paint->drawLine(previouspoint,nextpoint);
				previouspoint=nextpoint;
			}
			if(lovalue!=0) {
				switch(bytespersample) {
				case 3:	lopixelvalue = (lovalue/float(INT_MAX)); break;
				case 4:	lopixelvalue = (lovalue/float(INT_MAX)); break;
				case 2:	lopixelvalue = (lovalue/float(SHRT_MAX)); break;
				case 1:	lopixelvalue = (lovalue/float(CHAR_MAX)); break;
				}
				lopixelvalue *= zeropoint;
				QPoint nextpoint(x,zeropoint-roundf(lopixelvalue));
				paint->drawLine(previouspoint,nextpoint);
				previouspoint=nextpoint;
			}
		}
		delete paint;
		this->lWaveformPixmaps.append(QPixmap::fromImage(waveform,Qt::MonoOnly));
		out.open(QFile::WriteOnly);
		waveform.save(&out);
		out.close();
		previouspoint.setX(0);
	}
}
