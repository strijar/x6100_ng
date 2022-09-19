#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QDebug>
#include "MainWindow.h"

int main(int argc, char **argv) {
  	QApplication 	app(argc, argv);
	MainWindow		main_window;
	
  	main_window.resize(800, 480);
  	main_window.show();

  	return app.exec();
}
