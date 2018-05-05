#ifndef MIXERPANSLIDER_H
#define MIXERPANSLIDER_H

#include <QSlider>
#include <QLayout>
#include <QToolTip>
#include <QStyle>
#include <QMouseEvent>

class MixerPanSlider : public QSlider
{
	Q_OBJECT
	Q_PROPERTY(int centerSnapRange READ centerSnapRange WRITE setCenterSnapRange)

public:
	explicit MixerPanSlider(QWidget *parent = 0) : QSlider(parent){}

	virtual void keyPressEvent(QKeyEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
	virtual void mouseDoubleClickEvent(QMouseEvent*);

	int centerSnapRange() { return this->iCentreSnapRange; }

private slots:
	void setCenterSnapRange(int s) { this->iCentreSnapRange=s; }
	void sliderConvenience(int);

private:
	int iCentreSnapRange = 0;
};

#endif // MIXERPANSLIDER_H
