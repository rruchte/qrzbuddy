#include <QApplication>
#include <QIcon>
#include <QPushButton>
#include <QCommandLineParser>

#include <openssl/provider.h>
#include <openssl/err.h>

#include <stdexcept>

#include "mainwindow.h"

//using namespace qrz;

void initializeOpenSSL()
{
	if (!OSSL_PROVIDER_load(nullptr, "default"))
	{
		throw std::runtime_error(
			"Failed to initialize OpenSSL default provider"
		);
	}
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("K4RWR");
	QCoreApplication::setApplicationName("QRZBuddy");
	QCoreApplication::setApplicationVersion("1.0");
	app.setWindowIcon(QIcon(":/icons/Unix/qrzbuddy_icon.png"));
#ifdef Q_OS_LINUX
	QGuiApplication::setDesktopFileName("io.github.rruchte.QRZBuddy");
#endif

	initializeOpenSSL();

	MainWindow w;
	w.show();

	return app.exec();
}
