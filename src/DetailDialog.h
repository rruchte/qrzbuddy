//
// Created by rob on 5/4/24.
//

#ifndef QRZBUDDY_DETAILDIALOG_H
#define QRZBUDDY_DETAILDIALOG_H


#include <QDialog>

#include "ui_detail.h"

#include "Configuration.h"
#include "model/Callsign.h"
#include "detailtablemodel.h"
#include "PrintHandler.h"

using namespace qrz;

class DetailDialog final
: public QMainWindow
{
Q_OBJECT
public:
	DetailDialog(QWidget *parent);
	Ui::DetailDialog ui;
	void setCallsign(const Callsign &callsign);
private slots:
	void onPrintActionTriggered();
	void onSavePdfActionTriggered();
signals:
	void addNamedLocationMarker(QVariant, QVariant, QVariant);
	void addLocationMarker(QVariant, QVariant);
	void setCenterPosition(QVariant, QVariant);
	void setZoom(QVariant);
	void disableCenterAnimation();
	void clearMapItems();
private:
	Callsign callsign;
	DetailTableModel tableModel;

	QWebEngineView printView;
	QWebEngineView downloadView;
	PrintHandler printHandler;

	std::string renderHtmlForCallsign();

	void loadCallsign();
};


#endif //QRZBUDDY_DETAILDIALOG_H
