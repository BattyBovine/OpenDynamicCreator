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

void ClipEditorWidget::setClipEditor(ClipContainerPtr clip)
{
	if(clip) {
		this->widgetMixer = new MixerWidget(clip, false, this);
		this->widgetTimeline = new TimelineWidget(clip, false, this);
		connect(this->widgetMixer, SIGNAL(snapChanged(int)), this->widgetTimeline, SLOT(setBeatUnitSnapFromCombo(int)));
		ui->layoutClipEditor->addWidget(this->widgetMixer, 0, 0);
		ui->layoutClipEditor->addWidget(this->widgetTimeline, 0, 1);
	}
}
