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



void ClipGroupEditorWidget::addClipGroupEditor(BaseMusicItem *musicitem, int beatspermeasure)
{
	if(musicitem) {
		int row = ui->layoutClips->rowCount();
		ui->layoutClips->addWidget(new MixerWidget(this, musicitem, true), row, 0);
		ui->layoutClips->addWidget(new TimelineWidget(this, musicitem, beatspermeasure), row, 1);
	}
}
