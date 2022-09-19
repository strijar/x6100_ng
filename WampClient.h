#pragma once

#include <QObject>
#include <QTcpSocket>
#include <qwamp.h>

class WampClient : public QObject
{
    Q_OBJECT
public:
    explicit WampClient(QObject *parent = 0);
    
    void doConnect();

signals:
    
public slots:
    void started();
    void connected();
    void joined(quint64 s);

private:
	void samples(const QVariantList&, const QVariantMap&);
	
	QTcpSocket		*socket;
	QWamp::Session	*session;
};
