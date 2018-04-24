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



void ClipGroupEditorWidget::addClipGroupEditor(ClipItem *clip, int beatspermeasure)
{
	if(clip) {
		int row = ui->layoutClips->rowCount();
		ui->layoutClips->addWidget(new ClipMixerWidget(this, clip, true), row, 0);
		ui->layoutClips->addWidget(new TimelineWidget(this, clip, beatspermeasure), row, 1);
	}
}
