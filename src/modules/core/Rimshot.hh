#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Rimshot_info.hh"
#include "util/math.hh"

namespace MetaModule
{

class Rimshot : public SmartCoreProcessor<RimshotInfo> {
	using Info = RimshotInfo;
	using enum Info::Elem;

public:
	Rimshot() = default;

	float highpass(float input,
				   float &prevInput1,
				   float &prevInput2,
				   float &prevOutput1,
				   float &prevOutput2,
				   float cutoffFreq,
				   float sampleRate,
				   float resonance) {
		// Calculate the filter coefficients for the 12dB/octave high-pass filter
		float omega = 2.0f * M_PI * cutoffFreq / sampleRate; // Angular frequency
		float sinOmega = sinf(omega);
		float cosOmega = cosf(omega);
		float alpha = sinOmega / (2.0f * resonance); // Q factor

		// Coefficients for the high-pass filter (resonant, 12dB per octave)
		float b0 = (1.0f + cosOmega) / 2.0f;
		float b1 = -(1.0f + cosOmega);
		float b2 = b0;
		float a0 = 1.0f + alpha;
		float a1 = -2.0f * cosOmega;
		float a2 = 1.0f - alpha;

		// Normalize coefficients
		b0 /= a0;
		b1 /= a0;
		b2 /= a0;
		a1 /= a0;
		a2 /= a0;

		// Apply the high-pass filter (biquad filter)
		float output = b0 * input + b1 * prevInput1 + b2 * prevInput2 - a1 * prevOutput1 - a2 * prevOutput2;

		// Update the filter state variables
		prevInput2 = prevInput1;
		prevInput1 = input;
		prevOutput2 = prevOutput1;
		prevOutput1 = output;

		return output;
	}

	void update(void) override {

		float cutoffControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float resonanceControl = combineKnobBipolarCV(getState<RingKnob>(), getInput<RingCvIn>());

		// Check if the trigger input is high
		bool currentTriggerState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge = !triggerStates[0] && currentTriggerState;
		triggerStates[0] = triggerStates[1];
		triggerStates[1] = currentTriggerState;

		trigTime = 5.0f; // Trigger pulse length of 5ms

		sampleTimeMs = (sampleRate * 1000.0f);

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

		highpassResonance = MathTools::map_value(resonanceControl, 0.f, 1.f, 1.f, 10.f);

		// Base frequency for high-pass filter
		hpCutoffFreq = MathTools::map_value(cutoffControl, 0.f, 1.f, 200.f, 2000.f);

		highpassOut =
			highpass(trigger, prevIn1, prevIn2, prevOut1, prevOut2, hpCutoffFreq, sampleRate, highpassResonance);

		finalOutput = (highpassOut * 3.f); // Give it some crunch
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<RimshotOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	// Amp decay envelope
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)

	// Trig
	bool pulseTriggered = false; // Flag to check if pulse was triggered

	// Output
	float finalOutput;

	// INTERFACE
	float timerMs = 0.0f;
	float sampleTimeMs = 0.0f;
	float trigTime = 0.0f;
	float trigger = 0.f;

	bool triggerStates[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state

	// High-pass filter variables
	float prevIn1 = 0.0f, prevIn2 = 0.0f;
	float prevOut1 = 0.0f, prevOut2 = 0.0f;

	float hpCutoffFreq = 0.f;
	float highpassOut = 0.f;
	float highpassResonance = 0.f;

	float sampleRate = 48000.0f;
};

} // namespace MetaModule
