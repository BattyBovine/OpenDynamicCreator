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



void ClipGroupEditorWidget::setClipGroupEditor(ClipContainerPtr clip)
{
	MixerWidget *mw = new MixerWidget(clip, true, this);
	TimelineWidget *tw = new TimelineWidget(clip, true, this);
	connect(mw, SIGNAL(snapChanged(int)), tw, SLOT(setBeatUnitSnapFromCombo(int)));
	ui->layoutClipGroup->addWidget(mw, 0, 0);
	ui->layoutClipGroup->addWidget(tw, 0, 1);
}

void ClipGroupEditorWidget::addClipEditor(ClipContainerPtr clip)
{
	if(clip) {
		int row = ui->layoutClips->rowCount();
		MixerWidget *mw = new MixerWidget(clip, true, this);
		TimelineWidget *tw = new TimelineWidget(clip, true, this);
		connect(mw, SIGNAL(snapChanged(int)), tw, SLOT(setBeatUnitSnapFromCombo(int)));
		ui->layoutClips->addWidget(mw, row, 0);
		ui->layoutClips->addWidget(tw, row, 1);
	}
}
