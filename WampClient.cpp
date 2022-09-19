#include <QDebug>
#include "WampClient.h"

using namespace std::placeholders;

WampClient::WampClient(QObject *parent) : QObject(parent) {
}
    
void WampClient::doConnect() {
	socket = new QTcpSocket(this);
	
	connect(socket, SIGNAL(connected()), this, SLOT(connected()));
	socket->connectToHost("192.168.1.214", 55555);
	
	if (!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
    }
}

void WampClient::connected() {
	QString sessionName("session");

	session = new QWamp::Session(sessionName, *socket);

	connect(session, SIGNAL(started()), this, SLOT(started()));
	session->start();
}

void WampClient::started() {
	connect(session, SIGNAL(joined(quint64)), this, SLOT(joined(quint64)));
	session->join("default");
}

void WampClient::joined(quint64 s) {
    session->subscribe("samples", std::bind(&WampClient::samples, this, _1, _2));
}

void WampClient::samples(const QVariantList& list, const QVariantMap& dict) {
	qDebug() << "samples " << list;
}
