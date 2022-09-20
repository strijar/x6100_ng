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
#include "Waterfall.h"

Waterfall::Waterfall(Spectrogram *spectrogram, QWidget *parent) : QOpenGLWidget(parent) {
	this->spectrogram = spectrogram;

	db_min = -100;
	db_max = -50;

	setAutoFillBackground(false);

	pen = QPen(Qt::red);
	
	chart_pix = new QPixmap();

	durty = true;	
	filter = 0.5f;
	
	psd_filter = new float[spectrogram->getNum()];
	
	for (int i = 0; i < spectrogram->getNum(); i++)
		psd_filter[i] = -130.0f;

	gradient_steps = 255;
	
	gradient = new QLinearGradient(0, 0, gradient_steps, 0);

	gradient->setColorAt(0, Qt::black);
	gradient->setColorAt(0.25, Qt::blue);
	gradient->setColorAt(0.5, Qt::red);
	gradient->setColorAt(0.75, Qt::yellow);
	gradient->setColorAt(1, Qt::white);

	QImage gradient_img(gradient_steps, 1, QImage::Format_RGB888);
	
	painter.begin(&gradient_img);
	painter.fillRect(gradient_img.rect(), *gradient);
	painter.end();
	
	gradient_color = new QColor[gradient_steps]();
	
	for (int i = 0; i < gradient_steps; i++)
		gradient_color[i] = gradient_img.pixelColor(i, 0);
}

void Waterfall::resizeEvent(QResizeEvent *event) {
	delete chart_pix;
	
	chart_pix = new QPixmap(spectrogram->getNum(), height());
	chart_pix->fill(Qt::black);
}

void Waterfall::paintEvent(QPaintEvent *event) {
	chart_pix->scroll(0, 1, chart_pix->rect());

    painter.begin(chart_pix);

	for (int x = 0; x < chart_pix->width(); x++) {
		float v = (float) (psd_filter[x] - db_min) / (db_max - db_min);
		
		if (v < 0.0f) {
			v = 0.0f;
		} else if (v > 1.0f) {
			v = 1.0f;
		}
	
		int index = v * gradient_steps;
	
		pen.setColor(gradient_color[index]);
		painter.setPen(pen);
		painter.drawPoint(x, 0);
	}
    
    painter.end();

    painter.begin(this);
    painter.drawPixmap(0, 0, chart_pix->scaledToWidth(width(), Qt::SmoothTransformation));
    painter.end();
}

void Waterfall::calcFilter() {
	float *psd = spectrogram->getPsd();

	for (int i = 0; i < spectrogram->getNum(); i++)
		psd_filter[i] = durty ? psd[i] : psd_filter[i] * filter + psd[i] * (1.0f - filter);

	if (durty) {
		durty = false;
	}
}

void Waterfall::setFilter(float f) {
	filter = f;
	durty = true;
}
