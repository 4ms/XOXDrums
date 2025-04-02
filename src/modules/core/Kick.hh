#pragma once

#include "core/DrumBase.hh"
#include "helpers/param_cv.hh"
#include "info/Kick_info.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Kick : public DrumBase<KickInfo> {
	using Info = KickInfo;
	using enum Info::Elem;

public:
	Kick() = default;

	void update(void) override {

		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float pitchDecayControl = combineKnobBipolarCV(getState<PitchDecayKnob>(), getInput<PitchDecayCvIn>());
		float ampDecayControl = combineKnobBipolarCV(getState<AmpDecayKnob>(), getInput<AmpDecayCvIn>());
		float envDepthControl = combineKnobBipolarCV(getState<PitchDepthKnob>(), getInput<DepthCvIn>());
		float saturationControl = combineKnobBipolarCV(getState<SaturationKnob>(), getInput<SaturationCvIn>());

		// Trig input
		bool bangState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		if (bangState && !lastBangState) {
			phase = 0.0f; // reset sine phase for 0 crossing
			amplitudeEnvelope = 1.0f;
			pitchEnvelope = 1.0f;
			pulseTime = ampDecayTime * (sampleRate / 1000.0f);
		}
		lastBangState = bangState;

		// Osc
		using MathTools::M_PIF;
		float frequency = 10 + (pitchControl * 40.0f);										 // 10hz - 40hz range
		float modulatedFrequency = frequency + (pitchEnvelope * (envDepthControl * 500.0f)); // Envelope depth range
		phase += modulatedFrequency * rSampleRate;
		phase += frequency * rSampleRate;
		phase -= static_cast<int>(phase);
		float sineWave = 5.0f * std::sin(2 * M_PIF * phase);

		// Envelopes
		ampDecayTime = 5.0f + (ampDecayControl * 300.0f); // amp decay range (5ms - 300ms)
		float ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		float pitchDecayTime = 5.0f + (pitchDecayControl * 30.0f); // pitch decay range (5ms - 30ms)
		float pitchDecayAlpha = std::exp(-1.0f / (sampleRate * (pitchDecayTime / 1000.0f)));
		pitchEnvelope *= pitchDecayAlpha;

		// Final output
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

		float finalOutput = (sineWave * amplitudeEnvelope) * saturation;
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<KickOut>(finalOutput);
	}

private:
	// Sine oscillator
	float phase = 0.0f;

	// Amp decay envelope
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)
	float ampDecayTime = 5.0f;		// Decay time in ms (5ms as requested)

	// Pitch decay envelope
	float pitchEnvelope = 1.0f; // Envelope output value (for volume control)

	// Trig
	bool lastBangState = false; // Previous state of the Bang input
	float pulseTime = 0.0f;		// Time tracking for pulse duration

	// Output
	float saturation = 0.0f;
};

} // namespace MetaModule
