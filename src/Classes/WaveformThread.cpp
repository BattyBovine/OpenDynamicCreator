#include "WaveformThread.h"

WaveformThread::WaveformThread(ClipContainerPtr clip, float width, float height, float scale, int resolution, int tilecount)
{
	this->ccClip = clip;
	this->fWidth = width;
	this->fHeight = height;
	this->fScale = scale;
	this->iResolution = resolution;
	this->iTileCount = tilecount;
}

void WaveformThread::run()
{
	const char *pcm = this->ccClip->rawData();
	const quint64 datalength = this->ccClip->rawDataLength();
	const quint8 bytespersample = this->ccClip->bytesPerSample();
	const quint8 channels = this->ccClip->channelCount();

	const int zeropoint = roundf(this->fHeight/2.0f);
	const int samplesize = (bytespersample*channels);
	const int samplecount = (datalength/samplesize);

	const float samplesperpixel = float(samplecount) / (WT_MAX_TILE_LENGTH*this->iTileCount);

	QSettings settings;
	QString cachepath = settings.value(KEY_TEMP_FOLDER).toString();
	if(cachepath.isEmpty())
		return;
	cachepath.append(QString("/%1/").arg(this->ccClip->uuidString()));
	QDir path(cachepath);
	QFile tempofile(cachepath+(QString("/%1").arg(this->ccClip->tempo()*100.0f)));
	if(!tempofile.exists()) {
		path.setFilter(QDir::NoDotAndDotDot);
		path.removeRecursively();
		path.mkpath(cachepath);
		tempofile.open(QFile::WriteOnly);
		tempofile.close();
	}
	cachepath.append(QString("%1/").arg(this->iResolution));
	path.setPath(cachepath);
	path.mkpath(cachepath);

	QColor waveformcolour = settings.value(KEY_WAVEFORM_COLOUR).value<QColor>();
	QPoint previouspoint(0,zeropoint);
	for(int tile=0; tile<this->iTileCount; tile++) {
		QImage waveform;
		QFile out(path.absolutePath()+QString("/%1.bmp").arg(tile));
		if(out.exists()) {
			waveform.load(&out,"BMP");
			waveform.setColor(0, QColor(255,255,255,0).rgba());
			waveform.setColor(1, waveformcolour.rgba());
			emit(tileFinished(this->iResolution,tile,QPixmap::fromImage(waveform,Qt::MonoOnly)));
			continue;
		}
		waveform = QImage(WT_MAX_TILE_LENGTH, this->fHeight, QImage::Format_Mono);
		QPainter *paint = new QPainter(&waveform);
		paint->setBrush(QColor(255,255,255));
		paint->setPen(Qt::NoPen);
		paint->drawRect(0,0,WT_MAX_TILE_LENGTH,this->fHeight);
		paint->setPen(QColor(0,0,0));
		for(int x=0; x<WT_MAX_TILE_LENGTH; x++) {
			quint64 dataposition = roundf(((tile*WT_MAX_TILE_LENGTH)+x)*samplesperpixel)*samplesize;
			qint32 hivalue=0, lovalue=0;
			const int roundedspp = roundf(roundf(samplesperpixel*x)/x);
			for(int s=0; s<roundedspp; s++) {
				for(int c=0; c<channels; c++) {
					quint64 dataoffset = dataposition+(s*samplesize)+(c*bytespersample);
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
			if(hivalue!=0) {
				QPoint nextpoint = this->intToPixel(hivalue,x,bytespersample,zeropoint);
				paint->drawLine(previouspoint,nextpoint);
				previouspoint=nextpoint;
			}
			if(lovalue!=0) {
				QPoint nextpoint = this->intToPixel(lovalue,x,bytespersample,zeropoint);
				paint->drawLine(previouspoint,nextpoint);
				previouspoint=nextpoint;
			}
		}
		delete paint;

		out.open(QFile::WriteOnly);
		waveform.save(&out);
		out.close();
		waveform.setColor(0, waveformcolour.rgba());
		waveform.setColor(1, QColor(255,255,255,0).rgba());
		emit(tileFinished(this->iResolution, tile, QPixmap::fromImage(waveform,Qt::MonoOnly)));
		previouspoint.setX(0);
	}
}
