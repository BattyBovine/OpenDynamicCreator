#include "ClipMixerWidget.h"
#include "ui_ClipMixerWidget.h"

ClipMixerWidget::ClipMixerWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ClipMixerWidget)
{
	ui->setupUi(this);
}

ClipMixerWidget::~ClipMixerWidget()
{
	delete ui;
}
