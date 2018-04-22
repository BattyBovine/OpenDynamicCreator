#ifndef CLIPEDITORWIDGET_H
#define CLIPEDITORWIDGET_H

#include <QWidget>

#include "src/qt/MusicTreeView.h"
#include "ui/Timeline/ClipMixerWidget.h"

namespace Ui {
class ClipEditorWidget;
}

class ClipEditorWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ClipEditorWidget(QWidget *parent = 0);
	~ClipEditorWidget();

	void setClipEditor(ClipItem *clip);

private:
	Ui::ClipEditorWidget *ui;
};

#endif // CLIPEDITORWIDGET_H
