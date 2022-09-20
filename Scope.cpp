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

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include "Scope.h"

Scope::Scope(QWidget *parent) : QOpenGLWidget(parent) {
	min_db = -90.0f;
	max_db = -50.0f;

	setAutoFillBackground(false);
	
	background = QBrush(QColor(32, 32, 32));
	
	lines = QPen(QColor(64, 200, 64));
	lines.setWidth(1);
}

void Scope::setSpectrogram(Spectrogram *spectrogram) {
	this->spectrogram = spectrogram;
}

void Scope::paintEvent(QPaintEvent *event) {
	QPainter painter;
 
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    painter.fillRect(event->rect(), background);
    painter.setPen(lines);

	float *psd = spectrogram->getPsd();

	for (unsigned int x = 0; x < spectrogram->getNum(); x++) {
		float v = (psd[x] - min_db) / (max_db - min_db);
		
		painter.drawLine(x, height(), x, height() * (1.0 - v));
	}
    
    painter.end();
}
