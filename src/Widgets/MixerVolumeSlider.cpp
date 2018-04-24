#include "Widgets/MixerVolumeSlider.h"

MixerVolumeSlider::MixerVolumeSlider(QWidget *parent) : QSlider(parent)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(showTip(int)));
}



void MixerVolumeSlider::showTip(int value)
{
	QMargins margins = this->parentWidget()->layout()->contentsMargins();
	QRect geometry = this->geometry();
	int w = geometry.width()-margins.left()-margins.right();
	int h = geometry.height()-margins.top()-margins.bottom();
	QPoint tooltippos(roundf(w/2.0f), h-QStyle::sliderPositionFromValue(this->minimum(), this->maximum(), value, h)+8);
	QToolTip::showText(this->mapToGlobal(tooltippos), (value==this->minimum()) ? QString("-inf") : QString("%1%2dB").arg((value>=0?"+":"")).arg(value));
}
