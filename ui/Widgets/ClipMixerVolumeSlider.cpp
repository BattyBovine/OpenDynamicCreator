#include "ClipMixerVolumeSlider.h"

ClipMixerVolumeSlider::ClipMixerVolumeSlider(QWidget *parent) : QSlider(parent)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(showTip(int)));
}



void ClipMixerVolumeSlider::showTip(int value)
{
	QMargins margins = this->parentWidget()->layout()->contentsMargins();
	QRect geometry = this->geometry();
	int w = geometry.width()-margins.left()-margins.right();
	int h = geometry.height()-margins.top()-margins.bottom();
	switch(this->orientation()) {
	case Qt::Horizontal:	// Pan
	{
		QPoint tooltippos(QStyle::sliderPositionFromValue(this->minimum(), this->maximum(), value, w), roundf(h/2.0f));
		QToolTip::showText(this->mapToGlobal(tooltippos), QString("%1%2%").arg((value==0) ? "" : (value<0)?"<":">").arg(abs(value)));
	}	break;
	case Qt::Vertical:		// Volume
	{
		QPoint tooltippos(roundf(w/2.0f), h-QStyle::sliderPositionFromValue(this->minimum(), this->maximum(), value, h));
		QToolTip::showText(this->mapToGlobal(tooltippos), (value==this->minimum()) ? QString("-inf") : QString("%1%2dB").arg((value>=0?"+":"")).arg(value));
	}	break;
	}
}
