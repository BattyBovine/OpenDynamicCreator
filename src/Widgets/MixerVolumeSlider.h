#ifndef MIXERVOLUMESLIDER_H
#define MIXERVOLUMESLIDER_H

#include <QSlider>
#include <QLayout>
#include <QSpinBox>
#include <QMouseEvent>
#include <QToolTip>
#include <QStyle>

class MixerVolumeSlider : public QSlider
{
	Q_OBJECT
public:
	explicit MixerVolumeSlider(QWidget *parent = 0);

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent*);

private slots:
	void showTip(int);
	void setDecibelValue();
};

#endif // MIXERVOLUMESLIDER_H
