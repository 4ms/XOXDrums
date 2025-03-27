#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Clave_info.hh"

#include <cmath> // for sine wave

namespace MetaModule
{

class Clave : public SmartCoreProcessor<ClaveInfo> {
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
			pulseTriggered = true;
			amplitudeEnvelope = 1.0f;
		}

		// Osc
		float dt = 1.0f / sampleRate;
		float frequency = 1000 + (pitchControl * 750.0f); // 1K -2K RANGE
		phase += frequency * 2.f * M_PI * dt;
		if (phase >= 2.f * M_PI) {
			phase -= 2.f * M_PI;
		}
		float sineWave = 5.0f * sinf(phase);

		// Envelopes
		float ampDecayTime = 5.0f + (ampDecayControl * 20.0f); // amp decay range (5ms - 25ms)
		float ampDecayAlpha = exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		if (pulseTriggered) {
			if (amplitudeEnvelope < 0.0f) {
				amplitudeEnvelope = 0.0f;
				pulseTriggered = false;
			}
		} else {
			amplitudeEnvelope = 0.0f;
		}

		float finalOutput = (sineWave * amplitudeEnvelope);
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<ClaveOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	// Sine oscillator
	float phase = 0.0f;
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)
	bool pulseTriggered = false;	// Flag to check if pulse was triggered

	bool triggerStates[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state

	float sampleRate = 48000.0f;
};

} // namespace MetaModule
