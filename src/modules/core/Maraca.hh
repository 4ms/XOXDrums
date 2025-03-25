#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Maraca_info.hh"

namespace MetaModule
{

class Maraca : public SmartCoreProcessor<MaracaInfo> {
	using Info = MaracaInfo;
	using enum Info::Elem;

public:
	Maraca() = default;

	float highpass(float input,
				   float &prevInput1,
				   float &prevInput2,
				   float &prevOutput1,
				   float &prevOutput2,
				   float cutoffFreq,
				   float sampleRate1,
				   float resonance) {
		// Calculate the filter coefficients for the 12dB/octave high-pass filter
		float omega = 2.0f * M_PI * cutoffFreq / sampleRate1; // Angular frequency
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
		float noise = (rand() / (float)RAND_MAX) * 10.0f - 5.0f;
		float decayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());

		// Trig input
		bool bangState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		if (bangState) {
			pulseTriggered = true;
			amplitudeEnvelope = 1.0f;
		}
		lastBangState = bangState;

		// Envelopes
		ampDecayTime = 2.5f + (decayControl * 10.0f); //
		ampDecayAlpha = exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		if (pulseTriggered) {
			if (amplitudeEnvelope < 0.0f) {
				amplitudeEnvelope = 0.0f;
				pulseTriggered = false;
			}
		} else {
			amplitudeEnvelope = 0.0f;
		}

		VCAOut = (noise * amplitudeEnvelope);

		highpassResonance = 1.f;
		hpCutoffFreq = 6000.f; // Base frequency for high-pass filter
		highpassOut =
			highpass(VCAOut, prevIn1, prevIn2, prevOut1, prevOut2, hpCutoffFreq, sampleRate, highpassResonance);

		finalOutput = (highpassOut * 0.75f); // Apply makeup gain post filter
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<MaracaOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	// Amp decay envelope
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)
	float ampDecayTime = 5.0f;		// Decay time in ms (5ms as requested)
	float ampDecayAlpha = 0.0f;		// Exponential decay coefficient

	// Trig
	bool pulseTriggered = false; // Flag to check if pulse was triggered
	bool lastBangState = false;	 // Previous state of the Bang input

	// Output
	float VCAOut = 0.f;
	float finalOutput = 0.f;

	// High-pass filter variables
	float prevIn1 = 0.0f, prevIn2 = 0.0f;
	float prevOut1 = 0.0f, prevOut2 = 0.0f;

	float hpCutoffFreq = 0.f;
	float highpassOut = 0.f;
	float highpassResonance = 0.f;

	float sampleRate = 48000.0f;
};

} // namespace MetaModule
