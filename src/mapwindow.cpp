#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWidget>
#include <QTimer>

#include "mapwindow.h"
#include "MaidenheadUtils.h"


mapwindow::mapwindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	// This is critical, without this the map will appear blank!
	ui.map->setResizeMode(QQuickWidget::SizeRootObjectToView);

	// Load our QML into the QQuickWidget defined in mapwindow.ui
	ui.map->setSource(QUrl("qrc:/map.qml"));

	// If there are errors, send them to debug stream
	QList<QQmlError> err = ui.map->errors();
	for (QList<QQmlError>::iterator i = err.begin(); i != err.end(); ++i)
	{
		qDebug() << *i;
	}

	// Get a pointer to the map QQuickItem so we can wire up our connections
	QQuickItem *mapObj = ui.map->rootObject();
	connect(this, SIGNAL(addNamedLocationMarkerWithSnr(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)), mapObj, SLOT(addNamedLocationMarkerWithSnr(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)));
	connect(this, SIGNAL(addLocationMarker(QVariant,QVariant)), mapObj, SLOT(addLocationMarker(QVariant,QVariant)));
	connect(this, SIGNAL(addStationMarker(QVariant,QVariant)), mapObj, SLOT(addStationMarker(QVariant,QVariant)));
	connect(this, SIGNAL(removeLocationMarker(QVariant,QVariant)), mapObj, SLOT(removeLocationMarker(QVariant,QVariant)));
	connect(this, SIGNAL(setCenterPosition(QVariant,QVariant)), mapObj, SLOT(setCenterPosition(QVariant,QVariant)));
	connect(this, SIGNAL(setZoom(QVariant)), mapObj, SLOT(setZoom(QVariant)));
	connect(this, SIGNAL(zoomToFitItems()), mapObj, SLOT(zoomToFitItems()));

	connect(mapObj, SIGNAL(showCallsignDetail(QString)), this, SLOT(handleMapCallsignDetailSignal(QString)));
}

mapwindow::~mapwindow(){}

void mapwindow::addCallsign(const Callsign &callsign)
{
	if(!callsign.getLat().empty() && !callsign.getLon().empty())
	{
		emit addNamedLocationMarkerWithSnr(std::stof(callsign.getLat()), std::stof(callsign.getLon()), callsign.getCall().c_str(), callsign.getSnr(), callsign.getReportedSnr(), callsign.getLastHeard().c_str());
		emit zoomToFitItems();
	}
}

void mapwindow::removeCallsign(const Callsign &callsign)
{
	if(!callsign.getLat().empty() && !callsign.getLon().empty())
	{
		emit removeLocationMarker(std::stof(callsign.getLat()), std::stof(callsign.getLon()));
		emit zoomToFitItems();
	}
}

void mapwindow::removeAllCallsigns()
{
	emit clearMapItems();
}

void mapwindow::handleMapCallsignDetailSignal(QString call)
{
	qDebug() << "mapwindow::showCallsignDetail " << call;
	emit showDetailForCall(call);
}

void mapwindow::setStationCoords(double lat, double lon)
{
	emit addStationMarker({lat}, {lon});
}

void mapwindow::setStationGrid(const QString &grid)
{
	if(grid.isEmpty())
	{
		return;
	}

	std::pair<double, double> coords = MaidenheadUtils::grid2Deg(grid.toStdString());

	emit addStationMarker(coords.first, coords.second);
}
