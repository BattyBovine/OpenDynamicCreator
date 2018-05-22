#include "Widgets/MixerWidget.h"
#include "ui_MixerWidget.h"

MixerWidget::MixerWidget(std::shared_ptr<ClipContainer> clip, bool groupmode, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MixerWidget)
{
	ui->setupUi(this);

	connect(ui->comboNoteSnap, SIGNAL(currentIndexChanged(int)), this, SLOT(snapComboChanged(int)));
	connect(ui->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(volumedBChanged(int)));
	connect(ui->sliderPan, SIGNAL(valueChanged(int)), this, SLOT(panChanged(int)));

	this->attachClipContainer(clip);
	this->setGroupMode(groupmode);
}

MixerWidget::~MixerWidget()
{
	delete ui;
}



void MixerWidget::attachClipContainer(std::shared_ptr<ClipContainer> c)
{
	this->ccClip = c;
	this->setVolumePercent(this->ccClip->volume());
//	this->setPan(this->ccClip->pan());
	ui->labelClipName->setText(this->ccClip->name());
	ui->labelClipName->setToolTip(this->ccClip->name());
}

void MixerWidget::setGroupMode(bool g)
{
	ui->labelClipName->setVisible(g);
	ui->btnMute->setVisible(g);
	ui->btnSolo->setVisible(g);
}

void MixerWidget::volumedBChanged(int d)
{
	this->ccClip->setVolume(QAudio::convertVolume(d, QAudio::DecibelVolumeScale, QAudio::LinearVolumeScale));
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
	ui->sliderVolume->setValue(QAudio::convertVolume(v, QAudio::LinearVolumeScale, QAudio::DecibelVolumeScale));
}
float MixerWidget::volumePercent()
{
	return QAudio::convertVolume(ui->sliderVolume->value(), QAudio::DecibelVolumeScale, QAudio::LinearVolumeScale);
}

void MixerWidget::panChanged(int/* p*/)
{
//	this->ccClip->setPan(p);
}
void MixerWidget::setPan(int p)
{
	ui->sliderPan->setValue(p);
}
int MixerWidget::pan()
{
	return ui->sliderPan->value();
}
