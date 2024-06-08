#include <algorithm>

#include <QToolButton>
#include <QMessageBox>

#include <QDir>
#include <QSettings>
#include <iostream>
#include <QErrorMessage>
#include <QAbstractSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <QTimer>
#include <QPrinter>
#include <QPrintPreviewDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tablemodel.h"
#include "Util.h"
#include "SettingsDialog.h"
#include "LoginDialog.h"
#include "DetailDialog.h"
#include "mapwindow.h"

#include "render/CallsignCSVRenderer.h"
#include "render/CallsignXMLRenderer.h"
#include "render/CallsignJSONRenderer.h"
#include "render/CallsignMarkdownRenderer.h"
#include "render/CallsignConsoleRenderer.h"
#include "PrintHandler.h"
#include "MaidenheadUtils.h"

using namespace Qt::StringLiterals;
using namespace qrz::render;
using namespace qrz;

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow)
{
	QCoreApplication::setOrganizationName("K4RWR");
	QCoreApplication::setApplicationName("QRZBuddy");

	ui->setupUi(this);

	js8CallClient = new Js8CallClient(config.getJs8CallHost().c_str(), config.getJs8CallPort(), config.getJs8CallEnabled());

	detailDialog = new DetailDialog(this);
	mapWindow = new mapwindow(this);
	settingsDialog = new SettingsDialog(this, &config, js8CallClient);

	// Add permanent status widget
	ui->statusbar->addPermanentWidget(&permanentStatusWidget);

	ui->callsignTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	ui->callsignTable->horizontalHeader()->setStretchLastSection(true);

	connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onActionSettingsTriggered);
	connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);
	connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::onActionPrintTriggered);
	connect(ui->actionCSV, &QAction::triggered, this, &MainWindow::onActionSaveCsvTriggered);
	connect(ui->actionXML, &QAction::triggered, this, &MainWindow::onActionSaveXmlTriggered);
	connect(ui->actionJSON, &QAction::triggered, this, &MainWindow::onActionSaveJsonTriggered);
	connect(ui->actionMarkdown, &QAction::triggered, this, &MainWindow::onActionSaveMarkdownTriggered);
	connect(ui->actionMapWindow, &QAction::triggered, this, &MainWindow::showMapWindow);

	connect(ui->callsignEntry, &QLineEdit::returnPressed, this, &MainWindow::onCallsignEntryReturnPressed);

	proxyModel.setSourceModel( &tableModel );

	ui->callsignTable->setModel(&proxyModel);
	ui->callsignTable->show();

	connect(ui->callsignTable, &CallsignTableView::showDetailForCall, this, &MainWindow::showCallsignDetail);

	controller = new AppController(&config, &tableModel);

	connect(controller, &AppController::credentialsNeeded, this, &MainWindow::collectCredentials);
	connect(controller, &AppController::displayError, this, &MainWindow::showErrorDialog);

	controller->initialize();

	connect(js8CallClient, &Js8CallClient::messageReceived, this, &MainWindow::onJs8CallMessageReceived);
	connect(js8CallClient, &Js8CallClient::error, this, &MainWindow::displaySocketError);
	connect(js8CallClient, &Js8CallClient::clientConnected, this, &MainWindow::onJs8CallSocketConnected);
	connect(js8CallClient, &Js8CallClient::clientDisconnected, this, &MainWindow::onJs8CallSocketDisconnected);
	connect(&config, &Configuration::js8CallEnabledStateChange, js8CallClient, &Js8CallClient::changeState);
	connect(&config, &Configuration::gridChanged, mapWindow, &mapwindow::setStationGrid);
	connect(&config, &Configuration::coordsChanged, mapWindow, &mapwindow::setStationCoords);

	connect(settingsDialog, &SettingsDialog::fetchCallsign, controller, &AppController::fetchCallsign);

	connect(mapWindow, &mapwindow::showDetailForCall, this, &MainWindow::showCallsignDetail);

	connect(&tableModel, &TableModel::callsignAdded, mapWindow, &mapwindow::addCallsign);
	connect(&tableModel, &TableModel::callsignRemoved, mapWindow, &mapwindow::removeCallsign);

	printHandler.setView(&printView);

	if(config.hasLat() && config.hasLng())
	{
		mapWindow->setStationCoords(std::stod(config.getLat()), std::stod(config.getLng()));
	}
	else if(config.hasGrid())
	{
		std::pair<double, double> coords = MaidenheadUtils::grid2Deg(config.getGrid());
		mapWindow->setStationCoords(coords.first, coords.second);
	}

	//ui->callsignTable->horizontalHeaderItem(0)->setText("Callsigns");
/*
	auto *menu = new RecentFileMenu(this, m_recentFiles.get());
	ui->actionRecent->setMenu(menu);
	connect(menu, &RecentFileMenu::fileOpened, this, &MainWindow::openFile);
	QWidget *w = ui->mainToolBar->widgetForAction(ui->actionRecent);
	auto *button = qobject_cast<QToolButton *>(w);
	if (button)
		connect(ui->actionRecent, &QAction::triggered, button, &QToolButton::showMenu);
*/
}

