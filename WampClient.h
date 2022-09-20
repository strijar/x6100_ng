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

#pragma once

#include <QObject>
#include <QTcpSocket>
#include <qwamp.h>
#include "Spectrogram.h"

class WampClient : public QObject
{
    Q_OBJECT
public:
    explicit WampClient(QObject *parent = 0);
    
    void doConnect();
    void setSpectrogram(Spectrogram *spectrogram);

signals:
    
public slots:
    void started();
    void connected();
    void joined(quint64 s);

private:
	void samples(const QVariantList&, const QVariantMap&);
	void set_vfo(const QVariantList&, const QVariantMap&);
	void set_rxvol(const QVariantList&, const QVariantMap&);
	
	QTcpSocket		*socket;
	QWamp::Session	*session;
	Spectrogram		*spectrogram;
};
