#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/Biquad.hh"
#include "helpers/param_cv.hh"
#include "info/Maraca_info.hh"
#include "util/edge_detector.hh"

namespace MetaModule
{

class Maraca : public SmartCoreProcessor<MaracaInfo> {
	using Info = MaracaInfo;
	using enum Info::Elem;

	static constexpr auto highpass_q = 1.f;
	static constexpr auto highpass_fc = 6000.f;

public:
	Maraca() {
		hpf.setBiquad(highpass_fc, sampleRate, highpass_q);
	}

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);

		if (param_id == param_idx<DecayKnob>) {
			recalc_decay();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);

		if (input_id == input_idx<DecayCvIn>) {
			recalc_decay();
		}
	}

	void recalc_decay() {
		const auto decayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());
		const auto ampDecayTime = 2.5f + (decayControl * 10.0f); //
		ampDecayAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
	}

	void update(void) override {
		auto pushButton = button.update(getState<TriggerButton>() == MomentaryButton::State_t::PRESSED);

		if (trig.update(getInputAsGate<TriggerIn>() || pushButton)) {
			amplitudeEnvelope = 1.0f;
			brightness = 1.0f;
		}

		brightness *= ledDecayAlpha;
		setLED<TriggerButton>(brightness);

		amplitudeEnvelope *= ampDecayAlpha;

		float noise = (rand() / (float)RAND_MAX) * 10.0f - 5.0f;
		float VCAOut = (noise * amplitudeEnvelope);

		float highpassOut = hpf.process(VCAOut);

		float finalOutput = highpassOut * 0.75f; // Apply makeup gain post filter

		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<AudioOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
		ledDecayAlpha = std::exp(-1.0f / (sr * 0.05));
		hpf.setBiquad(highpass_fc, sampleRate, highpass_q);
		recalc_decay();
	}

private:
	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	BiquadHPF hpf{};

	// Amp decay envelope
	float amplitudeEnvelope = 0.0f; // Envelope output value (for volume control)
	float ampDecayAlpha = 0.f;

	float sampleRate{48000};

	RisingEdgeDetector trig{};
	RisingEdgeDetector button{};

	float ledDecayAlpha{};
	float brightness = 0.f;
};

} // namespace MetaModule
