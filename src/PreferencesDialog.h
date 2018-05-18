#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QCoreApplication>
#include <QThread>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QSettings>
#include <QDir>

#define KEY_TEMP_FOLDER			"TempFolder"
#define KEY_WINDOW_COLOUR		"WindowColour"
#define KEY_WAVEFORM_COLOUR		"WaveformColour"
#define KEY_PLAY_MARKER_COLOUR	"PlayMarkerColour"
#define KEY_EVENT_MARKER_COLOUR	"EventMarkerColour"


namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
	Q_OBJECT
public:
	explicit PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();

	static void initSettings();

	QString getTempFolder() { return this->settings.value(KEY_TEMP_FOLDER).toString(); }

private slots:
	void selectTempFolder();
	void saveTempFolder(QString);
	void getCacheSize(quint64);
	void clearCache();
	void clearCacheResult();

	void selectWindowColour() { this->settings.setValue(KEY_WINDOW_COLOUR, QColorDialog::getColor(this->settings.value(KEY_WINDOW_COLOUR).value<QColor>(), this)); }
	void selectWaveformColour() { this->settings.setValue(KEY_WAVEFORM_COLOUR, QColorDialog::getColor(this->settings.value(KEY_WAVEFORM_COLOUR).value<QColor>(), this)); }
	void selectPlayMarkerColour() { this->settings.setValue(KEY_PLAY_MARKER_COLOUR, QColorDialog::getColor(this->settings.value(KEY_PLAY_MARKER_COLOUR).value<QColor>(), this)); }
	void selectEventMarkerColour() { this->settings.setValue(KEY_EVENT_MARKER_COLOUR, QColorDialog::getColor(this->settings.value(KEY_EVENT_MARKER_COLOUR).value<QColor>(), this)); }

private:
	void cacheFunctionsEnabled(bool);

	Ui::PreferencesDialog *ui;
	QSettings settings;

	QString sOldCache;
	bool bDeleteOldCache = false;
};


class CacheSizeWorker : public QObject
{
	Q_OBJECT
public:
	CacheSizeWorker(QString p) { this->sPath=p; }
public slots:
	void start() { this->iSize = this->getCacheSize(this->sPath); emit(finished(this->iSize)); }
	quint64 getCacheSize(QString path) {
		quint64 size = 0;
		QFileInfo info(path);
		if (info.isDir()) {
			QDir dir(path);
			dir.setFilter(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
			QFileInfoList list = dir.entryInfoList();
			foreach(QFileInfo fileinfo, list)
				size += (fileinfo.isDir()) ? this->getCacheSize(fileinfo.filePath()) : fileinfo.size();
		}
		return size;
	}
signals:
	void finished(quint64);
private:
	QString sPath;
	quint64 iSize=0;
};
class CacheClearWorker : public QObject
{
	Q_OBJECT
public:
	CacheClearWorker(QString p) { this->sPath=p; }
public slots:
	void start() {
		QDir cachedir(this->sPath);
		if(cachedir.exists())
			cachedir.removeRecursively();
		emit(finished());
	}
signals:
	void finished();
private:
	QString sPath;
};

#endif // PREFERENCESDIALOG_H
