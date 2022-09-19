#include <QWidget>

#include "WampClient.h"

class MainWindow : public QWidget 
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);

signals:
public slots:

private:
	WampClient		wamp;
};
