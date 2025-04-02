#pragma once

#include "core/DrumBase.hh"
#include "helpers/param_cv.hh"
#include "info/Clave_info.hh"
#include "util/math.hh"

#include <cmath> // for sine wave

namespace MetaModule
{

class Clave : public DrumBase<ClaveInfo> {
	using Info = ClaveInfo;
	using enum Info::Elem;

public:
	Clave() = default;

	void update(void) override {

		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float ampDecayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());

		// Check if the trigger input is high
		bool currentTriggerState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge = !triggerStates[0] && currentTriggerState;
		triggerStates[0] = triggerStates[1];
		triggerStates[1] = currentTriggerState;

		// Trig input
		if (bangRisingEdge) {
			phase = 0.0f; // reset sine phase for 0 crossing
			amplitudeEnvelope = 1.0f;
		}

		// Osc
		using MathTools::M_PIF;
		float frequency = 1000 + (pitchControl * 750.0f); // 1K -2K RANGE
		phase += frequency * rSampleRate * 2;
		phase -= static_cast<int>(phase);
		float sineWave = 5.0f * sinf(2 * M_PIF * phase);

		// Envelopes
		float ampDecayTime = 5.0f + (ampDecayControl * 20.0f); // amp decay range (5ms - 25ms)
		float ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		float finalOutput = (sineWave * amplitudeEnvelope);
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<ClaveOut>(finalOutput);
	}

private:
	// Sine oscillator
	float phase = 0.0f;
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)

	bool triggerStates[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state
};

} // namespace MetaModule