MainWindow::~MainWindow()
{
	saveSettings();

	delete(controller);
}

void MainWindow::onActionAboutTriggered()
{

	QString text = tr("A Widgets application to display and print JSON, "
					  "text and PDF files. Demonstrates various features to use "
					  "in widget applications: Using QSettings, query and save "
					  "user preferences, manage file histories and control cursor "
					  "behavior when hovering over widgets.\n\n"
					  "This version has loaded the following plugins:\n%1\n"
					  "\n\nIt supports the following mime types:\n%2")
			.arg("WOMBATS", "FOOBAR");

	QMessageBox::about(this, tr("About Document Viewer Demo"), text);
}


void MainWindow::onActionSettingsTriggered()
{
	qDebug() << "Pop settings dialog";

	settingsDialog->exec();
}

void MainWindow::onActionPrintTriggered()
{
	CallsignXMLRenderer r = CallsignXMLRenderer();
	std::string xml = r.Render(tableModel.getCallsigns());

	QFile xslFile(":/resources/callsign-list.xsl");

	if (!xslFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Unable to open XSL file";
		return;
	}

	QTextStream in(&xslFile);
	QString xsl = in.readAll();

	xslFile.close();

	std::string out = transformXMLWithXSLT(xml, xsl.toStdString());

	connect(&printView, &QWebEngineView::loadFinished, &printHandler, &PrintHandler::printPreview);

	printView.setHtml(out.c_str());
	printView.resize(1024, 750);
}

void MainWindow::onActionSaveCsvTriggered()
{
	CallsignCSVRenderer renderer = CallsignCSVRenderer();
	std::string output = renderer.Render(tableModel.getCallsigns());

	QFileDialog::saveFileContent(output.c_str(), "qrz_callsigns.csv");
}

void MainWindow::onActionSaveXmlTriggered()
{
	CallsignXMLRenderer renderer = CallsignXMLRenderer();
	std::string output = renderer.Render(tableModel.getCallsigns());

	QFileDialog::saveFileContent(output.c_str(), "qrz_callsigns.xml");
}

void MainWindow::onActionSaveJsonTriggered()
{
	CallsignJSONRenderer renderer = CallsignJSONRenderer();
	std::string output = renderer.Render(tableModel.getCallsigns());

	QFileDialog::saveFileContent(output.c_str(), "qrz_callsigns.json");
}

void MainWindow::onActionSaveMarkdownTriggered()
{
	CallsignMarkdownRenderer renderer = CallsignMarkdownRenderer();
	std::string output = renderer.Render(tableModel.getCallsigns());

	QFileDialog::saveFileContent(output.c_str(), "qrz_callsigns.md");
}

bool MainWindow::collectCredentials()
{
	LoginDialog login(this, &config);
	login.setModal(true);
	login.exec();

	return true;
}

void MainWindow::showCallsignDetail(const QString &call)
{
	qDebug() << "Show details for " << call;

	try
	{
		Callsign callsign = tableModel.getCallsign(call.toStdString());

		detailDialog->setCallsign(callsign);
		detailDialog->show();

		// Load bio after we show the window so we don't hold up the dialog
		std::string bioHTML = controller->fetchBio(call.toStdString());
		detailDialog->ui.bioBrowser->setHtml(bioHTML.c_str());
	}
	catch(std::exception &e)
	{
		showErrorDialog(e.what());
	}
}

void MainWindow::showMapWindow()
{
	mapWindow->show();
}

void MainWindow::showErrorDialog(const std::string &msg)
{
	QErrorMessage *errorDialog = new QErrorMessage(this);
	errorDialog->showMessage(msg.c_str());
	errorDialog->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
	errorDialog->setModal(false);
	errorDialog->show();
}

void MainWindow::displaySocketError(int socketError, const QString &message)
{
	onJs8CallSocketDisconnected();

	switch (socketError) {
		case QAbstractSocket::HostNotFoundError:
			QMessageBox::information(this, tr("QRZBuddy"),
									 tr("The JS8Call host was not found. Please check the "
										"host and port settings."));
			break;
		case QAbstractSocket::ConnectionRefusedError:
			QMessageBox::information(this, tr("QRZBuddy"),
									 tr("The connection was refused by the peer. "
										"Make sure the JS8Call server is running, "
										"and check that the host name and port "
										"settings are correct."));
			break;
		default:
			QMessageBox::information(this, tr("QRZBuddy"),
									 tr("The following error occurred: %1.")
											 .arg(message));
	}
}

void MainWindow::readSettings()
{
	QSettings settings;

	// Restore QMainWindow state
	if (settings.contains(settingsMainWindow)) {
		QByteArray mainWindowState = settings.value(settingsMainWindow).toByteArray();
		restoreState(mainWindowState);
	}
}

void MainWindow::saveSettings() const
{
	QSettings settings;

	// Save working directory
	//settings.setValue(settingsDir, m_currentDir.absolutePath());

	// Save QMainWindow state
	settings.setValue(settingsMainWindow, saveState());

	// Save recent files
	//m_recentFiles->saveSettings(settings, settingsFiles);

	settings.sync();
}

