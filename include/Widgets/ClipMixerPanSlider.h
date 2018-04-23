#ifndef CLIPMIXERPANSLIDER_H
#define CLIPMIXERPANSLIDER_H

#include <QSlider>
#include <QLayout>
#include <QToolTip>
#include <QStyle>
#include <QMouseEvent>

class ClipMixerPanSlider : public QSlider
{
	Q_OBJECT
	Q_PROPERTY(int centerSnapRange READ centerSnapRange WRITE setCenterSnapRange)

public:
	explicit ClipMixerPanSlider(QWidget *parent = 0);

	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
	virtual void mouseDoubleClickEvent(QMouseEvent*);

	int centerSnapRange() { return this->iCentreSnapRange; }

private slots:
	void setCenterSnapRange(int s) { this->iCentreSnapRange=s; }
	void sliderConvenience(int);

private:
	int iCentreSnapRange = 0;
	bool bSnapToCentre = false;
};

#endif // CLIPMIXERPANSLIDER_H
