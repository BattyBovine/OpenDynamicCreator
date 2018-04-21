#include "ClipMixerWidget.h"
#include "ui_ClipMixerWidget.h"

ClipMixerWidget::ClipMixerWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ClipMixerWidget)
{
	ui->setupUi(this);

	connect(ui->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(volumedBChanged(int)));
}

ClipMixerWidget::~ClipMixerWidget()
{
	delete ui;
}



void ClipMixerWidget::volumedBChanged(int d)
{
	if(this->ciClip)
		this->ciClip->setClipVolume(this->dBToVolume(d));
}

void ClipMixerWidget::setVolumedB(int d)
{
	ui->sliderVolume->setValue(d);
}
int ClipMixerWidget::volumedB()
{
	return ui->sliderVolume->value();
}

void ClipMixerWidget::setVolumePercent(int v)
{
	ui->sliderVolume->setValue(this->volumeTodB(v));
}
int ClipMixerWidget::volumePercent()
{
	return this->dBToVolume(ui->sliderVolume->value());
}
