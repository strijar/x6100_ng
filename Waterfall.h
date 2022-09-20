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

#include <QOpenGLWidget>
#include <QPixmap>
#include <QImage>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QLinearGradient>
#include "Spectrogram.h"

class Waterfall : public QOpenGLWidget
{
	Q_OBJECT
public:
	explicit Waterfall(Spectrogram *spectrogram, QWidget *parent = 0);
    void setFilter(float f);
    void calcFilter();
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event);

signals:

public slots:
	
private:
	Spectrogram		*spectrogram;
	float			*psd_filter;
	float			filter;
	bool			durty;

	QLinearGradient	*gradient;
	QColor			*gradient_color;
	int				gradient_steps;

	QPixmap			*chart_pix;
	QPainter 		painter;
	QPen 			pen;
	
	int				db_min = -110;
	int				db_max = -50;
};
