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

#include <QVBoxLayout>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), wamp(parent), spectrogram() {
	setFixedSize(800, 480);
	
	QVBoxLayout *layout = new QVBoxLayout(this);
	
	wamp.setSpectrogram(&spectrogram);
	wamp.doConnect();
	
	scope = new Scope(&spectrogram);
	scope->setFixedSize(800, 480 / 2);
	scope->setFilter(0.75f);

	waterfall = new Waterfall(&spectrogram);
	waterfall->setFixedSize(800, 480 / 2);
	waterfall->setFilter(0.25f);

	layout->setMargin(0);
	layout->setSpacing(0);
	
	layout->addWidget(scope);
	layout->addWidget(waterfall);
	
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(updateSpectrogram()));
	timer->start(1000 / 25);
}

void MainWindow::updateSpectrogram() {
	spectrogram.lock();
	scope->calcFilter();
	waterfall->calcFilter();
	spectrogram.unlock();
	
	scope->update();
	waterfall->update();
	spectrogram.reset();
}
