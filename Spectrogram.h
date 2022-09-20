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

#include <QObject>
#include <QVariant>
#include <complex>
#include <liquid/liquid.h>

class Spectrogram : public QObject
{
	Q_OBJECT
	
public:
	Spectrogram();
	virtual ~Spectrogram();

	void setSamples(const QVariantList& samples);
	void setFilter(float f);
	void reset();
	
	float* getPsd();
	unsigned int getNum();
	
signals:	
	void changed();
	
private:
	spgramcf				q;
	
	unsigned int			nfft = 512;
	float					filter = 0.5f;
	bool					durty;
	std::complex<float>		*buf;
	float					*psd;
	float					*psd_filter;
};
