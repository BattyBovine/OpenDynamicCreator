#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDebug>

#include <QDialog>
#include <QCoreApplication>
#include <QFileDialog>
#include <QColorDialog>
#include <QSettings>
#include <QMutex>
#include <QDir>

#define KEY_TEMP_FOLDER		"TempFolder"
#define KEY_WINDOW_COLOUR	"WindowColour"
#define KEY_WAVEFORM_COLOUR	"WaveformColour"


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

	void selectWindowColour() { this->settings.setValue(KEY_WINDOW_COLOUR, QColorDialog::getColor(this->settings.value(KEY_WINDOW_COLOUR).value<QColor>(), this)); }
	void selectWaveformColour() { this->settings.setValue(KEY_WAVEFORM_COLOUR, QColorDialog::getColor(this->settings.value(KEY_WAVEFORM_COLOUR).value<QColor>(), this)); }

private:
	Ui::PreferencesDialog *ui;
	QSettings settings;

	QString sOldCache;
	bool bDeleteOldCache = false;
};

#endif // PREFERENCESDIALOG_H
