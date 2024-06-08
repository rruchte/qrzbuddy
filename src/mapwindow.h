#ifndef QRZBUDDY_MAPWINDOW_H
#define QRZBUDDY_MAPWINDOW_H

#include <QMainWindow>
#include <ui_mapwindow.h>
#include "model/Callsign.h"


using namespace qrz;

class mapwindow final: public QMainWindow
{
Q_OBJECT
public:
	explicit mapwindow(QWidget *parent = nullptr);
	~mapwindow();
	Ui::mapwindow ui;
public slots:
	void addCallsign(const Callsign &callsign);
	void removeCallsign(const Callsign &callsign);
	void removeAllCallsigns();
	void setStationGrid(const QString &grid);
	void setStationCoords(double, double);
signals:
	void addNamedLocationMarkerWithSnr(QVariant, QVariant, QVariant, QVariant, QVariant, QVariant);
	void addLocationMarker(QVariant, QVariant);
	void addStationMarker(QVariant, QVariant);
	void setCenterPosition(QVariant, QVariant);
	void setZoom(QVariant);
	void removeLocationMarker(QVariant, QVariant);
	void zoomToFitItems();
	void clearMapItems();
	void showDetailForCall(const QString &call);
private:
private slots:
	void handleMapCallsignDetailSignal(QString call);
};


#endif //QRZBUDDY_MAPWINDOW_H
