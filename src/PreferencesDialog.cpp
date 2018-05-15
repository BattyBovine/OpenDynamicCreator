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
}
PreferencesDialog::~PreferencesDialog()
{
	if(this->bDeleteOldCache) {
		QDir oldcache(this->sOldCache);
		oldcache.removeRecursively();
	}
	delete ui;
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
	this->saveTempFolder(this->getTempFolder());
}
