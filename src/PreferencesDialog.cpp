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
	connect(ui->buttonClearCache, SIGNAL(clicked(bool)), this, SLOT(clearCache()));

	connect(ui->buttonWindowColour, SIGNAL(clicked(bool)), this, SLOT(selectWindowColour()));
	connect(ui->buttonWaveformColour, SIGNAL(clicked(bool)), this, SLOT(selectWaveformColour()));
	connect(ui->buttonPlayMarkerColour, SIGNAL(clicked(bool)), this, SLOT(selectPlayMarkerColour()));
	connect(ui->buttonEventMarkerColour, SIGNAL(clicked(bool)), this, SLOT(selectEventMarkerColour()));

	connect(ui->buttonOK, SIGNAL(clicked(bool)), this, SLOT(close()));
}
PreferencesDialog::~PreferencesDialog()
{
	if(ui->lineTempFolder->text().isEmpty()) {
		ui->lineTempFolder->setText(this->sOldCache);
	} else if(this->bDeleteOldCache) {
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
void PreferencesDialog::clearCache()
{
	QString cache = ui->lineTempFolder->text();
	if(cache.isEmpty())	return;
	switch(QMessageBox::warning(this, tr("Confirm cache clear"), tr("Are you sure you wish to delete <b>%1</b>?").arg(cache), QMessageBox::Yes, QMessageBox::No)) {
	case QMessageBox::Yes:
		QDir cachedir(cache);
		if(cachedir.exists()) {
			ui->buttonOK->setEnabled(false);
			ui->buttonTempFolder->setEnabled(false);
			ui->buttonClearCache->setEnabled(false);
			ui->lineTempFolder->setEnabled(false);
			QApplication::setOverrideCursor(Qt::WaitCursor);
			QApplication::processEvents();
			cachedir.removeRecursively();
			QApplication::restoreOverrideCursor();
			ui->lineTempFolder->setEnabled(true);
			ui->buttonClearCache->setEnabled(true);
			ui->buttonTempFolder->setEnabled(true);
			ui->buttonOK->setEnabled(true);
		}
	}
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
	if(!settings.contains(KEY_PLAY_MARKER_COLOUR))
		settings.setValue(KEY_PLAY_MARKER_COLOUR, QColor(Qt::green));
	if(!settings.contains(KEY_EVENT_MARKER_COLOUR))
		settings.setValue(KEY_EVENT_MARKER_COLOUR, QColor(Qt::red));
}
