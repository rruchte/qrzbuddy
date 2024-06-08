//
// Created by rob on 5/4/24.
//

#ifndef QRZBUDDY_LOGINDIALOG_H
#define QRZBUDDY_LOGINDIALOG_H


#include <QDialog>

#include "ui_login.h"

#include "Configuration.h"

using namespace qrz;

class LoginDialog final
: public QDialog
{
Q_OBJECT
public:
	LoginDialog(QWidget *parent, Configuration *config);
private slots:
	void accept() override;
private:
	Ui::LoginDialog ui;
	Configuration *config;

	void loadSettings();
};


#endif //QRZBUDDY_LOGINDIALOG_H
