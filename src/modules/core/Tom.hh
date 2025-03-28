#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Tom_info.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Tom : public SmartCoreProcessor<TomInfo> {
	using Info = TomInfo;
	using enum Info::Elem;

public:
	Tom() = default;

	void update(void) override {

		// Interface
		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float ampDecayControl = combineKnobBipolarCV(getState<AmpDecayKnob>(), getInput<AmpDecayCvIn>());
		float pitchDecayControl = combineKnobBipolarCV(getState<PitchDecayKnob>(), getInput<PitchDecayCvIn>());
		float envDepthControl = combineKnobBipolarCV(getState<EnvDepthKnob>(), getInput<EnvDepthCvIn>());

		bool currentBangState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge = !bangStates[0] && currentBangState;
		bangStates[0] = currentBangState;

		float decayTimeAmp = MathTools::map_value(ampDecayControl, 0.0f, 1.0f, 25.0f, 100.f);
		float decayTimePitch = MathTools::map_value(pitchDecayControl, 0.0f, 1.0f, 5.0f, 50.f);

		float decayAlphaAmp = std::exp(-1.0f / (sampleRate * (decayTimeAmp / 1000.0f)));
		float decayAlphaPitch = std::exp(-1.0f / (sampleRate * (decayTimePitch / 1000.0f)));

		if (bangRisingEdge) {
			envelopeValueAmp = 1.0f;
			envelopeValuePitch = 1.0f;
		}

		envelopeValueAmp *= decayAlphaAmp;
		envelopeValuePitch *= decayAlphaPitch;

		//Range switch
		// Final output
		if (getState<RangeSwitch>() == Toggle3pos::State_t::UP) {
			frequency = 160 + (pitchControl * 400.0f);
		}
		if (getState<RangeSwitch>() == Toggle3pos::State_t::CENTER) {
			frequency = 80 + (pitchControl * 200.0f);
		}
		if (getState<RangeSwitch>() == Toggle3pos::State_t::DOWN) {
			frequency = 40 + (pitchControl * 100.0f);
		}

		// Osc
		using MathTools::M_PIF;

		float dt = 1.0f / sampleRate;
		float modulatedFrequency =
			frequency + (envelopeValuePitch * (envDepthControl * 500.0f)); // Envelope depth range
		phase += modulatedFrequency * 2.f * M_PIF * dt;
		phase += frequency * 2.f * M_PIF * dt;
		if (phase >= 2.f * M_PIF) {
			phase -= 2.f * M_PIF;
		}
		float sineWave = 5.0f * sinf(phase);

		float finalOutput = (sineWave * envelopeValueAmp);
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<TomOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	// Sine oscillator
	float phase = 0.0f;
	float frequency = 100.0f;

	// Decay envelopes
	float envelopeValueAmp = 0.0f;

	float envelopeValuePitch = 0.0f;
	
	bool bangStates[2] = {false, false};

	float sampleRate = 48000.0f;
};

} // namespace MetaModule
