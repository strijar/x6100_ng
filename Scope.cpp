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

#include <QPaintEvent>
#include <QDebug>
#include "Scope.h"

Scope::Scope(Spectrogram *spectrogram, QWidget *parent) : QOpenGLWidget(parent) {
	this->spectrogram = spectrogram;
	chart_pix = new QPixmap();

	grid_min = -100;
	grid_max = -50;
	grid_step = 6;

	setAutoFillBackground(false);
	
	background = QBrush(QColor(32, 32, 32));
	
	chart = QPen(QColor(255, 255, 255, 128));
	chart.setWidth(1);

	grid = QPen(QColor(200, 200, 200));
	grid.setWidth(1);
	grid.setStyle(Qt::DashLine);
	
	durty = true;	
	filter = 0.5f;
	
	psd_filter = new float[spectrogram->getNum()];
	
	for (int i = 0; i < spectrogram->getNum(); i++)
		psd_filter[i] = -130.0f;
}

void Scope::resizeEvent(QResizeEvent *event) {
	delete chart_pix;
	
	chart_pix = new QPixmap(spectrogram->getNum(), height());
}

void Scope::calcFilter() {
	float *psd = spectrogram->getPsd();

	for (int i = 0; i < spectrogram->getNum(); i++)
		psd_filter[i] = durty ? psd[i] : psd_filter[i] * filter + psd[i] * (1.0f - filter);

	if (durty) {
		durty = false;
	}
}

void Scope::paintEvent(QPaintEvent *event) {
    // Chart

	chart_pix->fill(Qt::transparent);
	
    painter.begin(chart_pix);
    painter.setPen(chart);

	for (int x = 0; x < chart_pix->width(); x++) {
		float v = (psd_filter[x] - grid_min) / (grid_max - grid_min);
		
		painter.drawLine(x, chart_pix->height(), x, chart_pix->height() * (1.0f - v));
	}
    
    painter.end();

	// Grid
 
    painter.begin(this);
    painter.fillRect(event->rect(), background);

    painter.setPen(grid);
    
    for (int db = grid_min; db < grid_max; db += grid_step) {
		float 	v = (float)(db - grid_min) / (grid_max - grid_min);
		int		y = height() * (1.0f - v);
	
		painter.drawLine(0, y, width(), y);
	}

    for (int x = 0; x < width(); x += width() / 10) {
		painter.drawLine(x, 0, x, height());
	}
    
    painter.drawPixmap(0, 0, chart_pix->scaledToWidth(width(), Qt::SmoothTransformation));
    painter.end();
}

void Scope::setFilter(float f) {
	filter = f;
	durty = true;
}
