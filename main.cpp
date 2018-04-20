#include "OpenDynamicCreator.h"
#include <QApplication>

#include "version.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setStyle("Fusion");

	a.setOrganizationName("Batty Bovine Productions, LLC");
	a.setOrganizationDomain("battybovine.com");
	a.setApplicationName(QApplication::tr("OpenDynamic Creator"));
	a.setApplicationVersion(QString("%1.%2.%3.%4")
							.arg(Version::MAJOR)
							.arg(Version::MINOR)
							.arg(Version::REVISION)
							.arg(Version::BUILD));

	OpenDynamicCreator w;
	w.show();

	return a.exec();
}
