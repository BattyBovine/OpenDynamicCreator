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



void ClipEditorWidget::setClipEditor(BaseMusicItem *musicitem, float tempo, int beatspermeasure, int beatunit)
{
	if(musicitem) {
		MixerWidget *mw = new MixerWidget(musicitem, false, this);
		TimelineWidget *tw = new TimelineWidget(musicitem, tempo, beatspermeasure, beatunit, this);
		connect(mw, SIGNAL(snapChanged(int)), tw, SLOT(setBeatUnitSnapFromCombo(int)));
		ui->layoutClipEditor->addWidget(mw, 0, 0);
		ui->layoutClipEditor->addWidget(tw, 0, 1);
	}
}
