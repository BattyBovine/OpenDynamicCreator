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
	explicit ClipMixerWidget(QWidget *parent = 0);
	~ClipMixerWidget();

	int volumedB();
	int volumePercent();

public slots:
	void attachClip(ClipItem *i) { this->ciClip = i; this->setVolumePercent(i->clipVolume()); }
	void volumedBChanged(int);
	void setVolumedB(int);
	void setVolumePercent(int);

private:
	inline int dBToVolume(int d) { return roundf(powf(10.0f, 0.05f * d) * 100); }
	inline int volumeTodB(int v) { return roundf(20.0f * log10f(v/100.0f)); }

	Ui::ClipMixerWidget *ui;
	ClipItem *ciClip = NULL;
};

#endif // CLIPMIXERWIDGET_H
