#include "StatePropertiesEditor.h"
#include "ui_StatePropertiesEditor.h"

StatePropertiesEditor::StatePropertiesEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::StatePropertiesEditor)
{
	ui->setupUi(this);
}

StatePropertiesEditor::~StatePropertiesEditor()
{
	delete ui;
}
