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

void ClipEditorWidget::setClipEditor(std::shared_ptr<ClipContainer> clip, QAction *playpause, QAction *stop)
{
	if(clip) {
		clip->stop();
		this->widgetMixer = new MixerWidget(clip, false, this);
		this->widgetTimeline = new TimelineWidget(clip, playpause, stop, false, this);
		connect(this->widgetMixer, SIGNAL(snapChanged(int)), this->widgetTimeline, SLOT(setBeatUnitSnapFromCombo(int)));
		ui->layoutClipEditor->addWidget(this->widgetMixer, 0, 0);
		ui->layoutClipEditor->addWidget(this->widgetTimeline, 0, 1);
	}
}
