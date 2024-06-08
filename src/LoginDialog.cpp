#include <QSettings>
#include "LoginDialog.h"

LoginDialog::LoginDialog(QWidget *parent, Configuration *config) : QDialog(parent), config(config)
{
	ui.setupUi(this);
	loadSettings();
}

void LoginDialog::loadSettings()
{
	ui.usernameLineEdit->setText(config->getUsername().c_str());
	ui.passwordLineEdit->setText(config->getPassword().c_str());
}

void LoginDialog::accept()
{
	qDebug() << ui.usernameLineEdit->text();
	qDebug() << ui.passwordLineEdit->text();

	config->setUsername(ui.usernameLineEdit->text().toStdString());
	config->setPassword(ui.passwordLineEdit->text().toStdString());

	QDialog::accept();
}
