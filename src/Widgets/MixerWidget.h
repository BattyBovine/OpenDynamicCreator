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

	void setVolumeSlider(qreal);

signals:
	void snapChanged(int);

private:
	Ui::MixerWidget *ui;
	std::shared_ptr<ClipContainer> ccClip = NULL;
};

#endif // MIXERWIDGET_H
