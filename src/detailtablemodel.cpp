#include "detailtablemodel.h"

DetailTableModel::DetailTableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

void DetailTableModel::setCallsign(const Callsign &callsign)
{
	this->callsign = callsign;
}

Callsign DetailTableModel::getCallsign()
{
	return callsign;
}


int DetailTableModel::rowCount(const QModelIndex &parent) const
{
	return 41;
}

int DetailTableModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant DetailTableModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		int row = index.row();
		int col = index.column();

		switch (row)
		{
			case 0:
				return (col==0) ? "First Name":QVariant{callsign.getFname().c_str()};
			case 1:
				return (col==0) ? "Last Name":QVariant{callsign.getName().c_str()};
			case 2:
				return (col==0) ? "Formatted Name":callsign.getNameFmt().c_str();
			case 3:
				return (col==0) ? "Nickname":callsign.getNickname().c_str();
			case 4:
				return (col==0) ? "Address":callsign.getAddr1().c_str();
			case 5:
				return (col==0) ? "City":callsign.getCity().c_str();
			case 6:
				return (col==0) ? "State":callsign.getState().c_str();
			case 7:
				return (col==0) ? "Zip":callsign.getZip().c_str();
			case 8:
				return (col==0) ? "FIPS county identifier (USA)":callsign.getFips().c_str();
			case 9:
				return (col==0) ? "County":callsign.getCounty().c_str();
			case 10:
				return (col==0) ? "Aliases":callsign.getAliases().c_str();
			case 11:
				return (col==0) ? "XRef":callsign.getXref().c_str();
			case 12:
				return (col==0) ? "Country":callsign.getCountry().c_str();
			case 13:
				return (col==0) ? "DXCC country name":callsign.getCcode().c_str();
			case 14:
				return (col==0) ? "Latitude":callsign.getLat().c_str();
			case 15:
				return (col==0) ? "Longitude":callsign.getLon().c_str();
			case 16:
				return (col==0) ? "Grid":callsign.getGrid().c_str();
			case 17:
				return (col==0) ? "Effective Date":callsign.getEfdate().c_str();
			case 18:
				return (col==0) ? "Expiration Date":callsign.getExpdate().c_str();
			case 19:
				return (col==0) ? "Previous Callsign":callsign.getPcall().c_str();
			case 20:
				return (col==0) ? "License Class":callsign.getClass().c_str();
			case 21:
				return (col==0) ? "License type codes (USA)":callsign.getCodes().c_str();
			case 22:
				return (col==0) ? "QSL manager info":callsign.getQslmgr().c_str();
			case 23:
				return (col==0) ? "Email address":callsign.getEmail().c_str();
			case 24:
				return (col==0) ? "Web page address":callsign.getUrl().c_str();
			case 25:
				return (col==0) ? "QRZ web page views":std::to_string(callsign.getUViews()).c_str();
			case 26:
				return (col==0) ? "Metro Service Area (USPS)":callsign.getMsa().c_str();
			case 27:
				return (col==0) ? "Telephone Area Code (USA)":callsign.getAreaCode().c_str();
			case 28:
				return (col==0) ? "Time Zone (USA)":callsign.getTimeZone().c_str();
			case 29:
				return (col==0) ? "GMT Time Offset":std::to_string(callsign.getGmtOffset()).c_str();
			case 30:
				return (col==0) ? "Daylight Saving Time Observed":callsign.getDst().c_str();
			case 31:
				return (col==0) ? "Will accept e-qsl":callsign.getEqsl().c_str();
			case 32:
				return (col==0) ? "Will return paper QSL":callsign.getMqsl().c_str();
			case 33:
				return (col==0) ? "CQ Zone identifier":std::to_string(callsign.getCqzone()).c_str();
			case 34:
				return (col==0) ? "ITU Zone identifier":std::to_string(callsign.getItuzone()).c_str();
			case 35:
				return (col==0) ? "Operator's year of birth":callsign.getBorn().c_str();
			case 36:
				return (col==0) ? "Managing callsign on QRZ":callsign.getUser().c_str();
			case 37:
				return (col==0) ? "Will accept LOTW":callsign.getLotw().c_str();
			case 38:
				return (col==0) ? "IOTA Designator":callsign.getIota().c_str();
			case 39:
				return (col==0) ? "Source of lat/long data":callsign.getGeoloc().c_str();
			case 40:
				return (col==0) ? "Attention address line":callsign.getAttn().c_str();
		}
	}

	return QVariant();
}
