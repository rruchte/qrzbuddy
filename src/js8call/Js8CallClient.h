#ifndef QRZBUDDY_JS8CALLCLIENT_H
#define QRZBUDDY_JS8CALLCLIENT_H

#include <QAbstractSocket>
#include <QTcpSocket>

#include "../Configuration.h"
#include "Js8CallRequest.h"

class Js8CallClient : public QObject
{
	Q_OBJECT

public:
	Js8CallClient(const QString &hostname, int port, bool enabled);
	~Js8CallClient();


public slots:
	void enable();
	void disable();
	void changeState(bool);
	bool isConnected();
	void updateConnectionDetails(const QString &hostname, int port);
	void sendRequest(Js8CallRequest request);
	void getCallsign(Js8CallResponseCallback &callback);
	void getGrid(Js8CallResponseCallback &callback);

private slots:
	void socketConnected();
	void socketClosed();
	void socketConnectionClosed();
	void socketError(QAbstractSocket::SocketError);
	void socketReadyRead();

signals:
	void messageReceived(QString msg);
	void clientConnected();
	void clientDisconnected();
	void error(int socketError, const QString &message);

private:
	void connectToJs8Call();
	QString hostName;
	quint16 port;
	bool enabled;
	bool hasEmittedSocketError = false;
	QTcpSocket *socket;
	std::map<int, Js8CallRequest> requestMap;
	bool connected = false;

	void handleResponse(Js8CallRequest &request, std::string responseValue);
};


#endif //QRZBUDDY_JS8CALLCLIENT_H
