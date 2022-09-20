/*
 *  Xiegu X6100 New Generation
 *  Copyright (c) 2022 by Belousov Oleg aka R1CBU
 *
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <QDebug>
#include "WampClient.h"

using namespace std::placeholders;

WampClient::WampClient(QObject *parent) : QObject(parent) {
}

void WampClient::setSpectrogram(Spectrogram *spectrogram) {
	this->spectrogram = spectrogram;
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
//    session->subscribe("set_vfo", std::bind(&WampClient::set_vfo, this, _1, _2));
//    session->subscribe("set_rxvol", std::bind(&WampClient::set_rxvol, this, _1, _2));
}

void WampClient::samples(const QVariantList& list, const QVariantMap& dict) {
	spectrogram->setSamples(list);
}

void WampClient::set_vfo(const QVariantList& list, const QVariantMap& dict) {
}

void WampClient::set_rxvol(const QVariantList& list, const QVariantMap& dict) {
}
