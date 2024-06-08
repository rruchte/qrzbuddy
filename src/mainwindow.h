//
// Created by rob on 5/2/24.
//

#ifndef QRZBUDDY_MAINWINDOW_H
#define QRZBUDDY_MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QStringLiteral>
#include <QLabel>
#include "tablemodel.h"
#include "AppController.h"
#include "js8call/Js8CallClient.h"
#include "DetailDialog.h"
#include "PrintHandler.h"
#include "mapwindow.h"
#include "SettingsDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:
	//bool openFile(const QString &fileName);
	void onCallsignEntryReturnPressed();
	void onJs8CallMessageReceived(QString msg);
	void showCallsignDetail(const QString &call);
	bool collectCredentials();
	void showErrorDialog(const std::string &msg);
	void displaySocketError(int socketError, const QString &message);
	void onJs8CallSocketConnected();
	void onJs8CallSocketDisconnected();

private slots:
	void onActionAboutTriggered();
	void onActionSettingsTriggered();
	void onActionPrintTriggered();
	void onActionSaveCsvTriggered();
	void onActionSaveXmlTriggered();
	void onActionSaveJsonTriggered();
	void onActionSaveMarkdownTriggered();
	void showMapWindow();

private:
	void readSettings();
	void saveSettings() const;
	/*void restoreViewerSettings();
	void resetViewer() const;
	void saveViewerSettings() const;*/

	QDir m_currentDir;
	std::unique_ptr<Ui::MainWindow> ui;
	SettingsDialog *settingsDialog;
	DetailDialog *detailDialog;
	mapwindow *mapWindow;

	TableModel tableModel;
	QSortFilterProxyModel proxyModel;
	QLabel permanentStatusWidget;

	Configuration config;
	AppController *controller;
	Js8CallClient *js8CallClient;

	QWebEngineView printView;
	PrintHandler printHandler;

	static constexpr QLatin1StringView settingsMainWindow = QLatin1StringView("MainWindow");
};


#endif //QRZBUDDY_MAINWINDOW_H
