#include "Widgets/MixerWidget.h"
#include "ui_MixerWidget.h"

MixerWidget::MixerWidget(std::shared_ptr<ClipContainer> clip, bool groupmode, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MixerWidget)
{
	ui->setupUi(this);

	ui->btnMute->setVisible(false);
	ui->btnMute->setVisible(false);

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
}

void MixerWidget::volumedBChanged(int d)
{
	if(d==ui->sliderVolume->minimum())
		this->ccClip->setVolume(0.0f);
	else
		this->ccClip->setVolume(QAudio::convertVolume(d, QAudio::DecibelVolumeScale, QAudio::LogarithmicVolumeScale));
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
	ui->sliderVolume->setValue(QAudio::convertVolume(v, QAudio::LogarithmicVolumeScale, QAudio::DecibelVolumeScale));
}
float MixerWidget::volumePercent()
{
	if(ui->sliderVolume->value()==ui->sliderVolume->minimum())
		return 0.0f;
	else
		return QAudio::convertVolume(ui->sliderVolume->value(), QAudio::DecibelVolumeScale, QAudio::LogarithmicVolumeScale);
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
