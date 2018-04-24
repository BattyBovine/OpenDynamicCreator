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



void ClipEditorWidget::setClipEditor(ClipItem *clip, int beatspermeasure)
{
	if(clip) {
		ui->layoutClipEditor->addWidget(new MixerWidget(this, clip), 0, 0);
		ui->layoutClipEditor->addWidget(new TimelineWidget(this, clip, beatspermeasure), 0, 1);
	}
}
