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



void ClipGroupEditorWidget::addClipGroupEditor(ClipItem *clip, float tempo, int beatspermeasure, int beatunit, QAction *playpause, QAction *stop)
{
	if(clip) {
		clip->stop();
		int row = ui->layoutClips->rowCount();
		MixerWidget *mw = new MixerWidget(clip, true, this);
		TimelineWidget *tw = new TimelineWidget(clip, tempo, beatspermeasure, beatunit, playpause, stop, true, this);
		connect(mw, SIGNAL(snapChanged(int)), tw, SLOT(setBeatUnitSnapFromCombo(int)));
		ui->layoutClips->addWidget(mw, row, 0);
		ui->layoutClips->addWidget(tw, row, 1);
	}
}
