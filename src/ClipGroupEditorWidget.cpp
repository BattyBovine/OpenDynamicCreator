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



void ClipGroupEditorWidget::addClipGroupEditor(BaseMusicItem *musicitem, float tempo, int beatspermeasure, int beatunit, QAction *playpause, QAction *stop)
{
	if(musicitem) {
		musicitem->stop();
		int row = ui->layoutClips->rowCount();
		MixerWidget *mw = new MixerWidget(musicitem, true, this);
		TimelineWidget *tw = new TimelineWidget(musicitem, tempo, beatspermeasure, beatunit, playpause, stop, true, this);
		connect(mw, SIGNAL(snapChanged(int)), tw, SLOT(setBeatUnitSnapFromCombo(int)));
		ui->layoutClips->addWidget(mw, row, 0);
		ui->layoutClips->addWidget(tw, row, 1);
	}
}
