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
	grid_min = -90;
	grid_max = -50;
	grid_step = 6;

	setAutoFillBackground(false);
	
	background = QBrush(QColor(32, 32, 32));
	clean = QBrush(QColor(0, 0, 0, 32));
	
	chart = QPen(QColor(0, 196, 64, 200));
	chart.setWidth(1);

	grid = QPen(QColor(200, 200, 200));
	grid.setWidth(1);
	grid.setStyle(Qt::DashLine);
	
	chart_pix = new QPixmap();
}

void Scope::resizeEvent(QResizeEvent *event) {
	delete chart_pix;
	
	chart_pix = new QPixmap(spectrogram->getNum(), height());
}

void Scope::setSpectrogram(Spectrogram *spectrogram) {
	this->spectrogram = spectrogram;
}

void Scope::paintEvent(QPaintEvent *event) {
	QPainter painter;
 
    painter.begin(this);
    painter.fillRect(event->rect(), background);

	// Grid

    painter.setPen(grid);
    
    for (int db = grid_min; db < grid_max; db += grid_step) {
		float 	v = (float)(db - grid_min) / (grid_max - grid_min);
		int		y = height() * (1.0f - v);
	
		painter.drawLine(0, y, width(), y);
	}

    for (int x = 0; x < width(); x += width() / 6) {
		painter.drawLine(x, 0, x, height());
	}

    // Chart

	QPainter painter_chart;

	chart_pix->fill(Qt::transparent);
	
    painter_chart.begin(chart_pix);
    painter_chart.setPen(chart);

	float *psd = spectrogram->getPsd();

	for (int x = 0; x < chart_pix->width(); x++) {
		float v = (psd[x] - grid_min) / (grid_max - grid_min);
		
		painter_chart.drawLine(x, chart_pix->height(), x, chart_pix->height() * (1.0f - v));
	}
    
    painter_chart.end();
    
    painter.drawPixmap(0, 0, chart_pix->scaledToWidth(width(), Qt::SmoothTransformation));
    painter.end();
}
