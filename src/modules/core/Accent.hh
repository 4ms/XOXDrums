#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Accent_info.hh"

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

		// Check if the trigger input is high
		bool currentTriggerState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge = !triggerStates[0] && currentTriggerState;
		triggerStates[0] = triggerStates[1];
		triggerStates[1] = currentTriggerState;

		// Trigger handling
		if (bangRisingEdge) {
			amplitudeEnvelope = 1.0f;
		}

		float ampDecayTime = 70.f;
		float ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		float scaled = ((amplitudeEnvelope * controlValue) + (1.f - controlValue));

		float VCAOut = (getInput<AudioIn>().value_or(0.f) * scaled);
		float finalOutput = VCAOut;

		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<AccentedOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	float amplitudeEnvelope = 0.f;
	float sampleRate = 48000.0f;
	bool triggerStates[2] = {false, false};
};

} // namespace MetaModule
