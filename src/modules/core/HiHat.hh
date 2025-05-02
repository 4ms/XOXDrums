#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "core/Biquad.hh"
#include "helpers/param_cv.hh"
#include "info/HiHat_info.hh"
#include "util/edge_detector.hh"
#include "util/math.hh"

namespace MetaModule
{

class HiHat : public SmartCoreProcessor<HiHatInfo> {
	using Info = HiHatInfo;
	using enum Info::Elem;

	static constexpr auto filter_q = 1.f;

public:
	HiHat() {
		bpf.setQ(filter_q);
		chh_hpf.setQ(filter_q);
		ohh_hpf.setQ(filter_q);
	}

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);
		if (param_id == static_cast<int>(ThicknessKnob)) {
			recalc_hpfs();
		} else if (param_id == static_cast<int>(BrightnessKnob)) {
			recalc_bpf();
		} else if (param_id == static_cast<int>(DecayKnob)) {
			recalc_decay();
		} else if (param_id == static_cast<int>(PitchKnob)) {
			recalc_freq();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);
		if (input_id == static_cast<int>(ThicknessCvIn)) {
			recalc_hpfs();
		} else if (input_id == static_cast<int>(BrightnessCvIn)) {
			recalc_bpf();
		} else if (input_id == static_cast<int>(DecayCvIn)) {
			recalc_decay();
		} else if (input_id == static_cast<int>(PitchCvIn)) {
			recalc_freq();
		}
	}

	void update(void) override {
		float decay_ohh_choked = decay_ohh;

		if (chh_trig.update(getInputAsGate<ChTrigIn>())) {
			envelopeValue1 = 1.0f;
			// Choke mode: Silence the open hihat decay if the closed hihat gets a trigger
			if (getState<ChokeSwitch>() == Toggle2pos::State_t::UP) {
				decay_ohh_choked = 0.f;
			}
		}

		if (ohh_trig.update(getInputAsGate<OhTrigIn>())) {
			envelopeValue2 = 1.0f;
		}

		// Square wave VCO x6 for two channels
		float oscSum = 0.f;

		for (auto i = 0u; i < offsets.size(); ++i) {
			phases[i] += phase_inc[i];
			phases[i] -= static_cast<int>(phases[i]);
			constexpr auto amp = 5.f / offsets.size();
			oscSum += (phases[i] < 0.5f) ? amp : -amp;
		}

		const float bandpassOut = bpf.process(oscSum);

		envelopeValue2 *= decay_ohh_choked;
		envelopeValue1 *= decay_chh;

		// Apply envelope to bandpass output
		float closedVCAOut = bandpassOut * envelopeValue1;
		float openVCAOut = bandpassOut * envelopeValue2;

		float closedHighpassOut = chh_hpf.process(closedVCAOut);
		float openHighpassOut = ohh_hpf.process(openVCAOut);

		float finalOutputClosed = (closedHighpassOut * finalMakeup); // Makeup gain
		float finalOutputOpen = (openHighpassOut * finalMakeup);	 // Makeup gain
		finalOutputClosed = std::clamp(finalOutputClosed, -5.f, 5.f);
		finalOutputOpen = std::clamp(finalOutputOpen, -5.f, 5.f);

		setOutput<ChOut>(finalOutputClosed);
		setOutput<OhOut>(finalOutputOpen);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
		constexpr float decayTimeClosedMs = 10.f;
		decay_chh = std::exp(-1.0f / (sampleRate * (decayTimeClosedMs / 1000.0f)));
		recalc_bpf();
		recalc_hpfs();
		recalc_decay();
		recalc_freq();
	}

private:
	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	void recalc_hpfs() {
		float thicknessControl = combineKnobBipolarCV(getState<ThicknessKnob>(), getInput<ThicknessCvIn>());

		finalMakeup = MathTools::map_value(thicknessControl, 0.f, 1.f, 5.0f, 1.0f);

		float hpCutoffFreq =
			MathTools::map_value(thicknessControl, 1.f, 0.f, 1000.f, 10000.f); // Base frequency for high-pass filter

		chh_hpf.setFc(hpCutoffFreq, sampleRate);
		ohh_hpf.setFc(hpCutoffFreq, sampleRate);
	}

	void recalc_bpf() {
		float brightnessControl = combineKnobBipolarCV(getState<BrightnessKnob>(), getInput<BrightnessCvIn>());
		float bandpassCutoffFrequency = MathTools::map_value(brightnessControl, 0.f, 1.f, 1000.f, 5000.f);
		bpf.setFc(bandpassCutoffFrequency, sampleRate);
	}

	void recalc_decay() {
		const auto decayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());
		constexpr auto min_seconds = 50.f / 1000.f;
		constexpr auto max_seconds = 250.f / 1000.f;
		const auto decayTimeOpen = MathTools::map_value(decayControl, 0.0f, 1.0f, min_seconds, max_seconds);
		decay_ohh = std::exp(-1.f / sampleRate / decayTimeOpen);
	}

	void recalc_freq() {
		const auto pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		const auto frequency = MathTools::map_value(pitchControl, 0.f, 1.f, 1000.f, 2000.f); // Base frequency
		const auto rSampleRate = 1.f / sampleRate;

		for (auto i = 0u; i < offsets.size(); ++i) {
			phase_inc[i] = (frequency + offsets[i]) * rSampleRate;
		}
	}

	// Oscillator
	static constexpr std::array<float, 6> offsets{
		100.f, 250.f, 400.f, 550.f, 600.f, 1000.f}; // Offsets for each oscillator
	std::array<float, 6> phases{};					// Phases for each oscillator
	std::array<float, 6> phase_inc{};

	// Bandpass Filter
	BiquadBPF bpf{};
	BiquadHPF chh_hpf{};
	BiquadHPF ohh_hpf{};

	float sampleRate{48000};

	float finalMakeup{1.f};

	// Decay envelopes
	float envelopeValue1 = 0.0f;

	float decay_ohh = 0.f;

	float envelopeValue2 = 0.0f;

	float decay_chh{};

	RisingEdgeDetector chh_trig{};
	RisingEdgeDetector ohh_trig{};
};

} // namespace MetaModule
