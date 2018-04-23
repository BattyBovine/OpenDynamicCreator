#include "Widgets/ClipMixerPanSlider.h"

ClipMixerPanSlider::ClipMixerPanSlider(QWidget *parent) : QSlider(parent)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(sliderConvenience(int)));
}

void ClipMixerPanSlider::mousePressEvent(QMouseEvent *e)
{
	QSlider::mousePressEvent(e);
	if(e->button()==Qt::LeftButton)
		this->bSnapToCentre = true;
}
void ClipMixerPanSlider::mouseReleaseEvent(QMouseEvent *e)
{
	QSlider::mouseReleaseEvent(e);
	if(e->button()==Qt::LeftButton)
		this->bSnapToCentre = false;
}
void ClipMixerPanSlider::mouseDoubleClickEvent(QMouseEvent *e)
{
	QSlider::mouseDoubleClickEvent(e);
	if(e->button()==Qt::LeftButton)
		this->setValue(0);
}



void ClipMixerPanSlider::sliderConvenience(int value)
{
	if(this->bSnapToCentre && (value!=0 && abs(value)<=this->iCentreSnapRange)) {
		this->setValue(0);
		return;
	}
	QMargins margins = this->parentWidget()->layout()->contentsMargins();
	QRect geometry = this->rect();
	int w = geometry.width()-margins.left()-margins.right();
	int h = geometry.height()-margins.top()-margins.bottom();
	QPoint tooltippos(QStyle::sliderPositionFromValue(this->minimum(), this->maximum(), value, w), roundf(h/2.0f));
	QToolTip::showText(this->mapToGlobal(tooltippos), QString("%1%2%").arg((value==0) ? "" : (value<0)?"<":">").arg(abs(value)));
}