void MainWindow::onCallsignEntryReturnPressed()
{
	qDebug() << "Callsign from manual input: " << ui->callsignEntry->text().toLocal8Bit().data();

	QString callsignInput = ui->callsignEntry->text();
	QList<QString> calls = callsignInput.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

	std::set<std::basic_string<char>> terms;
	foreach(auto &call, calls)
	{
		std::string callStr = call.toStdString();
		ToUpper(callStr);

		terms.insert(callStr);
	}

	AppCommand cmd;
	cmd.setSearchTerms(terms);

	if(controller->handleCommand(cmd))
	{
		ui->callsignEntry->clear();
		ui->callsignTable->resizeColumnsToContents();
		ui->callsignTable->horizontalHeader()->setStretchLastSection(true);
	}
}

void MainWindow::onJs8CallMessageReceived(QString msg)
{
	QJsonParseError e;
	QJsonDocument doc = QJsonDocument::fromJson(msg.toLocal8Bit(), &e);

	if(e.error != QJsonParseError::NoError)
	{
		QErrorMessage *errorDialog = new QErrorMessage(this);
		errorDialog->showMessage(e.errorString());
		errorDialog->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
		errorDialog->setModal(false);
		errorDialog->show();

		return;
	}

	if(!doc.isObject())
	{
		qDebug() << "QJsonDocument did not produce an object for message: " << msg;

		return;
	}

	QJsonObject json = doc.object();

	if(json.contains("params") && json["params"].isObject())
	{
		QJsonObject params = json["params"].toObject();
		if(params.contains("FROM"))
		{
			QString from = params.value("FROM").toString();
			if(!from.startsWith("@"))
			{
				qDebug() << "Callsign from JS8Call: " << from;

				//Normalize compound callsigns like K4RWR/P
				auto fromParts = from.split("/");

				std::set<std::basic_string<char>> terms;
				terms.insert(fromParts.at(0).toStdString());

				AppCommand cmd;
				cmd.setSearchTerms(terms);

				if(controller->handleCommand(cmd))
				{
					ui->callsignEntry->clear();
					ui->callsignTable->resizeColumnsToContents();
				}

				try
				{
					Callsign *callsign = tableModel.getCallsignPtr(from.toStdString());

					if (callsign == nullptr)
					{
						return;
					}

					QDateTime now = QDateTime::currentDateTime();
					QString datestamp = now.toString("yyyy-MM-dd hh:mm");

					callsign->setLastHeard(datestamp.toStdString());

					if (params.contains("SNR"))
					{
						auto snr = params.value("SNR").toInt();
						qDebug() << from << "SNR: " << snr;

						callsign->setSnr(snr);
					}

					if (params.contains("CMD"))
					{
						QString cmd = params.value("CMD").toString();

						if (cmd.contains("SNR") && cmd.contains("SNR") && params.contains("EXTRA"))
						{
							QString recipCall = params.value("TO").toString();
							int reportedSNR = params.value("EXTRA").toString().toInt();

							qDebug() << "Signal report to " << recipCall << " from " << from << ": " << reportedSNR;

							// If this is a signal report to us...
							if (config.getUsername() == recipCall.toStdString())
							{
								qDebug() << "My signal report from " << from << ": " << reportedSNR;

								callsign->setReportedSnr(reportedSNR);
							}
						}
					}

					mapWindow->addCallsign(*callsign);
				}
				catch(std::runtime_error &e)
				{
					qDebug() << e.what();
				}
			}
		}
	}
}

void MainWindow::onJs8CallSocketConnected()
{
	permanentStatusWidget.setText("JS8Call Connected");
	permanentStatusWidget.setStyleSheet("color: #00AA00");

	settingsDialog->enableJs8CallPopulation();
}

void MainWindow::onJs8CallSocketDisconnected()
{
	if(config.getJs8CallEnabled())
	{
		permanentStatusWidget.setText("JS8Call Disconnected");
		permanentStatusWidget.setStyleSheet("color: #FF0000");
	}
	else
	{
		permanentStatusWidget.setText("JS8Call Disabled");
		permanentStatusWidget.setStyleSheet("color: #999999");
	}

	settingsDialog->disableJs8CallPopulation();
}

/*
void MainWindow::saveViewerSettings() const
{
	if (!m_viewer)
		return;

	QSettings settings;
	settings.beginGroup(settingsViewers);
	settings.setValue(m_viewer->viewerName(), m_viewer->saveState());
	settings.endGroup();
	settings.sync();
}

void MainWindow::resetViewer() const
{
	if (!m_viewer)
		return;

	saveViewerSettings();
	m_viewer->cleanup();
}

void MainWindow::restoreViewerSettings()
{
	if (!m_viewer)
		return;

	QSettings settings;
	settings.beginGroup(settingsViewers);
	QByteArray viewerSettings = settings.value(m_viewer->viewerName(), QByteArray()).toByteArray();
	settings.endGroup();
	if (!viewerSettings.isEmpty())
		m_viewer->restoreState(viewerSettings);
}
*/
