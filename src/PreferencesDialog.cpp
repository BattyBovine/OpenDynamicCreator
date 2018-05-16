#include "src\PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog)
{
	ui->setupUi(this);

	ui->labelDirectoryError->setVisible(false);
	this->sOldCache = this->getTempFolder();
	ui->lineTempFolder->setText(this->sOldCache);
	connect(ui->lineTempFolder, SIGNAL(textChanged(QString)), this, SLOT(saveTempFolder(QString)));
	connect(ui->buttonTempFolder, SIGNAL(clicked(bool)), this, SLOT(selectTempFolder()));

	connect(ui->buttonWindowColour, SIGNAL(clicked(bool)), this, SLOT(selectWindowColour()));
	connect(ui->buttonWaveformColour, SIGNAL(clicked(bool)), this, SLOT(selectWaveformColour()));

	connect(ui->buttonOK, SIGNAL(clicked(bool)), this, SLOT(close()));
}
PreferencesDialog::~PreferencesDialog()
{
	if(this->bDeleteOldCache) {
		QDir oldcache(this->sOldCache);
		oldcache.removeRecursively();
	}
	delete ui;
}

void PreferencesDialog::selectTempFolder()
{
	ui->lineTempFolder->setText(QFileDialog::getExistingDirectory(this, tr("Select folder"),
																  this->settings.value(KEY_TEMP_FOLDER).toString()));
}
void PreferencesDialog::saveTempFolder(QString d)
{
	ui->labelDirectoryError->setVisible(false);
	this->bDeleteOldCache = true;
	if(d.isEmpty()) {
		ui->labelDirectoryError->setText(tr("Directory cannot be blank"));
		ui->labelDirectoryError->setVisible(true);
		return;
	}
	QDir temp(d);
	if(!temp.exists()) {
		ui->labelDirectoryError->setText(tr("Directory does not exist; it will be created"));
		ui->labelDirectoryError->setVisible(true);
	}
	this->settings.setValue(KEY_TEMP_FOLDER, d);
}



void PreferencesDialog::initSettings()
{
	QSettings settings;
	if(!settings.contains(KEY_TEMP_FOLDER))
		settings.setValue(KEY_TEMP_FOLDER, QString("%1/%2").arg(QDir::tempPath()).arg(QCoreApplication::applicationName()));
	if(!settings.contains(KEY_WINDOW_COLOUR))
		settings.setValue(KEY_WINDOW_COLOUR, QColor(Qt::white));
	if(!settings.contains(KEY_WAVEFORM_COLOUR))
		settings.setValue(KEY_WAVEFORM_COLOUR, QColor(Qt::blue));
}
