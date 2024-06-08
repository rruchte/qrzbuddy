//
// Created by rob on 5/3/24.
//

#ifndef QRZBUDDY_DETAILTABLEMODEL_H
#define QRZBUDDY_DETAILTABLEMODEL_H

#include <set>

#include <QAbstractTableModel>

#include "model/Callsign.h"

using namespace qrz;

class DetailTableModel: public QAbstractTableModel
{
Q_OBJECT
public:
	explicit DetailTableModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	void setCallsign(const Callsign &callsign);
	Callsign getCallsign();

private:
	Callsign callsign;
};

#endif //QRZBUDDY_DETAILTABLEMODEL_H
