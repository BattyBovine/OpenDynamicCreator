#include "MusicPropertiesWidget.h"
#include "ui_MusicPropertiesWidget.h"

MusicPropertiesWidget::MusicPropertiesWidget(TrackItem *track, QWidget *parent) :
	QScrollArea(parent),
	ui(new Ui::MusicPropertiesWidget)
{
	ui->setupUi(this);

	this->attachTrackItem(track);

	connect(ui->spinTempo, SIGNAL(valueChanged(int)), this, SLOT(tempoChanged(int)));
	connect(ui->spinBeatsPerMeasure, SIGNAL(valueChanged(int)), this, SLOT(beatsPerMeasureChanged(int)));
	connect(ui->spinBeatUnit, SIGNAL(valueChanged(int)), this, SLOT(beatUnitChanged(int)));
	connect(ui->dspinPlaybackSpeed, SIGNAL(valueChanged(double)), this, SLOT(playbackSpeedChanged(double)));
}

MusicPropertiesWidget::~MusicPropertiesWidget()
{
	delete ui;
}



void MusicPropertiesWidget::attachTrackItem(TrackItem *t)
{
	this->tiTrack = t;
	this->setTempo(t->tempo());
	this->setBeatsPerMeasure(t->beatsPerMeasure());
	this->setBeatUnit(t->beatUnit());
	this->setPlaybackSpeed(t->playbackSpeed());
}



void MusicPropertiesWidget::tempoChanged(int t)
{
	if(this->tiTrack)
		this->tiTrack->setTempo(t);
}
void MusicPropertiesWidget::setTempo(int t)
{
	ui->spinTempo->setValue(t);
}
int MusicPropertiesWidget::tempo()
{
	return ui->spinTempo->value();
}

void MusicPropertiesWidget::beatsPerMeasureChanged(int b)
{
	if(this->tiTrack)
		this->tiTrack->setBeatsPerMeasure(b);
}
void MusicPropertiesWidget::setBeatsPerMeasure(int b)
{
	ui->spinBeatsPerMeasure->setValue(b);
}
int MusicPropertiesWidget::beatsPerMeasure()
{
	return ui->spinBeatsPerMeasure->value();
}

void MusicPropertiesWidget::beatUnitChanged(int b)
{
	if(this->tiTrack)
		this->tiTrack->setBeatUnit(b);
}
void MusicPropertiesWidget::setBeatUnit(int b)
{
	ui->spinBeatUnit->setValue(b);
}
int MusicPropertiesWidget::beatUnit()
{
	return ui->spinBeatUnit->value();
}

void MusicPropertiesWidget::playbackSpeedChanged(double s)
{
	if(this->tiTrack)
		this->tiTrack->setPlaybackSpeed(s);
}
void MusicPropertiesWidget::setPlaybackSpeed(double s)
{
	ui->dspinPlaybackSpeed->setValue(s);
}
double MusicPropertiesWidget::playbackSpeed()
{
	return ui->dspinPlaybackSpeed->value();
}
