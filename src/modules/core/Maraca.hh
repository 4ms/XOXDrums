#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "core/Biquad.hh"
#include "helpers/param_cv.hh"
#include "info/Maraca_info.hh"
#include "util/math.hh"

namespace MetaModule
{

class Maraca : public SmartCoreProcessor<MaracaInfo> {
	using Info = MaracaInfo;
	using enum Info::Elem;

	static constexpr auto highpass_q = 1.f;

public:
	Maraca() {
		bpf.setQ(highpass_q);
	}

	float highpass(float input, float cutoffFreq) {
		bpf.setFc(cutoffFreq, sampleRate);
		return bpf.process(input);
	}

	void update(void) override {
		float noise = (rand() / (float)RAND_MAX) * 10.0f - 5.0f;
		float decayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());

		// Trig input
		bool bangState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		if (bangState) {
			amplitudeEnvelope = 1.0f;
		}
		lastBangState = bangState;

		// Envelopes
		float ampDecayTime = 2.5f + (decayControl * 10.0f); //
		float ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		float VCAOut = (noise * amplitudeEnvelope);

		float hpCutoffFreq = 6000.f; // Base frequency for high-pass filter
		float highpassOut = highpass(VCAOut, hpCutoffFreq);

		float finalOutput = (highpassOut * 0.75f); // Apply makeup gain post filter
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<MaracaOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	BiquadBPF bpf{};

	// Amp decay envelope
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)

	// Trig
	bool lastBangState = false; // Previous state of the Bang input

	float sampleRate = 48000.0f;
};

} // namespace MetaModule
