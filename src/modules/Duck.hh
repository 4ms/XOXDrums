#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Duck_info.hh"
#include "util/edge_detector.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Duck : public SmartCoreProcessor<DuckInfo> {
	using Info = DuckInfo;
	using enum Info::Elem;

public:
	Duck() = default;

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);

		if (param_id == param_idx<TimeKnob>) {
			recalc_decay();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);

		if (input_id == input_idx<TimeCvIn>) {
			recalc_decay();
		}
	}

	void update(void) override {
		auto pushButton = getState<PushButton>() == MomentaryButton::State_t::PRESSED;

		float amountControl = combineKnobBipolarCV(getState<AmountKnob>(), getInput<AmountCvIn>());
		float agc = MathTools::map_value(amountControl, 0.f, 1.f, 0.5f, 1.f);

		if (trig.update(getInputAsGate<TriggerIn>() | pushButton)) {
			amplitudeEnvelope = 1.0f;
		}

		if(pushButton){
			setLED<PushButton>(1.f);
		}
		else {
			setLED<PushButton>(0.f);
		}
		
		amplitudeEnvelope *= ampDecayAlpha;

		float scaled = (1.f - (amountControl)) + ((1.f - (amplitudeEnvelope)*amountControl));

		float VCAOut = (getInput<AudioIn>().value_or(0.f) * scaled) * agc;

		float finalOutput = VCAOut;
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);
		setOutput<AudioOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
		recalc_decay();
	}

private:
	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	void recalc_decay() {
		const auto timeControl = combineKnobBipolarCV(getState<TimeKnob>(), getInput<TimeCvIn>());
		const auto ampDecayTime = 50.0f + (timeControl * 1950.0f);
		ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
	}

	float amplitudeEnvelope = 1.0f;
	float sampleRate{48000};
	float ampDecayAlpha{};

	RisingEdgeDetector trig{};
};

} // namespace MetaModule
