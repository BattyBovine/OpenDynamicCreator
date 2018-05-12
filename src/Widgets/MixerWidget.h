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
	explicit MixerWidget(std::shared_ptr<ClipContainer>, bool groupmode=false, QWidget *parent=0);
	~MixerWidget();

	int volumedB();
	float volumePercent();
	int pan();

public slots:
	void attachClipContainer(std::shared_ptr<ClipContainer>);
	void setGroupMode(bool);
	void setVolumedB(int);
	void setVolumePercent(float);
	void setPan(int);

private slots:
	void snapComboChanged(int i) { emit(snapChanged(i)); }
	void volumedBChanged(int);
	void panChanged(int);

signals:
	void snapChanged(int);

private:
	inline float dBToVolume(int d) { return powf(10.0f, 0.05f * d); }
	inline int volumeTodB(float v) { return roundf(20.0f * log10f(v)); }

	Ui::MixerWidget *ui;
	std::shared_ptr<ClipContainer> ccClip = NULL;
};

#endif // MIXERWIDGET_H
