#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDebug>

#include <QDialog>
#include <QCoreApplication>
#include <QSettings>
#include <QMutex>
#include <QDir>

#define KEY_TEMP_FOLDER	"TempFolder"


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

public slots:
	void saveTempFolder(QString);

private:
	Ui::PreferencesDialog *ui;
	QSettings settings;

	QString sOldCache;
	bool bDeleteOldCache = false;
};

#endif // PREFERENCESDIALOG_H
