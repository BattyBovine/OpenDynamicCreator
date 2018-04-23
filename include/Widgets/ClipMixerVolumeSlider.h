#ifndef CLIPMIXERVOLUMESLIDER_H
#define CLIPMIXERVOLUMESLIDER_H

#include <QSlider>
#include <QLayout>
#include <QToolTip>
#include <QStyle>

class ClipMixerVolumeSlider : public QSlider
{
	Q_OBJECT
public:
	explicit ClipMixerVolumeSlider(QWidget *parent = 0);

private slots:
	void showTip(int);
};

#endif // CLIPMIXERVOLUMESLIDER_H
