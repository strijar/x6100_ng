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
	nfft = 800;

	q = spgramcf_create(nfft, LIQUID_WINDOW_HANN, nfft, nfft / 2);
	
	psd = new float[nfft];
	buf = new std::complex<float>[nfft];
	mutex = new QMutex();
	
	for (int i = 0; i < nfft; i++)
		psd[i] = -130.0f;
}

Spectrogram::~Spectrogram() {
	spgramcf_destroy(q);
	
	delete[] buf;
	delete[] psd;
	
	delete mutex;
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
	
	mutex->lock();
	spgramcf_get_psd(q, psd);
	mutex->unlock();
}

float* Spectrogram::getPsd() {
	return psd;
}

unsigned int Spectrogram::getNum() {
	return nfft;
}

void Spectrogram::reset() {
	spgramcf_reset(q);
}

void Spectrogram::lock() {
	mutex->lock();
}

void Spectrogram::unlock() {
	mutex->unlock();
}
