#pragma once

#include "core/Biquad.hh"
#include "core/DrumBase.hh"
#include "helpers/param_cv.hh"
#include "info/Snare_info.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Snare : public DrumBase<SnareInfo> {
	using Info = SnareInfo;
	using enum Info::Elem;

	static constexpr auto filter_q = 1.f;

public:
	Snare() {
		bpf.setQ(filter_q);
	}

	void set_param(int param_id, float val) override {
		if (param_id == static_cast<int>(NoiseColorKnob)) {
			recalc_bpf = true;
		}
		SmartCoreProcessor::set_param(param_id, val);
	}

	void set_input(int input_id, float val) override {
		if (input_id == static_cast<int>(NoiseColorCvIn)) {
			recalc_bpf = true;
		}
		SmartCoreProcessor::set_input(input_id, val);
	}

	void update(void) override {

		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchIn>());
		float envDepthControl = combineKnobBipolarCV(getState<PitchEnvAmountKnob>(), getInput<PitchEnvAmountCvIn>());
		float pitchDecayControl = combineKnobBipolarCV(getState<PitchDecayKnob>(), getInput<PitchDecayCvIn>());
		float ampDecayControl = combineKnobBipolarCV(getState<BodyDecayKnob>(), getInput<BodyDecayCvIn>());
		float saturationControl = combineKnobBipolarCV(getState<SaturationKnob>(), getInput<SaturationCvIn>());
		float noiseVolumeControl = combineKnobBipolarCV(getState<Body_NoiseKnob>(), getInput<Body_NoiseCvIn>());
		if (recalc_bpf) {
			recalc_bpf = false;
			float noiseColorControl = combineKnobBipolarCV(getState<NoiseColorKnob>(), getInput<NoiseColorCvIn>());
			float cutoffFrequency = 1000.0f + (noiseColorControl * 5000.0f);
			float modulatedCutoffFrequency =
				cutoffFrequency + (pitchEnvelope * (envDepthControl * 5000.0f)); // Envelope depth range
			bpf.setFc(modulatedCutoffFrequency, sampleRate);
		}
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
		float frequency = 80 + (pitchControl * 100.0f);										 // Body pitch range
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
		float noise =
			(std::rand() / static_cast<float>(std::numeric_limits<decltype(std::rand())>::max())) * 10.0f - 5.0f;
		float filteredNoise = bpf.process(noise) * 2;
		filteredNoise = ((filteredNoise * noiseEnvelope) * noiseVolumeControl);
		filteredNoise = std::clamp(filteredNoise, -5.0f, 5.0f);

		switch (getState<RangeSwitch>()) {
			using enum Toggle3pos::State_t;
			case UP:
				saturation = 1 + (saturationControl * 100);
				break;
			case CENTER:
				saturation = 1 + (saturationControl * 10);
				break;
			case DOWN:
				saturation = 1 + (saturationControl * 2);
				break;
		}

		float finalOutput = ((sineWave * amplitudeEnvelope) + filteredNoise) * saturation;
		finalOutput = std::clamp(finalOutput, -2.5f, 2.5f);

		setOutput<SnareOut>(finalOutput);
	}

private:
	BiquadBPF bpf{};
	// Sine oscillator
	float phase = 0.0f;

	// Body decay envelope
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)
	float ampDecayTime = 5.0f;		// Decay time in ms (5ms as requested)

	// Pitch decay envelope
	float pitchEnvelope = 1.0f; // Envelope output value (for volume control)

	// Noise decay envelope
	float noiseEnvelope = 1.0f;

	// Trig
	bool lastBangState = false; // Previous state of the Bang input
	float pulseTime = 0.0f;		// Time tracking for pulse duration

	// Output
	float saturation = 0.0f;

	bool recalc_bpf{true};
};

} // namespace MetaModule
