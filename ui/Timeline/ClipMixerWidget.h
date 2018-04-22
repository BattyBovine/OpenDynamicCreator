#ifndef CLIPMIXERWIDGET_H
#define CLIPMIXERWIDGET_H

#include <QWidget>

#include <src/qt/MusicTreeView.h>

namespace Ui {
class ClipMixerWidget;
}

class ClipMixerWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ClipMixerWidget(ClipItem *ci=NULL, bool groupmode=false, QWidget *parent = 0);
	~ClipMixerWidget();

	int volumedB();
	float volumePercent();

public slots:
	void attachClip(ClipItem*);
	void volumedBChanged(int);
	void setVolumedB(int);
	void setVolumePercent(float);

private:

	inline float dBToVolume(int d) { return powf(10.0f, 0.05f * d) * 100; }
	inline int volumeTodB(float v) { return roundf(20.0f * log10f(v/100.0f)); }

	Ui::ClipMixerWidget *ui;
	ClipItem *ciClip = NULL;
};

#endif // CLIPMIXERWIDGET_H
