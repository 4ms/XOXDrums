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

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);
		if (param_id == static_cast<int>(AmountKnob)) {
			recalc_decay();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);
		if (input_id == static_cast<int>(AmountCvIn)) {
			recalc_decay();
		}
	}

	void update(void) override {
		if (trig.update(getInputAsGate<TriggerIn>())) {
			amplitudeEnvelope = 1.f;
		}

		amplitudeEnvelope *= ampDecayAlpha;

		const auto scale = amplitudeEnvelope * amount + (1.f - amount);

		const auto rawInput = getInput<InputIn>().value_or(0.f);

		setOutput<Out>(std::clamp(rawInput * scale, -5.f, 5.f));
	}

	void set_samplerate(float sr) override {
		constexpr auto ampDecayTime = 70.f;
		ampDecayAlpha = std::exp(-1.0f / (sr * (ampDecayTime / 1000.0f)));
	}

private:
	void recalc_decay() {
		const auto rawAmount = combineKnobBipolarCV(getState<AmountKnob>(), getInput<AmountCvIn>());
		amount = 0.2f + (rawAmount * 0.9f);
	}

	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	RisingEdgeDetector trig{};

	float amplitudeEnvelope = 0.f;
	float ampDecayAlpha{};
	float amount{1.f};
};

} // namespace MetaModule
