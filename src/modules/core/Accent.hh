#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Accent_info.hh"
#include "util/edge_detector.hh"

namespace MetaModule
{

class Accent : public SmartCoreProcessor<AccentInfo> {
	using Info = AccentInfo;
	using enum Info::Elem;

public:
	Accent() = default;

	void update(void) override {

		float controlValue = combineKnobBipolarCV(getState<AmountKnob>(), getInput<AmountCvIn>());
		controlValue = 0.2f + (controlValue * 0.9f);

		if (trig.update(getInputAsGate<TrigIn>())) {
			amplitudeEnvelope = 1.f;
		}

		float ampDecayTime = 70.f;
		float ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		float scaled = ((amplitudeEnvelope * controlValue) + (1.f - controlValue));

		float VCAOut = (getInput<InputIn>().value_or(0.f) * scaled);
		float finalOutput = VCAOut;

		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<Out>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	RisingEdgeDetector trig{};

	float amplitudeEnvelope = 0.f;
	float sampleRate{48000};
};

} // namespace MetaModule
