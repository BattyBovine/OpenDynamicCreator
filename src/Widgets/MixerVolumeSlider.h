#ifndef MIXERVOLUMESLIDER_H
#define MIXERVOLUMESLIDER_H

#include <QSlider>
#include <QLayout>
#include <QToolTip>
#include <QStyle>

class MixerVolumeSlider : public QSlider
{
	Q_OBJECT
public:
	explicit MixerVolumeSlider(QWidget *parent = 0);

private slots:
	void showTip(int);
};

#endif // MIXERVOLUMESLIDER_H
