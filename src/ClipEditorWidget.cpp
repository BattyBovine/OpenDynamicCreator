#include "ClipEditorWidget.h"
#include "ui_ClipEditorWidget.h"

ClipEditorWidget::ClipEditorWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ClipEditorWidget)
{
	ui->setupUi(this);
}

ClipEditorWidget::~ClipEditorWidget()
{
	delete ui;
}



void ClipEditorWidget::setClipEditor(ClipItem *clip)
{
	if(clip) {
		ui->layoutClipEditor->addWidget(new ClipMixerWidget(clip, true), 0, 0);
		ui->layoutClipEditor->addWidget(new TimelineWidget(), 0, 1);
	}
}
