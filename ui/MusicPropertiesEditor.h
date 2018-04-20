#ifndef MUSICPROPERTIESEDITOR_H
#define MUSICPROPERTIESEDITOR_H

#include <QWidget>

namespace Ui {
class MusicPropertiesEditor;
}

class MusicPropertiesEditor : public QWidget
{
	Q_OBJECT

public:
	explicit MusicPropertiesEditor(QWidget *parent = 0);
	~MusicPropertiesEditor();

private:
	Ui::MusicPropertiesEditor *ui;
};

#endif // MUSICPROPERTIESEDITOR_H
