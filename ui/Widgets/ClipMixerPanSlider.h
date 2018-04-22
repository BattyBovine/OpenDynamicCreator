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
public:
	explicit ClipMixerPanSlider(QWidget *parent = 0);

	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);

private slots:
	void sliderConvenience(int);

private:
	bool bSnapToCentre = false;
};

#endif // CLIPMIXERPANSLIDER_H
