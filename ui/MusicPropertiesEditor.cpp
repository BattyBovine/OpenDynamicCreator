#include "MusicPropertiesEditor.h"
#include "ui_musicpropertieseditor.h"

MusicPropertiesEditor::MusicPropertiesEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MusicPropertiesEditor)
{
	ui->setupUi(this);
}

MusicPropertiesEditor::~MusicPropertiesEditor()
{
	delete ui;
}
