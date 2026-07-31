#pragma once

#include "CoreModules/SmartCoreProcessorPoly.hh"
#include "helpers/param_cv.hh"
#include "info/Accent_info.hh"
#include "util/edge_detector.hh"
#include <array>

namespace MetaModule
{

class Accent : public SmartCoreProcessorPoly<AccentInfo> {
	using Info = AccentInfo;
	using enum Info::Elem;

public:
	Accent() = default;

	void update(void) override {
		if (bypassed) { handle_bypass(); return; }
		auto pushButton = button.update(getState<TriggerButton>() == MomentaryButton::State_t::PRESSED);

		unsigned n = std::max({numChannels<TriggerIn>(), numChannels<AmountCvIn>(), numChannels<AudioIn>()});
		if (n == 0) n = 1;
		setChannels<AccentedOut>(n);

		for (unsigned c = 0; c < n; c++) {
			if (trig[c].update(getInputAsGate<TriggerIn>(c)) || pushButton) {
				amplitudeEnvelope[c] = 1.f;
				brightness = 1.f;
			}

			amplitudeEnvelope[c] *= ampDecayAlpha;

			const auto rawAmount = combineKnobBipolarCV(getState<AmountKnob>(), getInputNorm<AmountCvIn>(c));
			const auto accentAmount = 0.2f + (rawAmount * 0.9f);
			const auto scale = amplitudeEnvelope[c] * accentAmount + (1.f - accentAmount);

			const auto rawInput = getInputNorm<AudioIn>(c).value_or(0.f);
			setOutput<AccentedOut>(std::clamp(rawInput * scale, -5.f, 5.f), c);
		}

		brightness *= ledDecayAlpha;
		setLED<TriggerButton>(brightness);
	}

	void set_samplerate(float sr) override {
		constexpr auto ampDecayTime = 70.f;
		ampDecayAlpha = std::exp(-1.0f / (sr * (ampDecayTime / 1000.0f)));
		ledDecayAlpha = std::exp(-1.0f / (sr * 0.05f));
	}

private:
	// Returns the input value for channel c, clamping to the last valid channel for mono sources.
	template<Info::Elem EL>
	std::optional<float> getInputNorm(unsigned c) {
		auto nch = numChannels<EL>();
		if (nch == 0) return std::nullopt;
		return getInput<EL>(std::min(c, nch - 1u));
	}

	template<Info::Elem EL>
	bool getInputAsGate(unsigned c) {
		return getInputNorm<EL>(c).value_or(0.f) > 0.5f;
	}

	std::array<RisingEdgeDetector, MaxPolyChannels> trig{};
	RisingEdgeDetector button{};

	std::array<float, MaxPolyChannels> amplitudeEnvelope{};
	float ampDecayAlpha = 0.f;
	float ledDecayAlpha = 0.f;
	float brightness = 0.f;
};

} // namespace MetaModule
