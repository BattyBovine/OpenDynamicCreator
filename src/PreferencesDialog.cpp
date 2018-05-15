#include "src\PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog)
{
	ui->setupUi(this);

	ui->labelDirectoryDoesNotExist->setVisible(false);
	this->sOldCache = this->getTempFolder();
	ui->lineTempFolder->setText(this->sOldCache);
	connect(ui->lineTempFolder, SIGNAL(textChanged(QString)), this, SLOT(saveTempFolder(QString)));
}
PreferencesDialog::~PreferencesDialog()
{
	if(this->bDeleteOldCache) {
		QDir oldcache(this->sOldCache);
		qDebug() << oldcache.absolutePath();
	}
	delete ui;
}

void PreferencesDialog::saveTempFolder(QString d)
{
	this->bDeleteOldCache = true;
	QDir temp(d);
	ui->labelDirectoryDoesNotExist->setVisible(!temp.exists());
	this->settings.setValue(KEY_TEMP_FOLDER, d);
}
