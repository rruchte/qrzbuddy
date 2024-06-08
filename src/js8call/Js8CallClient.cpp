//
// Created by rob on 5/6/24.
//
#include "Js8CallClient.h"

#include <QTcpSocket>
#include <QTextStream>
#include <QTimer>
#include <random>

#include "Message.h"

Js8CallClient::Js8CallClient(const QString &host, int port, bool enabled) :
																hostName(host),
																port(port),
																enabled(enabled)
{
	// create the socket and connect various of its signals
	socket = new QTcpSocket();
	connect( socket, SIGNAL(connected()), SLOT(socketConnected()) );
	connect( socket, SIGNAL(disconnected()), SLOT(socketConnectionClosed()) );
	connect( socket, SIGNAL(readyRead()), SLOT(socketReadyRead()) );
	connect( socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));

	if(enabled)
	{
		connectToJs8Call();
	}
}

Js8CallClient::~Js8CallClient()
{
	delete socket;
}

void Js8CallClient::updateConnectionDetails(const QString &hostName, int port)
{
	this->hostName = hostName;
	this->port = port;

	if(enabled && socket->state() == QAbstractSocket::ConnectedState)
	{
		disable();
		enable();
	}
}

void Js8CallClient::connectToJs8Call()
{
	socket->connectToHost( hostName, port );
}

void Js8CallClient::enable()
{
	if(socket->state() == QAbstractSocket::UnconnectedState)
	{
		connectToJs8Call();
	}

	enabled = true;
}

void Js8CallClient::disable()
{
	if(socket->state() != QAbstractSocket::UnconnectedState)
	{
		socket->disconnectFromHost();
	}

	enabled = false;
}

void Js8CallClient::changeState(bool state)
{
	qDebug() << "Js8CallClient::changeState " << ((state) ? "true":"false");

	enabled = state;

	if(state)
	{
		enable();
	}
	else
	{
		disable();
	}
}

bool Js8CallClient::isConnected()
{
	return connected;
}

void Js8CallClient::socketReadyRead()
{
	// read from the server
	while (socket->canReadLine())
	{
		auto msg = socket->readLine().trimmed();

		if(msg.isEmpty()){
			return;
		}

		QJsonParseError e;
		QJsonDocument d = QJsonDocument::fromJson(msg, &e);
		if(e.error != QJsonParseError::NoError)
		{
			qDebug() << "API.ERROR: Invalid JSON (unparsable)";
			return;
		}

		if(!d.isObject())
		{
			qDebug() << "API.ERROR: Invalid JSON (not an object)";
			return;
		}

		Message m;
		m.read(d.object());

		if(requestMap.contains(m.id()))
		{
			handleResponse(requestMap.at(m.id()), m.value().toStdString());
			requestMap.erase(m.id());
		}

		emit messageReceived(msg);
	}
}

void Js8CallClient::socketConnected()
{
	qDebug() << "Connected to JS8Call server";
	emit clientConnected();
	connected = true;
	// Since we've connected successfully, we want to re-enable error alerts
	hasEmittedSocketError = false;
}

void Js8CallClient::socketConnectionClosed()
{
	qDebug() << "Connection closed by the JS8Call server";
	connected = false;
	emit clientDisconnected();
}

void Js8CallClient::socketClosed()
{
	qDebug() << "Connection to JS8Call server closed";
	connected = false;
	emit clientDisconnected();
}

void Js8CallClient::socketError( QAbstractSocket::SocketError e )
{
	QString msg = tr("Error number %1 occurred\n").arg(e);

	qDebug() << msg;

	disable();

	QTimer::singleShot( 5000, this, SLOT(enable()));

	if(!hasEmittedSocketError)
	{
		// Set this before we emit, or we will emit continuously until all alert dialogs have been closed
		hasEmittedSocketError = true;

		emit error(e, msg);
	}
}

void Js8CallClient::sendRequest(Js8CallRequest request)
{
	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(100000000, 9999999999);

	int id = 0;

	do{
		id = dis(gen);
	} while(requestMap.contains(id));

	std::string type = "";
	if(request.getType() == STATION_GET_CALLSIGN)
	{
		type = "STATION.GET_CALLSIGN";
	}
	else if(request.getType() == STATION_GET_GRID)
	{
		type = "STATION.GET_GRID";
	}
	else
	{
		throw std::invalid_argument("Invalid request type");
	}

	auto m = Message(type.c_str(), {}, {
			{"_ID", id},
	});

	requestMap.emplace(id, request);
	qDebug() << m.toJson();
	socket->write(m.toJson());
	socket->write("\n");
	socket->flush();
}

void Js8CallClient::handleResponse(Js8CallRequest &request, std::string responseValue)
{
	Js8CallResponseCallback callback = request.getCallback();
	callback(responseValue);
}

void Js8CallClient::getCallsign(Js8CallResponseCallback &callback)
{
	qDebug() << "Js8CallClient::getCallsign";
	Js8CallRequest request(STATION_GET_CALLSIGN, callback);

	sendRequest(request);
}

void Js8CallClient::getGrid(Js8CallResponseCallback &callback)
{
	qDebug() << "Js8CallClient::getGrid";
	Js8CallRequest request(STATION_GET_GRID, callback);

	sendRequest(request);
}

