#ifndef STATEPROPERTIESEDITOR_H
#define STATEPROPERTIESEDITOR_H

#include <QWidget>

namespace Ui {
class StatePropertiesEditor;
}

class StatePropertiesEditor : public QWidget
{
	Q_OBJECT

public:
	explicit StatePropertiesEditor(QWidget *parent = 0);
	~StatePropertiesEditor();

private:
	Ui::StatePropertiesEditor *ui;
};

#endif // STATEPROPERTIESEDITOR_H
