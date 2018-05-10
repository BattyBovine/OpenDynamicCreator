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
		QRect pixmapbounds(pixwidth*i, mybounds.y(),
						   pixwidth, mybounds.height());
		p->drawPixmap(pixmapbounds, this->lWaveformPixmaps[i]);
	}
	p->drawRoundedRect(mybounds, 4.0f, 4.0f);
}

void ClipTimelineItem::generateWaveform(const char *pcm, quint64 datalength, int bytespersample, int channels)
{
	const int zeropoint = roundf(128/2.0f);

	const int samplesize = (bytespersample*channels);
	const int samplecount = (datalength/samplesize);

	const float waveformlength = samplecount/2.0f;
	float tilewidth = waveformlength;
	int tilecount=1;
	while(tilewidth>CTI_MAX_TILE_LENGTH)
		tilewidth = waveformlength/float(++tilecount);
	const float samplesperpixel = samplecount / tilewidth;

	this->lWaveformPixmaps.clear();
	this->lWaveformPixmaps.reserve(tilecount);
	QPoint previouspoint(0,zeropoint);
	for(int tile=0; tile<tilecount; tile++) {
		int roundedwidth;
		if(tile>0)	roundedwidth = roundf(roundf(tilewidth*tile)/tile);
		else		roundedwidth = roundf(tilewidth);
		QPixmap waveform(roundedwidth, 128);
		QPainter *paint = new QPainter(&waveform);
		paint->setBrush(QColor(255,255,255));
		paint->setPen(Qt::NoPen);
		paint->drawRect(0,0,roundedwidth,128);
		paint->setPen(QColor(0,0,255));
		for(int x=0; x<roundedwidth; x++) {
			quint64 dataposition = roundf(((tile*tilewidth*samplesperpixel)+(x*samplesperpixel))/tilecount)*samplesize;
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
			convertedvalue = roundf((convertedvalue/float(SHRT_MAX))*zeropoint);
//			if(abs(convertedvalue)<=pixelvalue)
//				continue;
//			pixelvalue = convertedvalue;
			QPoint nextpoint(x,zeropoint-convertedvalue);
			paint->drawLine(previouspoint,nextpoint);
			previouspoint=nextpoint;
		}
		delete paint;
		this->lWaveformPixmaps.append(waveform);
		QString outpath = QString("%1/%2/").arg(QDir::tempPath()).arg("OpenDynamicCreator");
		QDir path(outpath);
		path.mkpath(outpath);
		outpath += QString("%1.png").arg(tile);
		QFile out(outpath);
		out.open(QFile::WriteOnly);
		waveform.save(&out, "PNG");
		out.close();
		previouspoint.setX(0);
	}
}
