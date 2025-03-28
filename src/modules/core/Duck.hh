#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Duck_info.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Duck : public SmartCoreProcessor<DuckInfo> {
	using Info = DuckInfo;
	using enum Info::Elem;

public:
	Duck() = default;

	void update(void) override {

		float amountControl = combineKnobBipolarCV(getState<AmountKnob>(), getInput<AmountCvIn>());
		float timeControl = combineKnobBipolarCV(getState<TimeKnob>(), getInput<TimeCvIn>());

		// Check if the trigger input is high
		bool currentTriggerState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge = !triggerStates[0] && currentTriggerState;
		triggerStates[0] = triggerStates[1];
		triggerStates[1] = currentTriggerState;

		if (bangRisingEdge) {
			amplitudeEnvelope = 1.0f;
		}

		// Envelopes
		float ampDecayTime = 50.0f + (timeControl * 1950.0f);
		float ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		float agc = MathTools::map_value(amountControl, 0.f, 1.f, 0.5f, 1.f);

		float scaled = (1.f - (amountControl)) + ((1.f - (amplitudeEnvelope)*amountControl));

		float VCAOut = (getInput<AudioIn>().value_or(0.f) * scaled) * agc;

		float finalOutput = VCAOut;
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);
		setOutput<DuckedOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	// Amp decay envelope
	float amplitudeEnvelope = 1.0f; // Envelope output value (for volume control)

	bool triggerStates[2] = {false, false};

	float sampleRate = 48000.0f;
};

} // namespace MetaModule
