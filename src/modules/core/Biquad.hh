//
//  Biquad.h
//
//  Created by Nigel Redmon on 11/24/12
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the Biquad code:
//  http://www.earlevel.com/main/2012/11/25/biquad-c-source-code/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code
//  for your own purposes, free or commercial.
//
// Modified by Cantrell Hepner @ 4ms 2025
// Make filter type a template param, remove all types but LPF, HPF, and BPF

#pragma once

#include "util/math.hh"
#include <math.h>

enum class BiquadType {
	LPF = 0,
	HPF,
	BPF,
};

template<BiquadType type>
class Biquad {
public:
	void setQ(float Q) {
		this->Q = Q;
		calcBiquad();
	}

	void setFc(float Fc, float sampleRate) {
		this->Fc = Fc / sampleRate;
		calcBiquad();
	}

	void setBiquad(float Fc, float sampleRate, float Q) {
		this->Q = Q;
		setFc(Fc, sampleRate);
	}

	float process(float in) {
		const auto out = in * a0 + z1;
		z1 = in * a1 + z2 - b1 * out;
		z2 = in * a2 - b2 * out;
		return out;
	}

private:
	void calcBiquad(void) {
		const auto K = std::tan(MathTools::M_PIF * Fc);
		const auto Ksqr = K * K;
		const auto KdivQ = K / Q;
		const auto norm = 1 / (1 + KdivQ + Ksqr);
		b2 = (1 - KdivQ + Ksqr) * norm;
		b1 = 2 * (Ksqr - 1) * norm;

		switch (type) {
			using enum BiquadType;
			case LPF:
				a0 = a2 = Ksqr * norm;
				a1 = 2 * a0;
				break;
			case HPF:
				a0 = a2 = norm;
				a1 = -2 * a0;
				break;
			case BPF:
				a0 = KdivQ * norm;
				a1 = 0;
				a2 = -a0;
				break;
		}
	}

	float a0;
	float a1;
	float a2;
	float b1;
	float b2;
	float Fc;
	float Q;
	float z1;
	float z2;
};

using BiquadLPF = Biquad<BiquadType::LPF>;
using BiquadHPF = Biquad<BiquadType::HPF>;
using BiquadBPF = Biquad<BiquadType::BPF>;
