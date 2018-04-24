#include "Widgets/ClipMixerWidget.h"
#include "ui_ClipMixerWidget.h"

ClipMixerWidget::ClipMixerWidget(QWidget *parent, ClipItem *ci, bool groupmode) :
	QWidget(parent),
	ui(new Ui::ClipMixerWidget)
{
	ui->setupUi(this);

	this->attachClip(ci);
	ui->labelClipName->setVisible(groupmode);

	connect(ui->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(volumedBChanged(int)));
	connect(ui->sliderPan, SIGNAL(valueChanged(int)), this, SLOT(panChanged(int)));
}

ClipMixerWidget::~ClipMixerWidget()
{
	delete ui;
}



void ClipMixerWidget::attachClip(ClipItem *i)
{
	this->ciClip = i;
	this->setVolumePercent(i->clipVolume());
	this->setPan(i->clipPan());
	ui->labelClipName->setText(this->ciClip->text());
	ui->labelClipName->setToolTip(this->ciClip->text());
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

void ClipMixerWidget::panChanged(int p)
{
	if(this->ciClip)
		this->ciClip->setClipPan(p);
}
void ClipMixerWidget::setPan(int p)
{
	ui->sliderPan->setValue(p);
}
int ClipMixerWidget::pan()
{
	return ui->sliderPan->value();
}