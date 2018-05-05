#include "Widgets/MixerPanSlider.h"

void MixerPanSlider::keyPressEvent(QKeyEvent *e)
{
	QSlider::keyPressEvent(e);
	this->sliderConvenience(this->value());
}
void MixerPanSlider::mousePressEvent(QMouseEvent *e)
{
	QSlider::mousePressEvent(e);
	this->sliderConvenience(this->value());
}
void MixerPanSlider::mouseMoveEvent(QMouseEvent *e)
{
	if(e->button()==Qt::LeftButton && (abs(this->value())<=this->centerSnapRange()))
		this->setValue(0);
	else
		QSlider::mouseMoveEvent(e);
	this->sliderConvenience(this->value());
}
void MixerPanSlider::mouseReleaseEvent(QMouseEvent *e)
{
	QSlider::mouseReleaseEvent(e);
	if(e->button()==Qt::LeftButton && (abs(this->value())<=this->centerSnapRange()))
		this->setValue(0);
	else
		QSlider::mouseReleaseEvent(e);
}
void MixerPanSlider::mouseDoubleClickEvent(QMouseEvent *e)
{
	QSlider::mouseDoubleClickEvent(e);
	if(e->button()==Qt::LeftButton)
		this->setValue(0);
}



void MixerPanSlider::sliderConvenience(int value)
{
	QMargins margins = this->parentWidget()->layout()->contentsMargins();
	QRect geometry = this->rect();
	int w = geometry.width()-margins.left()-margins.right();
	int h = geometry.height()-margins.top()-margins.bottom();
	QPoint tooltippos(QStyle::sliderPositionFromValue(this->minimum(), this->maximum(), value, w), roundf(h/2.0f));
	QToolTip::showText(this->mapToGlobal(tooltippos), QString("%1%2%").arg((value==0) ? "" : (value<0)?"<":">").arg(abs(value)));
}
