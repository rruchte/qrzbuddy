
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, Configuration *config, Js8CallClient *js8CallClient) : QDialog(parent), configuration(config), js8CallClient(js8CallClient)
{
	ui.setupUi(this);

	if(js8CallClient->isConnected())
	{
		enableJs8CallPopulation();
	}
	else
	{
		disableJs8CallPopulation();
	}

	connect(ui.callsignLineEdit, &QLineEdit::textChanged, this, &SettingsDialog::handleCallsignTextChanged);

	connect(ui.populateFromJs8CallButton, &QPushButton::clicked, this, &SettingsDialog::handleJs8CallPopulationButtonClick);
	connect(ui.populateFromQrzButton, &QPushButton::clicked, this, &SettingsDialog::handleQrzPopulationButtonClick);

	loadSettings();
}

void SettingsDialog::loadSettings()
{
	ui.usernameLineEdit->setText(configuration->getUsername().c_str());
	ui.passwordLineEdit->setText(configuration->getPassword().c_str());

	ui.callsignLineEdit->setText(configuration->getCallsign().c_str());
	ui.gridLineEdit->setText(configuration->getGrid().c_str());
	ui.latitudeLineEdit->setText(configuration->getLat().c_str());
	ui.longitudeLineEdit->setText(configuration->getLng().c_str());

	ui.enableJS8CallCheckBox->setChecked(configuration->getJs8CallEnabled());

	ui.hostnameLineEdit->setText(configuration->getJs8CallHost().c_str());
	ui.portSpinBox->setValue(configuration->getJs8CallPort());

	QString defaultHost = "127.0.0.1";
	int defaultPort = 2442;

	if(ui.hostnameLineEdit->text().isEmpty())
	{
		ui.hostnameLineEdit->setText(defaultHost);
	}

	if(ui.portSpinBox->value() == 0)
	{
		ui.portSpinBox->setValue(defaultPort);
	}
}

void SettingsDialog::accept()
{
	qDebug() << ui.usernameLineEdit->text();
	qDebug() << ui.passwordLineEdit->text();
	qDebug() << ui.callsignLineEdit->text();
	qDebug() << ui.gridLineEdit->text();
	qDebug() << ui.latitudeLineEdit->text();
	qDebug() << ui.longitudeLineEdit->text();
	qDebug() << ui.enableJS8CallCheckBox->isChecked();
	qDebug() << ui.hostnameLineEdit->text();
	qDebug() << ui.portSpinBox->text();

	configuration->setUsername(ui.usernameLineEdit->text().toStdString());
	configuration->setPassword(ui.passwordLineEdit->text().toStdString());
	configuration->setCallsign(ui.callsignLineEdit->text().toStdString());
	configuration->setGrid(ui.gridLineEdit->text().toStdString());
	configuration->setLat(ui.latitudeLineEdit->text().toStdString());
	configuration->setLng(ui.longitudeLineEdit->text().toStdString());

	configuration->setJs8CallEnabled(ui.enableJS8CallCheckBox->isChecked());

	configuration->setJs8CallConnectionDetails(ui.hostnameLineEdit->text().toStdString(), ui.portSpinBox->text().toInt());

	QDialog::accept();
}

void SettingsDialog::handleJs8CallPopulationButtonClick()
{
	qDebug() << "JS8Call Populate Button Clicked";

	Js8CallResponseCallback callsignCallback = [this](const std::string &callsignValue){
		qDebug() << "JS8Call Callsign Response callback: " << callsignValue;
		ui.callsignLineEdit->setText(QString::fromStdString(callsignValue));
	};

	js8CallClient->getCallsign(callsignCallback);

	Js8CallResponseCallback gridCallback = [this](const std::string &gridValue){
		qDebug() << "JS8Call Grid Response callback: " << gridValue;
		ui.gridLineEdit->setText(QString::fromStdString(gridValue));
	};

	js8CallClient->getGrid(gridCallback);
}

void SettingsDialog::handleQrzPopulationButtonClick()
{
	qDebug() << "QRZ Populate Button Clicked";

	std::string call = ui.callsignLineEdit->text().toStdString();

	if(call.length() > 0)
	{
		QrzCallsignResponseCallback callback = [this](Callsign callsign)
		{
			qDebug() << "QRZ Callsign Response callback: " << callsign.getCall();

			ui.gridLineEdit->setText(QString::fromStdString(callsign.getGrid()));
			ui.latitudeLineEdit->setText(QString::fromStdString(callsign.getLat()));
			ui.longitudeLineEdit->setText(QString::fromStdString(callsign.getLon()));
		};

		emit fetchCallsign(call, callback);
	}
}

void SettingsDialog::handleCallsignTextChanged()
{
	if(ui.callsignLineEdit->text().length() > 0)
	{
		ui.populateFromQrzButton->setEnabled(true);
	}
	else
	{
		ui.populateFromQrzButton->setEnabled(false);
	}

}

void SettingsDialog::enableJs8CallPopulation()
{
	ui.populateFromJs8CallButton->setEnabled(true);
}

void SettingsDialog::disableJs8CallPopulation()
{
	ui.populateFromJs8CallButton->setEnabled(false);
}
