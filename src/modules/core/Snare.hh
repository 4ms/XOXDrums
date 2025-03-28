#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Snare_info.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Snare : public SmartCoreProcessor<SnareInfo> {
	using Info = SnareInfo;
	using enum Info::Elem;

public:
	Snare() = default;

	float biquadBandpassFilter1(float input, float cutoff, float sampleRate1) {
		// Calculate the filter coefficients for the bandpass filter (using cutoff and resonance)
		const auto omega = 2.0f * MathTools::M_PIF * cutoff / sampleRate1;
		float sn = std::sin(omega);
		float cs = std::cos(omega);
		float alpha = sn / (2.0f); // Resonance is last number

		// Compute the bandpass filter coefficients (Biquad)
		filterB0 = alpha;
		filterB1 = 0.0f;
		filterB2 = -alpha;
		filterA0 = 1.0f + alpha;
		filterA1 = -2.0f * cs;
		filterA2 = 1.0f - alpha;

		// Normalize coefficients
		filterB0 /= filterA0;
		filterB1 /= filterA0;
		filterB2 /= filterA0;
		filterA1 /= filterA0;
		filterA2 /= filterA0;

		// Apply the filter to the input signal
		float output =
			filterB0 * input + filterB1 * filterX1 + filterB2 * filterX2 - filterA1 * filterY1 - filterA2 * filterY2;

		// Update the filter states
		filterX2 = filterX1;
		filterX1 = input;
		filterY2 = filterY1;
		filterY1 = output;

		return output;
	}

	void update(void) override {

		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchIn>());
		float envDepthControl = combineKnobBipolarCV(getState<PitchEnvAmountKnob>(), getInput<PitchEnvAmountCvIn>());
		float pitchDecayControl = combineKnobBipolarCV(getState<PitchDecayKnob>(), getInput<PitchDecayCvIn>());
		float ampDecayControl = combineKnobBipolarCV(getState<BodyDecayKnob>(), getInput<BodyDecayCvIn>());
		float saturationControl = combineKnobBipolarCV(getState<SaturationKnob>(), getInput<SaturationCvIn>());
		float noiseVolumeControl = combineKnobBipolarCV(getState<Body_NoiseKnob>(), getInput<Body_NoiseCvIn>());
		float noiseColorControl = combineKnobBipolarCV(getState<NoiseColorKnob>(), getInput<NoiseColorCvIn>());
		float noiseDecayControl = combineKnobBipolarCV(getState<NoiseDecayKnob>(), getInput<NoiseDecayCvIn>());

		// Trig input
		bool bangState = getInput<TriggerIn>().value_or(0.f) > 0.5f;
		if (bangState && !lastBangState) {
			phase = 0.0f; // reset sine phase for 0 crossing
			amplitudeEnvelope = 1.0f;
			pitchEnvelope = 1.0f;
			noiseEnvelope = 1.0f;
			pulseTime = ampDecayTime * (sampleRate / 1000.0f);
		}
		lastBangState = bangState;

		// Osc
		using MathTools::M_PIF;
		float dt = 1.0f / sampleRate;
		float frequency = 80 + (pitchControl * 100.0f);									   // Body pitch range
		float modulatedFrequency = frequency + (pitchEnvelope * (envDepthControl * 500.0f)); // Envelope depth range
		phase += modulatedFrequency * 2.f * M_PIF * dt;
		phase += frequency * 2.f * M_PIF * dt;
		if (phase >= 2.f * M_PIF) {
			phase -= 2.f * M_PIF;
		}
		float sineWave = 5.0f * std::sin(phase);
		sineWave = (sineWave * ((1.f - noiseVolumeControl) / 2.f));

		// Envelopes
		ampDecayTime = 5.0f + (ampDecayControl * 50.0f); // amp decay range
		float ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		float pitchDecayTime = 5.0f + (pitchDecayControl * 30.0f); // pitch decay range
		float pitchDecayAlpha = std::exp(-1.0f / (sampleRate * (pitchDecayTime / 1000.0f)));
		pitchEnvelope *= pitchDecayAlpha;

		float noiseDecayTime = 5.0f + (noiseDecayControl * 75.0f); // pitch decay range
		float noiseDecayAlpha = std::exp(-1.0f / (sampleRate * (noiseDecayTime / 1000.0f)));
		noiseEnvelope *= noiseDecayAlpha;

		// Noise + filter
		float noise = (rand() / (float)RAND_MAX) * 10.0f - 5.0f;
		float cutoffFrequency = 1000.0f + (noiseColorControl * 5000.0f);
		float modulatedCutoffFrequency =
			cutoffFrequency + (pitchEnvelope * (envDepthControl * 5000.0f)); // Envelope depth range
		float filteredNoise = (biquadBandpassFilter1(noise, modulatedCutoffFrequency, sampleRate)) * 2.0f;
		filteredNoise = ((filteredNoise * noiseEnvelope) * noiseVolumeControl);
		filteredNoise = std::clamp(filteredNoise, -5.0f, 5.0f);

		if (getState<RangeSwitch>() == Toggle3pos::State_t::UP) {
			saturation = 1 + (saturationControl * 100);
		}
		if (getState<RangeSwitch>() == Toggle3pos::State_t::CENTER) {
			saturation = 1 + (saturationControl * 10);
		}
		if (getState<RangeSwitch>() == Toggle3pos::State_t::DOWN) {
			saturation = 1 + (saturationControl * 2);
		}

		float finalOutput = ((sineWave * amplitudeEnvelope) + filteredNoise) * saturation;
		finalOutput = std::clamp(finalOutput, -2.5f, 2.5f);

		setOutput<SnareOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	// Sine oscillator
	float phase = 0.0f;

	// Body decay envelope
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)
	float ampDecayTime = 5.0f;		// Decay time in ms (5ms as requested)

	// Pitch decay envelope
	float pitchEnvelope = 1.0f;	  // Envelope output value (for volume control)

	// Noise decay envelope
	float noiseEnvelope = 1.0f;

	// Trig
	bool lastBangState = false;	 // Previous state of the Bang input
	float pulseTime = 0.0f;		 // Time tracking for pulse duration

	// Output
	float saturation = 0.0f;

	// Bandpass
	float filterB0 = 0.0f, filterB1 = 0.0f, filterB2 = 0.0f, filterA0 = 0.0f, filterA1 = 0.0f, filterA2 = 0.0f;
	float filterX1 = 0.0f, filterX2 = 0.0f, filterY1 = 0.0f, filterY2 = 0.0f;

	float sampleRate = 48000.0f;
};

} // namespace MetaModule
