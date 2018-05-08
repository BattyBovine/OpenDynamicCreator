#ifndef CLIPEDITORWIDGET_H
#define CLIPEDITORWIDGET_H

#include <QWidget>

#include "Widgets/MusicTreeView.h"
#include "Widgets/MixerWidget.h"
#include "Widgets/GraphicsItems/TimelineWidget.h"

namespace Ui {
class ClipEditorWidget;
}

class ClipEditorWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ClipEditorWidget(QWidget *parent = 0);
	~ClipEditorWidget();

	void setClipEditor(ClipItem*, float, int, int, QAction*, QAction*);

private:
	Ui::ClipEditorWidget *ui;

	MixerWidget *widgetMixer = NULL;
	TimelineWidget *widgetTimeline = NULL;
};

#endif // CLIPEDITORWIDGET_H
