#include <QApplication>
#include <QPushButton>
#include <QCommandLineParser>

#include "mainwindow.h"

//using namespace qrz;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("K4RWR");
	QCoreApplication::setApplicationName("QRZBuddy");
	QCoreApplication::setApplicationVersion("1.0");

	MainWindow w;
	w.show();

	return app.exec();
}
