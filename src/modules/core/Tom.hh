#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Tom_info.hh"

#include <cmath> // for sine wave
#define TWO_PI (2.0 * M_PI)

namespace MetaModule
{

class Tom : public SmartCoreProcessor<TomInfo> {
	using Info = TomInfo;
	using enum Info::Elem;

public:
	Tom() = default;

	float mapToRange(float value, float oldMin, float oldMax, float newMin, float newMax) {
		return newMin + (newMax - newMin) * ((value - oldMin) / (oldMax - oldMin));
	}

	void update(void) override {

		// Interface
		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float ampDecayControl = combineKnobBipolarCV(getState<AmpDecayKnob>(), getInput<AmpDecayCvIn>());
		float pitchDecayControl = combineKnobBipolarCV(getState<PitchDecayKnob>(), getInput<PitchDecayCvIn>());
		float envDepthControl = combineKnobBipolarCV(getState<EnvDepthKnob>(), getInput<EnvDepthCvIn>());

		bool currentBangState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge = !bangStates[0] && currentBangState;
		bangStates[0] = currentBangState;

		decayTimeAmp = mapToRange(ampDecayControl, 0.0f, 1.0f, 25.0f, 100.f);
		decayTimePitch = mapToRange(pitchDecayControl, 0.0f, 1.0f, 5.0f, 50.f);

		decayAlphaAmp = exp(-1.0f / (sampleRate * (decayTimeAmp / 1000.0f)));
		decayAlphaPitch = exp(-1.0f / (sampleRate * (decayTimePitch / 1000.0f)));

		if (bangRisingEdge) {
			pulseTriggered = true;
			envelopeValueAmp = 1.0f;
			envelopeValuePitch = 1.0f;
		}

		if (pulseTriggered) {
			envelopeValueAmp *= decayAlphaAmp;
			envelopeValuePitch *= decayAlphaPitch;
		} else {
			envelopeValueAmp = 0.0f;
			envelopeValuePitch = 0.0f;
		}

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
		float dt = 1.0f / sampleRate;
		modulatedFrequency = frequency + (envelopeValuePitch * (envDepthControl * 500.0f)); // Envelope depth range
		phase += modulatedFrequency * TWO_PI * dt;
		phase += frequency * TWO_PI * dt;
		if (phase >= TWO_PI) {
			phase -= TWO_PI;
		}
		float sineWave = 5.0f * sinf(phase);

		finalOutput = (sineWave * envelopeValueAmp);
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
	float modulatedFrequency;

	// Decay envelopes
	float decayAlphaAmp = 0.0f;
	float envelopeValueAmp = 0.0f;
	bool pulseTriggered = false;
	float decayTimeAmp = 0.0f; // Time in milliseconds for decay

	float decayAlphaPitch = 0.0f;
	float envelopeValuePitch = 0.0f;
	float decayTimePitch = 0.0f; // Time in milliseconds for decay

	float finalOutput = 0.f;

	bool bangStates[2] = {false, false};

	float sampleRate = 48000.0f;
};

} // namespace MetaModule
