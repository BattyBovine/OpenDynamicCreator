#ifndef MIXERWIDGET_H
#define MIXERWIDGET_H

#include <QWidget>

#include <Widgets/MusicTreeView.h>

namespace Ui {
class MixerWidget;
}

class MixerWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MixerWidget(QWidget *parent = 0, BaseMusicItem *musicitem=NULL, bool groupmode=false);
	~MixerWidget();

	int volumedB();
	float volumePercent();
	int pan();

public slots:
	void attachMusicItem(BaseMusicItem*);
	void setVolumedB(int);
	void setVolumePercent(float);
	void setPan(int);

private slots:
	void volumedBChanged(int);
	void panChanged(int);

private:
	inline float dBToVolume(int d) { return powf(10.0f, 0.05f * d); }
	inline int volumeTodB(float v) { return roundf(20.0f * log10f(v)); }

	Ui::MixerWidget *ui;
	BaseMusicItem *bmiMusicItem = NULL;
};

#endif // MIXERWIDGET_H
