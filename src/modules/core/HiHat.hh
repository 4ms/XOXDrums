#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "core/Biquad.hh"
#include "helpers/param_cv.hh"
#include "info/HiHat_info.hh"
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
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);
		if (input_id == static_cast<int>(ThicknessCvIn)) {
			recalc_hpfs();
		} else if (input_id == static_cast<int>(BrightnessCvIn)) {
			recalc_bpf();
		}
	}

	void update(void) override {

		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float decayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());

		// Check if the trigger input is high
		bool currentTriggerState1 = getInput<ClosedTrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge1 = !triggerStates1[0] && currentTriggerState1;
		triggerStates1[0] = triggerStates1[1];
		triggerStates1[1] = currentTriggerState1;

		bool currentTriggerState2 = getInput<OpenTrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge2 = !triggerStates2[0] && currentTriggerState2;
		triggerStates2[0] = triggerStates2[1];
		triggerStates2[1] = currentTriggerState2;

		// Square wave VCO x6 for two channels
		float frequency = MathTools::map_value(pitchControl, 0.f, 1.f, 1000.f, 2000.f); // Base frequency

		float oscSum = 0.f;

		for (auto i = 0u; i < offsets.size(); ++i) {
			phases[i] += (frequency + offsets[i]) * rSampleRate;
			phases[i] -= static_cast<int>(phases[i]);
			oscSum += (phases[i] < 0.5f) ? 1.0f : -1.0f;
		}

		oscSum = std::clamp(oscSum, -5.f, 5.f);

		// Bandpass
		float bandpassOut = bpf.process(oscSum);

		// Envelopes

		// Closed
		if (bangRisingEdge1) {
			envelopeValue1 = 1.0f;
		}

		float decay_ohh{};

		if (getState<ChokeSwitch>() == Toggle2posHoriz::State_t::LEFT) {
			constexpr auto min_seconds = 50.f / 1000.f;
			constexpr auto max_seconds = 250.f / 1000.f;
			const auto decayTimeOpen = MathTools::map_value(decayControl, 0.0f, 1.0f, min_seconds, max_seconds);
			decay_ohh = std::exp(-rSampleRate / decayTimeOpen);
		}

		envelopeValue2 *= decay_ohh;
		envelopeValue1 *= decay_chh;

		// Apply envelope to bandpass output
		float closedVCAOut = (bandpassOut * envelopeValue1);
		float openVCAOut = (bandpassOut * envelopeValue2);

		float closedHighpassOut = chh_hpf.process(closedVCAOut);
		float openHighpassOut = ohh_hpf.process(openVCAOut);

		float finalOutputClosed = (closedHighpassOut * finalMakeup); // Makeup gain
		float finalOutputOpen = (openHighpassOut * finalMakeup);	 // Makeup gain
		finalOutputClosed = std::clamp(finalOutputClosed, -5.f, 5.f);
		finalOutputOpen = std::clamp(finalOutputOpen, -5.f, 5.f);

		setOutput<ClosedOut>(finalOutputClosed);
		setOutput<OpenOut>(finalOutputOpen);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
		rSampleRate = 1.f / sampleRate;
		constexpr float decayTimeClosedMs = 10.f;
		decay_chh = std::exp(-1.0f / (sampleRate * (decayTimeClosedMs / 1000.0f)));
	}

private:
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

	// Oscillator
	static constexpr std::array<float, 6> offsets{
		100.f, 250.f, 400.f, 550.f, 600.f, 1000.f}; // Offsets for each oscillator
	std::array<float, 6> phases{};					// Phases for each oscillator

	// Bandpass Filter
	BiquadBPF bpf{};
	BiquadHPF chh_hpf{};
	BiquadHPF ohh_hpf{};

	float sampleRate{48000};
	float rSampleRate{1.f / 48000};

	float finalMakeup{1.f};

	// Decay envelopes
	float envelopeValue1 = 0.0f;

	float envelopeValue2 = 0.0f;

	float decay_chh{};

	bool triggerStates1[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state
	bool triggerStates2[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state
};

} // namespace MetaModule
