#include "ClipMixerWidget.h"
#include "ui_ClipMixerWidget.h"

ClipMixerWidget::ClipMixerWidget(ClipItem *ci, bool groupmode, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ClipMixerWidget)
{
	ui->setupUi(this);

	this->attachClip(ci);
	ui->labelClipName->setVisible(groupmode);

	connect(ui->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(volumedBChanged(int)));
}

ClipMixerWidget::~ClipMixerWidget()
{
	delete ui;
}



void ClipMixerWidget::attachClip(ClipItem *i)
{
	this->ciClip = i;
	this->setVolumePercent(i->clipVolume());
	ui->labelClipName->setText(this->ciClip->text());
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

void ClipMixerWidget::setVolumePercent(float v)
{
	ui->sliderVolume->setValue(this->volumeTodB(v));
}
float ClipMixerWidget::volumePercent()
{
	return this->dBToVolume(ui->sliderVolume->value());
}
