#ifndef CLIPMIXERSLIDER_H
#define CLIPMIXERSLIDER_H

#include <QWidget>
#include <QLayout>
#include <QSlider>
#include <QToolTip>
#include <QStyle>

class ClipMixerSlider : public QSlider
{
	Q_OBJECT
public:
	explicit ClipMixerSlider(QWidget *parent = 0);

private slots:
	void showTip(int);
};

#endif // CLIPMIXERSLIDER_H
