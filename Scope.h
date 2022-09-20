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
#include <QPen>
#include <QBrush>
#include "Spectrogram.h"

class Scope : public QOpenGLWidget
{
	Q_OBJECT
public:
	explicit Scope(QWidget *parent = 0);
    void setSpectrogram(Spectrogram *spectrogram);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
signals:

public slots:
	
private:
	Spectrogram		*spectrogram;
	QBrush 			background;
	QPen 			lines;
	float			min_db = -110.0f;
	float			max_db = -50.0f;
};
