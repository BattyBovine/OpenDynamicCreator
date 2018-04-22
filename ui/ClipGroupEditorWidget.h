#ifndef CLIPGROUPEDITORWIDGET_H
#define CLIPGROUPEDITORWIDGET_H

#include <QScrollArea>

#include "src/qt/MusicTreeView.h"
#include "ui/Timeline/ClipMixerWidget.h"

namespace Ui {
class ClipGroupEditorWidget;
}

class ClipGroupEditorWidget : public QScrollArea
{
	Q_OBJECT

public:
	explicit ClipGroupEditorWidget(QWidget *parent = 0);
	~ClipGroupEditorWidget();

	void addClipGroupEditor(ClipItem*);

private:
	Ui::ClipGroupEditorWidget *ui;
};

#endif // CLIPGROUPEDITORWIDGET_H
