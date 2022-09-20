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

#include <QDebug>
#include "Spectrogram.h"

Spectrogram::Spectrogram() : QObject() {
	q = spgramcf_create(nfft, LIQUID_WINDOW_HANN, nfft, nfft / 8);
	
	psd = new float[nfft];
	psd_filter = new float[nfft];
	
	for (int i = 0; i < nfft; i++)
		psd_filter[i] = 0;
	
	buf = new std::complex<float>[nfft];
	durty = true;
}

Spectrogram::~Spectrogram() {
	spgramcf_destroy(q);
	
	delete[] buf;
	delete[] psd;
}

void Spectrogram::setFilter(float f) {
	filter = f;
	durty = true;
}

void Spectrogram::setSamples(const QVariantList& samples) {
	auto	it = samples.begin();
	int		index = 0;
	
	while (it != samples.end()) {
		buf[index].imag(it->toFloat()); it++;
		buf[index].real(it->toFloat());	it++;

		index++; 
	}

	spgramcf_write(q, buf, samples.size() / 2);
	spgramcf_get_psd(q, psd);

	for (int i = 0; i < nfft; i++)
		psd_filter[i] = durty ? psd[i] : psd_filter[i] * filter + psd[i] * (1.0f - filter);
	
	if (durty)
		durty = false;
	
	emit changed();
}

float* Spectrogram::getPsd() {
	return psd_filter;
}

unsigned int Spectrogram::getNum() {
	return nfft;
}

void Spectrogram::reset() {
	spgramcf_reset(q);
}
