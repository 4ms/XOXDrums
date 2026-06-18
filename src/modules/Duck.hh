#pragma once

#include "CoreModules/SmartCoreProcessorPoly.hh"
#include "helpers/param_cv.hh"
#include "info/Duck_info.hh"
#include "util/edge_detector.hh"
#include "util/math.hh"
#include <array>
#include <cmath>

namespace MetaModule
{

class Duck : public SmartCoreProcessorPoly<DuckInfo> {
	using Info = DuckInfo;
	using enum Info::Elem;

public:
	Duck() = default;

	void update(void) override {
		if (bypassed) { handle_bypass(); return; }
		auto pushButton = button.update(getState<TriggerButton>() == MomentaryButton::State_t::PRESSED);

		unsigned n = std::max({numChannels<TriggerIn>(), numChannels<AmountCvIn>(),
		                       numChannels<TimeCvIn>(), numChannels<AudioIn>()});
		if (n == 0) n = 1;
		setChannels<AudioOut>(n);

		for (unsigned c = 0; c < n; c++) {
			// Only recompute the decay coefficient when the time CV actually changes.
			const auto timeControl = combineKnobBipolarCV(getState<TimeKnob>(), getInputNorm<TimeCvIn>(c));
			if (timeControl != cachedTimeCv[c]) {
				cachedTimeCv[c] = timeControl;
				const auto ampDecayTime = 50.0f + (timeControl * 1950.0f);
				ampDecayAlpha[c] = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
			}

			float amountControl = combineKnobBipolarCV(getState<AmountKnob>(), getInputNorm<AmountCvIn>(c));
			float agc = MathTools::map_value(amountControl, 0.f, 1.f, 0.5f, 1.f);

			if (trig[c].update(getInputAsGate<TriggerIn>(c)) || pushButton) {
				amplitudeEnvelope[c] = 1.0f;
				brightness = 1.f;
			}

			amplitudeEnvelope[c] *= ampDecayAlpha[c];

			float scaled = (1.f - amountControl) + ((1.f - amplitudeEnvelope[c]) * amountControl);
			float VCAOut = getInputNorm<AudioIn>(c).value_or(0.f) * scaled * agc;

			setOutput<AudioOut>(std::clamp(VCAOut, -5.0f, 5.0f), c);
		}

		brightness *= ledDecayAlpha;
		setLED<TriggerButton>(brightness);
	}

	void set_samplerate(float sr) override {
		ledDecayAlpha = std::exp(-1.0f / (sr * 0.05f));
		sampleRate = sr;
		cachedTimeCv.fill(-999.f); // invalidate cached decay coefficients
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

	std::array<float, MaxPolyChannels> amplitudeEnvelope{};
	float sampleRate = 48000.f;

	std::array<float, MaxPolyChannels> ampDecayAlpha{};
	std::array<float, MaxPolyChannels> cachedTimeCv{-999.f, -999.f, -999.f, -999.f};

	std::array<RisingEdgeDetector, MaxPolyChannels> trig{};
	RisingEdgeDetector button{};

	float ledDecayAlpha = 0.f;
	float brightness = 0.f;
};

} // namespace MetaModule
