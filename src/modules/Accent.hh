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

		if (param_id == param_idx<AmountKnob>) {
			recalc_decay();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);

		if (input_id == input_idx<AmountCvIn>) {
			recalc_decay();
		}
	}

	void update(void) override {
		auto pushButton = button.update(getState<TriggerButton>() == MomentaryButton::State_t::PRESSED);

		if (trig.update(getInputAsGate<TriggerIn>() || pushButton)) {
			amplitudeEnvelope = 1.f;
			brightness = 1.f;
		}

		if (brightness > 0.004f) {
			brightness *= ledDecayAlpha;
		}
		setLED<TriggerButton>(brightness);

		amplitudeEnvelope *= ampDecayAlpha;

		const auto scale = amplitudeEnvelope * accentAmount + (1.f - accentAmount);

		const auto rawInput = getInput<AudioIn>().value_or(0.f);

		setOutput<AccentedOut>(std::clamp(rawInput * scale, -5.f, 5.f));
	}

	void set_samplerate(float sr) override {
		constexpr auto ampDecayTime = 70.f;
		ampDecayAlpha = std::exp(-1.0f / (sr * (ampDecayTime / 1000.0f)));
		ledDecayAlpha = std::exp(-1.0f / (sr * 0.05f));
	}

private:
	void recalc_decay() {
		const auto rawAmount = combineKnobBipolarCV(getState<AmountKnob>(), getInput<AmountCvIn>());
		accentAmount = 0.2f + (rawAmount * 0.9f);
	}

	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	RisingEdgeDetector trig{};
	RisingEdgeDetector button{};

	float amplitudeEnvelope = 0.f;
	float ampDecayAlpha = 0.f;
	float ledDecayAlpha = 0.f;
	float accentAmount = 1.f;
	float brightness = 0.f;
};

} // namespace MetaModule
