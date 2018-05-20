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

	void setClipEditor(std::shared_ptr<ClipContainer>);

private:
	Ui::ClipEditorWidget *ui;

	MixerWidget *widgetMixer = NULL;
	TimelineWidget *widgetTimeline = NULL;
};

#endif // CLIPEDITORWIDGET_H
