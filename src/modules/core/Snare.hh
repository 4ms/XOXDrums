#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "core/Biquad.hh"
#include "helpers/param_cv.hh"
#include "info/Snare_info.hh"
#include "util/edge_detector.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Snare : public SmartCoreProcessor<SnareInfo> {
	using Info = SnareInfo;
	using enum Info::Elem;

	static constexpr auto filter_q = 1.f;

public:
	Snare() {
		bpf.setQ(filter_q);
	}

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);

		if (param_id == param_idx<BodyDecayKnob>) {
			recalc_amp_decay();
		} else if (param_id == param_idx<PitchDecayKnob>) {
			recalc_pitch_decay();
		} else if (param_id == param_idx<NoiseDecayKnob>) {
			recalc_noise_decay();
		} else if (param_id == param_idx<SaturationKnob> || param_id == param_idx<RangeSwitch>) {
			recalc_saturation();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);

		if (input_id == input_idx<BDecayCvIn>) {
			recalc_amp_decay();
		} else if (input_id == input_idx<PDecayCvIn>) {
			recalc_pitch_decay();
		} else if (input_id == input_idx<NDecayCvIn>) {
			recalc_noise_decay();
		} else if (input_id == input_idx<SaturationCvIn>) {
			recalc_saturation();
		}
	}

	void update(void) override {
		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float envDepthControl = combineKnobBipolarCV(getState<PitchEnvAmountKnob>(), getInput<PAmtCvIn>());
		float noiseVolumeControl = combineKnobBipolarCV(getState<BodyNoiseKnob>(), getInput<BnCvIn>());
		float noiseColorControl = combineKnobBipolarCV(getState<NoiseColorKnob>(), getInput<NColorCvIn>());

		if (trig.update(getInputAsGate<TriggerIn>())) {
			phase = 0.0f; // reset sine phase for 0 crossing
			amplitudeEnvelope = 1.0f;
			pitchEnvelope = 1.0f;
			noiseEnvelope = 1.0f;
		}

		// Osc
		using MathTools::M_PIF;
		float frequency = 80 + (pitchControl * 100.0f);										 // Body pitch range
		float modulatedFrequency = frequency + (pitchEnvelope * (envDepthControl * 500.0f)); // Envelope depth range
		phase += (frequency + modulatedFrequency) * rSampleRate;
		phase -= static_cast<int>(phase);
		float sineWave = 5.0f * std::sin(2 * M_PIF * phase);
		sineWave = (sineWave * ((1.f - noiseVolumeControl) / 2.f));

		amplitudeEnvelope *= ampDecayAlpha;
		pitchEnvelope *= pitchDecayAlpha;
		noiseEnvelope *= noiseDecayAlpha;

		// Noise + filter
		float cutoffFrequency = 1000.0f + (noiseColorControl * 5000.0f);
		float modulatedCutoffFrequency = cutoffFrequency + (pitchEnvelope * (envDepthControl * 5000.0f));
		float noise = (std::rand() / static_cast<float>(RAND_MAX)) * 10.0f - 5.0f;
		bpf.setFc(modulatedCutoffFrequency, sampleRate);
		float filteredNoise = bpf.process(noise) * 2;
		filteredNoise = ((filteredNoise * noiseEnvelope) * noiseVolumeControl);
		filteredNoise = std::clamp(filteredNoise, -5.0f, 5.0f);

		float finalOutput = ((sineWave * amplitudeEnvelope) + filteredNoise) * saturation;
		finalOutput = std::clamp(finalOutput, -2.5f, 2.5f);

		setOutput<Out>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
		rSampleRate = 1.f / sampleRate;
		recalc_amp_decay();
		recalc_pitch_decay();
		recalc_noise_decay();
		recalc_saturation();
	}

private:
	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	float calc_decay_alpha(float time) {
		return std::exp(-1.0f / (sampleRate * (time / 1000.0f)));
	}

	void recalc_amp_decay() {
		const auto ampDecayControl = combineKnobBipolarCV(getState<BodyDecayKnob>(), getInput<BDecayCvIn>());
		const auto ampDecayTime = 5.0f + (ampDecayControl * 50.0f); // amp decay range
		ampDecayAlpha = calc_decay_alpha(ampDecayTime);
	}

	void recalc_pitch_decay() {
		const auto pitchDecayControl = combineKnobBipolarCV(getState<PitchDecayKnob>(), getInput<PDecayCvIn>());
		const auto pitchDecayTime = 5.0f + (pitchDecayControl * 30.0f); // pitch decay range
		pitchDecayAlpha = calc_decay_alpha(pitchDecayTime);
	}

	void recalc_noise_decay() {
		const auto noiseDecayControl = combineKnobBipolarCV(getState<NoiseDecayKnob>(), getInput<NDecayCvIn>());
		const auto noiseDecayTime = 5.0f + (noiseDecayControl * 75.0f); // pitch decay range
		noiseDecayAlpha = calc_decay_alpha(noiseDecayTime);
	}

	void recalc_saturation() {
		const auto saturationControl = combineKnobBipolarCV(getState<SaturationKnob>(), getInput<SaturationCvIn>());
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
	}

	BiquadBPF bpf{};
	// Sine oscillator
	float phase = 0.0f;

	// Body decay envelope
	float amplitudeEnvelope = 0.0f; // Envelope output value (for volume control)
	float ampDecayAlpha = 0.0f;		// Decay time in ms (5ms as requested)

	float sampleRate{48000};
	float rSampleRate{1.f / 48000};

	// Pitch decay envelope
	float pitchEnvelope = 0.0f;
	float pitchDecayAlpha = 0.f;

	// Noise decay envelope
	float noiseEnvelope = 0.0f;
	float noiseDecayAlpha = 0.f;

	// Output
	float saturation = 0.0f;

	RisingEdgeDetector trig{};
};

} // namespace MetaModule
