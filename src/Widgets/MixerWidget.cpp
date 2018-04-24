#include "Widgets/MixerWidget.h"
#include "ui_MixerWidget.h"

MixerWidget::MixerWidget(QWidget *parent, BaseMusicItem *musicitem, bool groupmode) :
	QWidget(parent),
	ui(new Ui::MixerWidget)
{
	ui->setupUi(this);

	this->attachMusicItem(musicitem);
	ui->labelClipName->setVisible(groupmode);

	connect(ui->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(volumedBChanged(int)));
	connect(ui->sliderPan, SIGNAL(valueChanged(int)), this, SLOT(panChanged(int)));
}

MixerWidget::~MixerWidget()
{
	delete ui;
}



void MixerWidget::attachMusicItem(BaseMusicItem *i)
{
	this->bmiMusicItem = i;
	this->setVolumePercent(i->volume());
	this->setPan(i->pan());
	ui->labelClipName->setText(this->bmiMusicItem->text());
	ui->labelClipName->setToolTip(this->bmiMusicItem->text());
}

void MixerWidget::volumedBChanged(int d)
{
	if(this->bmiMusicItem)
		this->bmiMusicItem->setVolume(this->dBToVolume(d));
}
void MixerWidget::setVolumedB(int d)
{
	ui->sliderVolume->setValue(d);
}
int MixerWidget::volumedB()
{
	return ui->sliderVolume->value();
}

void MixerWidget::setVolumePercent(float v)
{
	ui->sliderVolume->setValue(this->volumeTodB(v));
}
float MixerWidget::volumePercent()
{
	return this->dBToVolume(ui->sliderVolume->value());
}

void MixerWidget::panChanged(int p)
{
	if(this->bmiMusicItem)
		this->bmiMusicItem->setPan(p);
}
void MixerWidget::setPan(int p)
{
	ui->sliderPan->setValue(p);
}
int MixerWidget::pan()
{
	return ui->sliderPan->value();
}
