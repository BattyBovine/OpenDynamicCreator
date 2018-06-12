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
	explicit MixerWidget(ClipContainerPtr, bool groupmode=false, QWidget *parent=0);
	~MixerWidget();

	int volume();
	int pan();

public slots:
	void attachClipContainer(ClipContainerPtr);
	void setGroupMode(bool);
	void setVolume(int);
	void setPan(int);

private slots:
	void snapComboChanged(int i) { emit(snapChanged(i)); }
	void volumeChanged(int);
	void panChanged(int);

signals:
	void snapChanged(int);

private:
	Ui::MixerWidget *ui;
	ClipContainerPtr ccClip = NULL;
};

#endif // MIXERWIDGET_H
