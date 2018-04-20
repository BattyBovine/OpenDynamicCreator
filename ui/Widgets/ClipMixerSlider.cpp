#include "ClipMixerSlider.h"

ClipMixerSlider::ClipMixerSlider(QWidget *parent) : QSlider(parent)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(showTip(int)));
}



void ClipMixerSlider::showTip(int value)
{
	QMargins margins = this->parentWidget()->layout()->contentsMargins();
	QRect geometry = this->geometry();
	int w = geometry.width()-margins.left()-margins.right();
	int h = geometry.height()-margins.top()-margins.bottom();
	switch(this->orientation()) {
	case Qt::Horizontal:	// Pan
		QToolTip::showText(this->mapToGlobal(QPoint(QStyle::sliderPositionFromValue(this->minimum(), this->maximum(), value, w), roundf(h/2.0f))), QString("%1").arg(value));
		break;
	case Qt::Vertical:		// Volume
		QToolTip::showText(this->mapToGlobal(QPoint(roundf(w/2.0f), h-QStyle::sliderPositionFromValue(this->minimum(), this->maximum(), value, h))), (value==this->minimum()) ? QString("-inf") : QString("%1%2dB").arg((value>=0?"+":"")).arg(value));
		break;
	}
}
