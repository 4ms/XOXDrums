#pragma once

#include "core/Biquad.hh"
#include "core/DrumBase.hh"
#include "helpers/param_cv.hh"
#include "info/Rimshot_info.hh"
#include "util/math.hh"

namespace MetaModule
{

class Rimshot : public DrumBase<RimshotInfo> {
	using Info = RimshotInfo;
	using enum Info::Elem;

public:
	Rimshot() = default;

	void set_param(int param_id, float val) override {
		if (param_id == static_cast<int>(PitchKnob)) {
			recalc_hpf = true;
		} else if (param_id == static_cast<int>(RingKnob)) {
			recalc_hpf = true;
		}
		SmartCoreProcessor::set_param(param_id, val);
	}

	void set_input(int input_id, float val) override {
		if (input_id == static_cast<int>(PitchCvIn)) {
			recalc_hpf = true;
		} else if (input_id == static_cast<int>(RingCvIn)) {
			recalc_hpf = true;
		}
		SmartCoreProcessor::set_input(input_id, val);
	}

	void update(void) override {

		if (recalc_hpf) {
			recalc_hpf = false;
			float cutoffControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
			float resonanceControl = combineKnobBipolarCV(getState<RingKnob>(), getInput<RingCvIn>());
			float highpassResonance = MathTools::map_value(resonanceControl, 0.f, 1.f, 1.f, 10.f);
			float hpCutoffFreq = MathTools::map_value(cutoffControl, 0.f, 1.f, 200.f, 2000.f);
			hpf.setBiquad(hpCutoffFreq, sampleRate, highpassResonance);
		}

		// Check if the trigger input is high
		bool currentTriggerState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge = !triggerStates[0] && currentTriggerState;
		triggerStates[0] = triggerStates[1];
		triggerStates[1] = currentTriggerState;

		float trigTime = 5.0f; // Trigger pulse length of 5ms

		float sampleTimeMs = (sampleRate * 1000.0f);

		// Trig input
		if (bangRisingEdge) {
			pulseTriggered = true;
			amplitudeEnvelope = 1.0f;
			timerMs = 0.0f;
		}

		if (pulseTriggered) {
			trigger = getInput<TrigIn>().value_or(0.f);
			timerMs += sampleTimeMs;
			if (timerMs >= trigTime) {
				pulseTriggered = false;
			}
		} else {
			trigger = 0.f;
		}

		float highpassOut = hpf.process(trigger);

		float finalOutput = (highpassOut * 3.f); // Give it some crunch
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<RimshotOut>(finalOutput);
	}

private:
	BiquadHPF hpf{};
	// Amp decay envelope
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)

	// Trig
	bool pulseTriggered = false; // Flag to check if pulse was triggered

	bool recalc_hpf;

	// INTERFACE
	float timerMs = 0.0f;
	float trigger = 0.f;

	bool triggerStates[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state
};

} // namespace MetaModule
