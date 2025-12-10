
#include "DetailDialog.h"
#include "Util.h"
#include "render/CallsignXMLRenderer.h"

#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QHeaderView>
#include <QFile>
#include <QFileDialog>
#include <QQuickItem>
#include <QQuickView>
#include <QQuickWidget>

using namespace qrz::render;

DetailDialog::DetailDialog(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	ui.detailTable->setModel(&tableModel);

	ui.imageBrowser->setAttribute(Qt::WA_AlwaysStackOnTop);
	ui.imageBrowser->page()->setBackgroundColor(Qt::transparent);

	printHandler.setView(&printView);

	connect(&printView, &QWebEngineView::loadFinished, &printHandler, &PrintHandler::printPreview);

	QPageLayout layout(QPageSize::A4, QPageLayout::Portrait, {75, 75, 75, 75});

	connect(&downloadView, &QWebEngineView::loadFinished, this, [this, layout](){
		downloadView.printToPdf([this](const QByteArray &bytes)
								{
									QFileDialog::saveFileContent(bytes, tr("%1.pdf").arg(this->callsign.getCall().c_str()));
								}, layout);
	});

	connect(ui.actionSave, &QAction::triggered, this, &DetailDialog::onSavePdfActionTriggered);
	connect(ui.actionPrint, &QAction::triggered, this, &DetailDialog::onPrintActionTriggered);

	/*
	 * This is the magic line, without this the map will appear blank
	 */
	ui.map->setResizeMode(QQuickWidget::SizeRootObjectToView);

	ui.map->setSource(QUrl("qrc:/map.qml"));
	ui.map->show();

	QQuickItem *mapObj = ui.map->rootObject();
	connect(this, SIGNAL(setCenterPosition(QVariant,QVariant)), mapObj, SLOT(setCenterPosition(QVariant,QVariant)));
	connect(this, SIGNAL(addNamedLocationMarker(QVariant,QVariant,QVariant)), mapObj, SLOT(addNamedLocationMarker(QVariant,QVariant,QVariant)));
	connect(this, SIGNAL(addLocationMarker(QVariant,QVariant)), mapObj, SLOT(addLocationMarker(QVariant,QVariant)));
	connect(this, SIGNAL(setZoom(QVariant)), mapObj, SLOT(setZoom(QVariant)));
	connect(this, SIGNAL(disableCenterAnimation()), mapObj, SLOT(disableCenterAnimation()));
	connect(this, SIGNAL(clearMapItems()), mapObj, SLOT(clearMapItems()));

	emit disableCenterAnimation();
}

void DetailDialog::setCallsign(const Callsign &callsign)
{
	this->callsign = callsign;
	loadCallsign();
}

void DetailDialog::loadCallsign()
{
	ui.callsign->setText(callsign.getCall().c_str());
	ui.formatedName->setText(callsign.getNameFmt().c_str());

	ui.address->setText(callsign.getAddr1().c_str());

	QString formattedAddress2 = tr("%1, %2 %3").arg(callsign.getCity().c_str(),
													 callsign.getState().c_str(),
													 callsign.getZip().c_str());
	ui.cityStateZip->setText(formattedAddress2);

	ui.country->setText(callsign.getCountry().c_str());

	tableModel.setCallsign(callsign);

	ui.detailTable->resizeColumnsToContents();
	ui.detailTable->horizontalHeader()->setStretchLastSection(true);

	clearMapItems();

	if(!callsign.getLat().empty() && !callsign.getLon().empty())
	{
		emit addLocationMarker(std::stof(callsign.getLat()), std::stof(callsign.getLon()));
		emit setZoom(10);
	}
	else
	{
		emit setCenterPosition(39.50, -98.35);
		emit setZoom(4);
	}


	if(!callsign.getImageinfo().empty() && !callsign.getImage().empty())
	{

		/*
		QString imageInfo = {callsign.getImageinfo().c_str()};
		QStringList imageInfoList = imageInfo.split(":");

		int height = imageInfoList.at(0).toInt();
		int width = imageInfoList.at(1).toInt();
		 */

		int width = ui.imageBrowser->width();
		int height = ui.imageBrowser->height();

		QString html = tr(R"html(
<html style="margin: 0;padding: 0;">
<body style="margin: 0;padding: 0;text-align: right;">
<div style="width: %2px;height:%3px;background: url(%1) no-repeat;background-size: contain;background-position: center right;"></div>
</body>
</html>
)html").arg(
				callsign.getImage().c_str(),
				std::to_string(width).c_str(),
				std::to_string(height).c_str()
		);

		ui.imageBrowser->setHtml(html);

		ui.imageBrowser->page()->setBackgroundColor(Qt::transparent);
	}
	else
	{
		ui.imageBrowser->setHtml("");
	}
}

void DetailDialog::onSavePdfActionTriggered()
{
	std::string html = renderHtmlForCallsign();

	downloadView.setHtml(html.c_str());
	downloadView.resize(1024, 750);
}

void DetailDialog::onPrintActionTriggered()
{
	std::string html = renderHtmlForCallsign();

	printView.setHtml(html.c_str());
	printView.resize(1024, 750);
}

std::string DetailDialog::renderHtmlForCallsign()
{
	CallsignXMLRenderer r = CallsignXMLRenderer();
	std::string xml = r.Render({callsign});

	QFile xslFile(":/resources/callsign-detail.xsl");

	if (!xslFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Unable to open XSL file";
		return "";
	}

	QTextStream in(&xslFile);
	QString xsl = in.readAll();

	xslFile.close();

	std::string out = transformXMLWithXSLT(xml, xsl.toStdString());

	return out;
}