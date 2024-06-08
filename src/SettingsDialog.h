//
// Created by rob on 5/4/24.
//

#ifndef QRZBUDDY_SETTINGSDIALOG_H
#define QRZBUDDY_SETTINGSDIALOG_H


#include <QDialog>

#include "ui_settings.h"

#include "Configuration.h"
#include "js8call/Js8CallClient.h"
#include "AppController.h"

using namespace qrz;

class SettingsDialog final
: public QDialog
{
Q_OBJECT
public:
	SettingsDialog(QWidget *parent, Configuration *config, Js8CallClient *js8CallClient);

public slots:
	void enableJs8CallPopulation();
	void disableJs8CallPopulation();
signals:
	void fetchCallsign(const std::string &call, QrzCallsignResponseCallback callback);

private slots:
	void accept() override;
	void handleCallsignTextChanged();
	void handleJs8CallPopulationButtonClick();
	void handleQrzPopulationButtonClick();

private:
	Ui::ConfigurationDialog ui;
	Configuration *configuration;
	Js8CallClient *js8CallClient;

	void loadSettings();
};


#endif //QRZBUDDY_SETTINGSDIALOG_H
