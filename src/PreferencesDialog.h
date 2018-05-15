#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDebug>

#include <QDialog>
#include <QCoreApplication>
#include <QSettings>
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

	QString getTempFolder() { return this->settings.value(KEY_TEMP_FOLDER, QString("%1/%2").arg(QDir::tempPath()).arg(QCoreApplication::applicationName())).toString(); }

public slots:
	void saveTempFolder(QString);

private:
	Ui::PreferencesDialog *ui;
	QSettings settings;

	QString sOldCache;
	bool bDeleteOldCache = false;
};

#endif // PREFERENCESDIALOG_H
