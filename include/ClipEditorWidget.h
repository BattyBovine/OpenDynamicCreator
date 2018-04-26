#ifndef CLIPEDITORWIDGET_H
#define CLIPEDITORWIDGET_H

#include <QWidget>

#include "Widgets/MusicTreeView.h"
#include "Widgets/MixerWidget.h"
#include "Widgets/TimelineWidget.h"

namespace Ui {
class ClipEditorWidget;
}

class ClipEditorWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ClipEditorWidget(QWidget *parent = 0);
	~ClipEditorWidget();

	void setClipEditor(BaseMusicItem*, float, int, int);

private:
	Ui::ClipEditorWidget *ui;
};

#endif // CLIPEDITORWIDGET_H
