//
// Created by rob on 5/3/24.
//
#include "tablemodel.h"

#include <format>

#include "Util.h"

const std::string TableModel::headers[] = {"Callsign", "Name", "Class", "Address", "City", "State", "Country"};

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int TableModel::rowCount(const QModelIndex &parent) const
{
	return callsigns.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
	return sizeof(headers) / sizeof(headers[0]);
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		int row = index.row();
		int col = index.column();

		if (callsigns.size() > row)
		{
			Callsign call = callsigns.at(row);

			switch (col)
			{
				case 0:
					return {call.getCall().c_str()};
				case 1:
					return {call.getNameFmt().c_str()};
				case 2:
					return {call.getClass().c_str()};
				case 3:
					return {call.getAddr1().c_str()};
				case 4:
					return {call.getAddr2().c_str()};
				case 5:
					return {call.getState().c_str()};
				case 6:
					return {call.getCountry().c_str()};
			}
		}
	}

	return QVariant();
}

QVariant TableModel::headerData(int column, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal && column < (sizeof(headers) / sizeof(headers[0])))
	{
		return {headers[column].c_str()};
	}

	return QVariant();
}

void TableModel::addCallsign(const Callsign &callsign)
{
	if(callIndex.contains(callsign.getCall()))
	{
		return;
	}

	emit layoutAboutToBeChanged();

	callsigns.push_back(callsign);
	callIndex.insert(callsign.getCall());

	emit layoutChanged();

	emit callsignAdded(callsign);
}

void TableModel::addCallsigns(const std::vector<Callsign> &calls)
{
	emit layoutAboutToBeChanged();

	for(auto currCall: calls)
	{
		if(callIndex.contains(currCall.getCall()))
		{
			return;
		}

		callsigns.push_back(currCall);
		callIndex.insert(currCall.getCall());

		emit callsignAdded(currCall);
	}

	emit layoutChanged();
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
	emit layoutAboutToBeChanged();

	auto begin = callsigns.begin()+row;
	auto end = begin+count;

	for(auto currCallsign = begin; currCallsign != end; ++currCallsign)
	{
		emit callsignRemoved(*currCallsign);
	}

	// Remove the callsign entries
	callsigns.erase(begin, end);

	// Rebuild the index
	callIndex.clear();
	for(Callsign currCallsign : callsigns)
	{
		callIndex.emplace(currCallsign.getCall());
	}

	emit layoutChanged();

	return true;
}

std::vector<Callsign> TableModel::getCallsigns()
{
	return callsigns;
}

Callsign TableModel::getCallsign(int index)
{
	return callsigns.at(index);
}

Callsign TableModel::getCallsign(std::string call)
{
	ToUpper(call);

	for(Callsign currCall : callsigns)
	{
		if (currCall.getCall() == call)
		{
			return currCall;
		}
	}

	std::string msg = std::format("Callsign {:s} not found", call);

	throw std::runtime_error{msg};
}


Callsign *TableModel::getCallsignPtr(std::string call)
{
	ToUpper(call);

	for(Callsign &currCall : callsigns)
	{
		if (currCall.getCall() == call)
		{
			return &currCall;
		}
	}

	std::string msg = std::format("Callsign {:s} not found", call);

	throw std::runtime_error{msg};
}