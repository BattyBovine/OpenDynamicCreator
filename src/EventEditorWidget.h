#ifndef EVENTEDITORWIDGET_H
#define EVENTEDITORWIDGET_H

#include <QWidget>

namespace Ui {
class EventEditorWidget;
}

class EventEditorWidget : public QWidget
{
	Q_OBJECT

public:
	explicit EventEditorWidget(QWidget *parent = 0);
	~EventEditorWidget();

private:
	Ui::EventEditorWidget *ui;
};

#endif // EVENTEDITORWIDGET_H
