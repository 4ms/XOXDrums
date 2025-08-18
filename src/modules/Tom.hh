#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Tom_info.hh"
#include "util/edge_detector.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Tom : public SmartCoreProcessor<TomInfo> {
	using Info = TomInfo;
	using enum Info::Elem;

public:
	Tom() = default;

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);

		if (param_id == param_idx<AmpDecayKnob>) {
			recalc_amp_decay();
		} else if (param_id == param_idx<PitchDecayKnob>) {
			recalc_pitch_decay();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);

		if (input_id == input_idx<AmpDecayCvIn>) {
			recalc_amp_decay();
		} else if (input_id == input_idx<PitchDecayCvIn>) {
			recalc_pitch_decay();
		}
	}

	void update(void) override {
		// Interface
		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float envDepthControl = combineKnobBipolarCV(getState<EnvDepthKnob>(), getInput<EnvDepthCvIn>());
		auto pushButton = getState<TriggerButton>() == MomentaryButton::State_t::PRESSED;

		if (trig.update(getInputAsGate<TriggerIn>() || pushButton)) {
			envelopeValueAmp = 1.0f;
			envelopeValuePitch = 1.0f;
			brightness = 1.f;
		}

		brightness *= ledDecayAlpha;
		setLED<TriggerButton>(brightness);

		envelopeValueAmp *= decayAlphaAmp;
		envelopeValuePitch *= decayAlphaPitch;

		//Range switch
		// Final output
		switch (getState<PitchRangeSwitch>()) {
			using enum Toggle3pos::State_t;
			case UP:
				frequency = 160 + (pitchControl * 400);
				break;
			case CENTER:
				frequency = 80 + (pitchControl * 200);
				break;
			case DOWN:
				frequency = 40 + (pitchControl * 100);
				break;
		}

		// Osc
		using MathTools::M_PIF;

		float modulatedFrequency =
			frequency + (envelopeValuePitch * (envDepthControl * 500.0f)); // Envelope depth range
		phase += (frequency + modulatedFrequency) * rSampleRate;
		phase -= static_cast<int>(phase);
		float sineWave = 5.0f * std::sin(2 * M_PIF * phase);

		float finalOutput = (sineWave * envelopeValueAmp);
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<AudioOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		ledDecayAlpha = std::exp(-1.0f / (sr * 0.05f));
		sampleRate = sr;
		rSampleRate = 1.f / sampleRate;
		recalc_amp_decay();
		recalc_pitch_decay();
	}

private:
	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	float calc_decay_alpha(float time) {
		return std::exp(-1.0f / (sampleRate * (time / 1000.0f)));
	}

	void recalc_pitch_decay() {
		const auto pitchDecayControl = combineKnobBipolarCV(getState<PitchDecayKnob>(), getInput<PitchDecayCvIn>());
		const auto decayTimePitch = MathTools::map_value(pitchDecayControl, 0.0f, 1.0f, 5.0f, 50.f);
		decayAlphaPitch = calc_decay_alpha(decayTimePitch);
	}

	void recalc_amp_decay() {
		const auto ampDecayControl = combineKnobBipolarCV(getState<AmpDecayKnob>(), getInput<AmpDecayCvIn>());
		const auto decayTimeAmp = MathTools::map_value(ampDecayControl, 0.0f, 1.0f, 25.0f, 100.f);
		decayAlphaAmp = calc_decay_alpha(decayTimeAmp);
	}

	// Sine oscillator
	float phase = 0.0f;
	float frequency = 100.0f;

	// Decay envelopes
	float envelopeValueAmp = 0.0f;

	float envelopeValuePitch = 0.0f;

	float decayAlphaPitch = 0.f;
	float decayAlphaAmp = 0.f;

	float sampleRate = 48000.f;
	float rSampleRate = 1.f / 48000.f;

	RisingEdgeDetector trig{};

	float ledDecayAlpha = 0.f;
	float brightness = 0.f;
};

} // namespace MetaModule
