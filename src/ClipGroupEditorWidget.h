#ifndef CLIPGROUPEDITORWIDGET_H
#define CLIPGROUPEDITORWIDGET_H

#include <QScrollArea>

#include "Widgets/MusicTreeView.h"
#include "Widgets/MixerWidget.h"
#include "Widgets/GraphicsItems/TimelineWidget.h"

namespace Ui {
class ClipGroupEditorWidget;
}

class ClipGroupEditorWidget : public QScrollArea
{
	Q_OBJECT

public:
	explicit ClipGroupEditorWidget(QWidget *parent = 0);
	~ClipGroupEditorWidget();

	void addClipGroupEditor(ClipItem*, float, int, int, QAction*, QAction*);

private:
	Ui::ClipGroupEditorWidget *ui;
};

#endif // CLIPGROUPEDITORWIDGET_H
