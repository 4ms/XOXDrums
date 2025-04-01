#pragma once

#include "core/DrumBase.hh"
#include "helpers/param_cv.hh"
#include "info/Cowbell_info.hh"
#include "util/math.hh"
#include <cmath> // for sine wave

namespace MetaModule
{

class Cowbell : public DrumBase<CowbellInfo> {
	using Info = CowbellInfo;
	using enum Info::Elem;

public:
	Cowbell() = default;

	template<Info::Elem Knob, Info::Elem CV>
	float offset10vppSum() {
		float cvScale = (getInput<CV>().value_or(0.f) + 5.0f) / 10.0f;
		float cvSum = (getState<Knob>() + (cvScale - 0.5f));
		return std::clamp(cvSum, 0.0f, 1.0f);
	}

	void update(void) override {

		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float ampDecayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());

		// Check if the trigger input is high
		bool currentTriggerState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge = !triggerStates[0] && currentTriggerState;
		triggerStates[0] = triggerStates[1];
		triggerStates[1] = currentTriggerState;

		// Envelopes
		float ampDecayTime = 20.0f + (ampDecayControl * 100.0f);
		float ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		if (bangRisingEdge) {
			phase = 0.0f; // reset sine phase for 0 crossing
			amplitudeEnvelope = 1.0f;
		}

		// Osc 1
		using MathTools::M_PIF;
		float dt = 1.0f / sampleRate;
		float frequency = 500.0f + (pitchControl * 300.0f);
		phase += frequency * 2.f * M_PIF * dt;
		phase += frequency * 2.f * M_PIF * dt;
		if (phase >= 2.f * M_PIF) {
			phase -= 2.f * M_PIF;
		}
		float squareWave = (phase < M_PIF) ? 5.0f : -5.0f;

		// Osc 2
		float frequency2 = (frequency - 260.0f);
		phase2 += frequency2 * 2.f * M_PIF * dt;
		phase2 += frequency2 * 2.f * M_PIF * dt;
		if (phase2 >= 2.f * M_PIF) {
			phase2 -= 2.f * M_PIF;
		}
		float squareWave2 = (phase2 < M_PIF) ? 5.0f : -5.0f;

		// Combine Oscillators
		float sumOutput = ((squareWave + squareWave2) * 0.5f) * amplitudeEnvelope;
		sumOutput = std::clamp(sumOutput, -5.0f, 5.0f);

		// Low-pass filter
		lowpassOutput = (lowpassAlpha * sumOutput) + ((1.0f - lowpassAlpha) * lowpassOutput);

		// High-pass filter (after low-pass)
		highpassOutput = highpassAlpha * (highpassOutput + lowpassOutput - prevHighpassOutput);
		prevHighpassOutput = lowpassOutput;

		float finalOutput = (highpassOutput * 5.0f);
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		// Output the final filtered signal
		setOutput<CowbellOut>(finalOutput);
	}

private:
	// Amp decay envelope
	float amplitudeEnvelope = 1.0f;

	bool triggerStates[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state

	// Sine oscillator
	float phase = 0.0f;
	float phase2 = 0.0f;

	// Low-pass filter variables
	float lowpassAlpha = 0.1f;

	// High-pass filter variables
	float lowpassOutput = 0.f;
	float highpassOutput = 0.f;
	float highpassAlpha = 0.6f; // Filter cutoff highpass, lower number = more filtering
	float prevHighpassOutput = 0.0f;
};

} // namespace MetaModule
