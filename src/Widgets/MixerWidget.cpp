#include "Widgets/MixerWidget.h"
#include "ui_MixerWidget.h"

MixerWidget::MixerWidget(ClipContainerPtr clip, bool groupmode, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MixerWidget)
{
	ui->setupUi(this);

	ui->btnMute->setVisible(false);
	ui->btnSolo->setVisible(false);
	ui->sliderPan->setVisible(false);

	connect(ui->comboNoteSnap, SIGNAL(currentIndexChanged(int)), this, SLOT(snapComboChanged(int)));
	connect(ui->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));
	connect(ui->sliderPan, SIGNAL(valueChanged(int)), this, SLOT(panChanged(int)));

	this->attachClipContainer(clip);
	this->setGroupMode(groupmode);
}

MixerWidget::~MixerWidget()
{
	delete ui;
}



void MixerWidget::attachClipContainer(ClipContainerPtr cc)
{
	this->ccClip = cc;
	this->setVolume(this->ccClip->volumedB());
//	this->setPan(this->ccClip->pan());
	ui->labelClipName->setText(this->ccClip->name());
	ui->labelClipName->setToolTip(this->ccClip->name());
//	connect(this->ccClip.get(), SIGNAL(volumeChanged(qreal)), this, SLOT(setVolumeSlider(qreal)));
}

void MixerWidget::setGroupMode(bool g)
{
	ui->labelClipName->setVisible(g);
}

void MixerWidget::volumeChanged(int d)
{
	if(d<=ui->sliderVolume->minimum())
		this->ccClip->setVolume(-200);
	else
		this->ccClip->setVolume(d);
}
void MixerWidget::setVolume(int d)
{
	ui->sliderVolume->setValue(d);
}
int MixerWidget::volume()
{
	return ui->sliderVolume->value();
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
