#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Cowbell_info.hh"
#include "util/edge_detector.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Cowbell : public SmartCoreProcessor<CowbellInfo> {
	using Info = CowbellInfo;
	using enum Info::Elem;

	static constexpr float highpassAlpha = 0.6f;
	static constexpr float lowpassAlpha = 0.1f;

public:
	Cowbell() = default;

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);

		if (param_id == param_idx<DecayKnob>) {
			recalc_decay();
		} else if (param_id == param_idx<PitchKnob>) {
			recalc_freq();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);

		if (input_id == input_idx<DecayCvIn>) {
			recalc_decay();
		} else if (input_id == input_idx<PitchCvIn>) {
			recalc_freq();
		}
	}

	void update(void) override {
		auto pushButton = getState<TriggerButton>() == MomentaryButton::State_t::PRESSED;

		if (trig.update(getInputAsGate<TriggerIn>() || pushButton)) {
			phase = 0.f;
			amplitudeEnvelope = 1.f;
			brightness = 1.f;
		}

		brightness *= ledDecayAlpha;
		setLED<TriggerButton>(brightness);

		amplitudeEnvelope *= ampDecayAlpha;

		// Osc 1
		using MathTools::M_PIF;
		phase += phase_inc_0;
		phase -= static_cast<int>(phase);
		float squareWave = (phase < .5f) ? 2.5f : -2.5f;

		// Osc 2
		phase_1 += phase_inc_1;
		phase_1 -= static_cast<int>(phase_1);
		float squareWave2 = (phase_1 < .5f) ? 2.5f : -2.5f;

		// Combine Oscillators
		float sumOutput = (squareWave + squareWave2) * amplitudeEnvelope;

		// Low-pass filter
		lowpassOutput = (lowpassAlpha * sumOutput) + ((1.0f - lowpassAlpha) * lowpassOutput);

		// High-pass filter (after low-pass)
		highpassOutput = highpassAlpha * (highpassOutput + lowpassOutput - prevHighpassOutput);
		prevHighpassOutput = lowpassOutput;

		float finalOutput = (highpassOutput * 5.0f);
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		// Output the final filtered signal
		setOutput<AudioOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
		ledDecayAlpha = std::exp(-1.0f / (sr * 0.05f));
		recalc_decay();
		recalc_freq();
	}

private:
	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	void recalc_decay() {
		float ampDecayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());
		float ampDecayTime = 20.0f + (ampDecayControl * 100.0f);
		ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
	}

	void recalc_freq() {
		const auto pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		const auto frequency = 500.0f + (pitchControl * 300.0f);
		const auto rSampleRate = 1.f / sampleRate;
		phase_inc_0 = frequency * rSampleRate * 2;

		const auto frequency2 = (frequency - 260.0f);
		phase_inc_1 = frequency2 * rSampleRate * 2;
	}

	float amplitudeEnvelope = 0.f;

	// Sine oscillator
	float phase = 0.0f;
	float phase_1 = 0.0f;
	float phase_inc_0 = 0.f;
	float phase_inc_1 = 0.f;

	float ampDecayAlpha = 0.f;

	// High-pass filter variables
	float lowpassOutput = 0.f;
	float highpassOutput = 0.f;
	float prevHighpassOutput = 0.0f;

	float sampleRate = 48000.f;

	RisingEdgeDetector trig{};

	float ledDecayAlpha = 0.f;
	float brightness = 0.f;
};

} // namespace MetaModule
