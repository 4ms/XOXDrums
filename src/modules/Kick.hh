#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Kick_info.hh"
#include "util/edge_detector.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Kick : public SmartCoreProcessor<KickInfo> {
	using Info = KickInfo;
	using enum Info::Elem;

public:
	Kick() = default;

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);

		if (param_id == param_idx<AmpDecayKnob>) {
			recalc_amp_decay();
		} else if (param_id == param_idx<PitchDecayKnob>) {
			recalc_freq_decay();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);

		if (input_id == input_idx<AmpDecayCvIn>) {
			recalc_amp_decay();
		} else if (input_id == input_idx<PitchDecayCvIn>) {
			recalc_freq_decay();
		}
	}

	void update(void) override {
		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float envDepthControl = combineKnobBipolarCV(getState<PitchDepthKnob>(), getInput<DepthCvIn>());
		float saturationControl = combineKnobBipolarCV(getState<SaturationKnob>(), getInput<SaturationCvIn>());

		if (trig.update(getInputAsGate<TriggerIn>())) {
			phase = 0.0f; // reset sine phase for 0 crossing
			amplitudeEnvelope = 1.0f;
			pitchEnvelope = 1.0f;
		}

		// Osc
		using MathTools::M_PIF;
		float frequency = 10 + (pitchControl * 40.0f);										 // 10hz - 40hz range
		float modulatedFrequency = frequency + (pitchEnvelope * (envDepthControl * 500.0f)); // Envelope depth range
		phase += (frequency + modulatedFrequency) * rSampleRate;
		phase -= static_cast<int>(phase);
		float sineWave = 5.0f * std::sin(2 * M_PIF * phase);

		// Envelopes
		amplitudeEnvelope *= ampDecayAlpha;
		pitchEnvelope *= pitchDecayAlpha;

		// Final output
		switch (getState<RangeSwitch>()) {
			using enum Toggle3posHoriz::State_t;
			case RIGHT:
				saturation = 1 + (saturationControl * 100);
				break;
			case CENTER:
				saturation = 1 + (saturationControl * 10);
				break;
			case LEFT:
				saturation = 1 + (saturationControl * 2);
				break;
		}

		float finalOutput = (sineWave * amplitudeEnvelope) * saturation;
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<AudioOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
		rSampleRate = 1.f / sr;
		recalc_amp_decay();
		recalc_freq_decay();
	}

private:
	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	void recalc_amp_decay() {
		const auto ampDecayControl = combineKnobBipolarCV(getState<AmpDecayKnob>(), getInput<AmpDecayCvIn>());
		const auto ampDecayTime = 5.0f + (ampDecayControl * 300.0f); // amp decay range (5ms - 300ms)
		ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
	}

	void recalc_freq_decay() {
		const auto pitchDecayControl = combineKnobBipolarCV(getState<PitchDecayKnob>(), getInput<PitchDecayCvIn>());
		const auto pitchDecayTime = 5.0f + (pitchDecayControl * 30.0f); // pitch decay range (5ms - 30ms)
		pitchDecayAlpha = std::exp(-1.0f / (sampleRate * (pitchDecayTime / 1000.0f)));
	}

	// Sine oscillator
	float phase = 0.0f;

	// Amp decay envelope
	float amplitudeEnvelope = 0.f; // Envelope output value (for volume control)
	float ampDecayAlpha = 0.f;

	float sampleRate{48000};
	float rSampleRate{};

	// Pitch decay envelope
	float pitchEnvelope = 0.f; // Envelope output value (for volume control)
	float pitchDecayAlpha = 0.f;

	// Output
	float saturation = 0.0f;

	RisingEdgeDetector trig{};
};

} // namespace MetaModule
