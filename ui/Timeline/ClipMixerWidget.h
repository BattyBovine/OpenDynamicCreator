#ifndef CLIPMIXERWIDGET_H
#define CLIPMIXERWIDGET_H

#include <QWidget>

namespace Ui {
class ClipMixerWidget;
}

class ClipMixerWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ClipMixerWidget(QWidget *parent = 0);
	~ClipMixerWidget();

private:
	Ui::ClipMixerWidget *ui;
};

#endif // CLIPMIXERWIDGET_H
