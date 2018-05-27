#include "src\EventEditorWidget.h"
#include "ui_EventEditorWidget.h"

EventEditorWidget::EventEditorWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::EventEditorWidget)
{
	ui->setupUi(this);
}

EventEditorWidget::~EventEditorWidget()
{
	delete ui;
}
