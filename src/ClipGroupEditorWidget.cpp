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



void ClipGroupEditorWidget::addClipGroupEditor(TrackItem *track, ClipItem *clip)
{
	if(clip) {
		int row = ui->layoutClips->rowCount();
		ui->layoutClips->addWidget(new ClipMixerWidget(clip, true), row, 0);
		ui->layoutClips->addWidget(new TimelineWidget(track, clip), row, 1);
	}
}
