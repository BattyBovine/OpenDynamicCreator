#include "ClipMixerPanSlider.h"

ClipMixerPanSlider::ClipMixerPanSlider(QWidget *parent) : QSlider(parent)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(showTip(int)));
}



void ClipMixerPanSlider::showTip(int value)
{
	QMargins margins = this->parentWidget()->layout()->contentsMargins();
	QRect geometry = this->geometry();
	int w = geometry.width()-margins.left()-margins.right();
	int h = geometry.height()-margins.top()-margins.bottom();
	QPoint tooltippos(QStyle::sliderPositionFromValue(this->minimum(), this->maximum(), value, w), roundf(h/2.0f));
	QToolTip::showText(this->mapToGlobal(tooltippos), QString("%1%2%").arg((value==0) ? "" : (value<0)?"<":">").arg(abs(value)));
}
