//
// Created by rob on 5/3/24.
//

#ifndef QRZBUDDY_TABLEMODEL_H
#define QRZBUDDY_TABLEMODEL_H

#include <set>

#include <QAbstractTableModel>

#include "model/Callsign.h"

using namespace qrz;

class TableModel: public QAbstractTableModel
{
Q_OBJECT
public:
	explicit TableModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	void addCallsign(const Callsign &callsign);
	void addCallsigns(const std::vector<Callsign> &callsigns);
	Callsign getCallsign(int index);
	Callsign getCallsign(std::string call);
	Callsign *getCallsignPtr(std::string call);
	std::vector<Callsign> getCallsigns();
signals:
	void callsignAdded(Callsign callsign);
	void callsignRemoved(Callsign callsign);

private:
	std::vector<Callsign> callsigns;
	std::set<std::string> callIndex;
	static const std::string headers[7];
};

#endif //QRZBUDDY_TABLEMODEL_H
