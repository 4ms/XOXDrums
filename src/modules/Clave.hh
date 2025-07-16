#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Clave_info.hh"
#include "util/edge_detector.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Clave : public SmartCoreProcessor<ClaveInfo> {
	using Info = ClaveInfo;
	using enum Info::Elem;

public:
	Clave() = default;

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);

		if (param_id == param_idx<DecayKnob>) {
			recalc_decay();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);

		if (input_id == input_idx<DecayCvIn>) {
			recalc_decay();
		}
	}

	void update(void) override {
		const float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		auto pushButton = getState<TriggerButton>() == MomentaryButton::State_t::PRESSED;

		if (trig.update(getInputAsGate<TriggerIn>() | pushButton)) {
			phase = 0.f;
			amplitudeEnvelope = 1.f;
		}

	
		if(pushButton || ((getInputAsGate<TriggerIn>()) > 0.5f)){
			setLED<TriggerButton>(1.f);
		}
		else {
			setLED<TriggerButton>(0.f);
		}

		// Osc
		using MathTools::M_PIF;
		const float frequency = 1000 + (pitchControl * 750.0f); // 1K -2K RANGE
		phase += frequency * rSampleRate * 2;
		phase -= static_cast<int>(phase);
		const float sineWave = 5.0f * std::sin(2 * M_PIF * phase) * amplitudeEnvelope;

		amplitudeEnvelope *= decayAlpha;

		const auto finalOutput = std::clamp(sineWave, -5.0f, 5.0f);

		setOutput<ClaveOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
		rSampleRate = 1.f / sampleRate;
	}

private:
	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	void recalc_decay() {
		const auto ampDecayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());
		const auto ampDecayTime = 5.0f + (ampDecayControl * 20.0f); // amp decay range (5ms - 25ms)
		decayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
	}

	// Sine oscillator
	float phase = 0.0f;
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)
	float sampleRate = 48000;
	float rSampleRate = 1 / 48000.f;

	float decayAlpha{};

	RisingEdgeDetector trig{};
};

} // namespace MetaModule
