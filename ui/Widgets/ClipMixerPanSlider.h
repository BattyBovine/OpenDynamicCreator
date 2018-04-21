#ifndef CLIPMIXERPANSLIDER_H
#define CLIPMIXERPANSLIDER_H

#include <QSlider>
#include <QLayout>
#include <QToolTip>
#include <QStyle>

class ClipMixerPanSlider : public QSlider
{
	Q_OBJECT
public:
	explicit ClipMixerPanSlider(QWidget *parent = 0);

private slots:
	void showTip(int);
};

#endif // CLIPMIXERPANSLIDER_H
