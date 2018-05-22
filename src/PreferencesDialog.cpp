#include "src\PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog)
{
	ui->setupUi(this);

	this->getOutputDeviceList();
	ui->comboOutputDevice->setCurrentIndex(this->settings.value(KEY_OUTPUT_DEVICE).toInt());
	connect(ui->comboOutputDevice, SIGNAL(currentIndexChanged(int)), this, SLOT(saveOutputDevice(int)));

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

	this->getCacheSize();
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

void PreferencesDialog::getOutputDeviceList()
{
	QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
	foreach(QAudioDeviceInfo device, devices)
		ui->comboOutputDevice->addItem(device.deviceName());
}

void PreferencesDialog::selectTempFolder()
{
	QString newdir = QFileDialog::getExistingDirectory(this, tr("Select folder"),
													   this->settings.value(KEY_TEMP_FOLDER).toString());
	if(!newdir.isEmpty())
		ui->lineTempFolder->setText(newdir);
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

void PreferencesDialog::getCacheSize()
{
	ui->labelCacheSize->setText(tr("Getting cache size..."));
	QThread *cachesizethread = new QThread();
	CacheSizeWorker *worker = new CacheSizeWorker(ui->lineTempFolder->text());
	worker->moveToThread(cachesizethread);
	connect(cachesizethread, SIGNAL(started()), worker, SLOT(start()));
	connect(worker, SIGNAL(finished(quint64)), this, SLOT(retrieveCacheSize(quint64)));
	connect(worker, SIGNAL(finished(quint64)), worker, SLOT(deleteLater()));
	connect(cachesizethread, SIGNAL(finished()), cachesizethread, SLOT(deleteLater()));
	cachesizethread->start();
}
void PreferencesDialog::retrieveCacheSize(quint64 size)
{
	float sizekb = size / 1024.0f;
	float sizemb = sizekb / 1024.0f;
	float sizegb = sizemb / 1024.0f;
	if(sizegb>1.0f)
		ui->labelCacheSize->setText(QString("%1 GB").arg(sizegb));
	else if(sizemb>1.0f)
		ui->labelCacheSize->setText(QString("%1 MB").arg(sizemb));
	else if(sizekb>1.0f)
		ui->labelCacheSize->setText(QString("%1 kB").arg(sizekb));
	else
		ui->labelCacheSize->setText(QString("%1 bytes").arg(size));
}

void PreferencesDialog::clearCache()
{
	if(this->iWorkerLock>0)
		return;
	QString cache = ui->lineTempFolder->text();
	if(cache.isEmpty())	return;
	switch(QMessageBox::warning(this, tr("Confirm cache clear"), tr("Are you sure you wish to delete <b>%1</b>?").arg(cache), QMessageBox::Yes, QMessageBox::No)) {
	case QMessageBox::Yes:
		this->iWorkerLock++;
		this->cacheFunctionsEnabled(false);
		QApplication::setOverrideCursor(Qt::WaitCursor);
		QThread *cacheclearthread = new QThread();
		CacheClearWorker *worker = new CacheClearWorker(ui->lineTempFolder->text());
		worker->moveToThread(cacheclearthread);
		connect(cacheclearthread, SIGNAL(started()), worker, SLOT(start()));
		connect(worker, SIGNAL(finished()), this, SLOT(clearCacheResult()));
		connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
		connect(cacheclearthread, SIGNAL(finished()), cacheclearthread, SLOT(deleteLater()));
		cacheclearthread->start();
		break;
	}
}
void PreferencesDialog::clearCacheResult()
{
	QApplication::restoreOverrideCursor();
	this->cacheFunctionsEnabled(true);
	this->iWorkerLock--;
	this->getCacheSize();
}
void PreferencesDialog::cacheFunctionsEnabled(bool enable)
{
	ui->buttonOK->setEnabled(enable);
	ui->lineTempFolder->setEnabled(enable);
	ui->buttonTempFolder->setEnabled(enable);
	ui->buttonClearCache->setEnabled(enable);
}



void PreferencesDialog::initSettings()
{
	QSettings settings;
	if(!settings.contains(KEY_OUTPUT_DEVICE))
		settings.setValue(KEY_OUTPUT_DEVICE, 0);

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



void PreferencesDialog::closeEvent(QCloseEvent *e)
{
	if(this->iWorkerLock>0)
		e->ignore();
}
