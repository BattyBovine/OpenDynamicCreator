#include "ClipGroupEditorWidget.h"
#include "ui_ClipGroupEditorWidget.h"

ClipGroupEditorWidget::ClipGroupEditorWidget(QWidget *parent) :
	QScrollArea(parent),
	ui(new Ui::ClipGroupEditorWidget)
{
	ui->setupUi(this);
}

ClipGroupEditorWidget::~ClipGroupEditorWidget()
{
	delete ui;
}



void ClipGroupEditorWidget::addClipGroupEditor(ClipItem *clip)
{
	if(clip) ui->layoutClipGroupEditor->insertWidget(ui->layoutClipGroupEditor->count()-1, new ClipMixerWidget(clip, true));
}
