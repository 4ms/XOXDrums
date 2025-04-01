#pragma once

#include "core/Biquad.hh"
#include "core/DrumBase.hh"
#include "debug_raw.h"
#include "helpers/param_cv.hh"
#include "info/HiHat_info.hh"
#include "util/math.hh"

namespace MetaModule
{

class HiHat : public DrumBase<HiHatInfo> {
	using Info = HiHatInfo;
	using enum Info::Elem;

	static constexpr auto filter_q = 1.f;

public:
	HiHat() {
		bpf.setQ(filter_q);
		chh_hpf.setQ(filter_q);
		ohh_hpf.setQ(filter_q);
	}

	float biquadBandpassFilter(float input, float cutoff) {
		bpf.setFc(cutoff, sampleRate);
		return bpf.process(input);
	}

	float closedHighpass(float input, float cutoffFreq) {
		chh_hpf.setFc(cutoffFreq, sampleRate);
		return chh_hpf.process(input);
	}

	// Open high-pass filter with different variable names
	float openHighpass(float input, float cutoffFreq) {
		ohh_hpf.setFc(cutoffFreq, sampleRate);
		return ohh_hpf.process(input);
	}

	void update(void) override {

		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		float decayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());
		float thicknessControl = combineKnobBipolarCV(getState<ThicknessKnob>(), getInput<ThicknessCvIn>());
		float brightnessControl = combineKnobBipolarCV(getState<BrightnessKnob>(), getInput<BrightnessCvIn>());

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

		for (int i = 0; i < 6; ++i) {
			phases[i] += (frequency + offsets[i]) * (1 / sampleRate);
			if (phases[i] >= 1.0f) {
				phases[i] -= 1.0f;
			}
			squareWaves[i] = (phases[i] < 0.5f) ? 1.0f : -1.0f;
		}

		float oscSum = 0.f;

		for (int i = 0; i < 6; ++i) {
			oscSum += squareWaves[i];
		}

		oscSum = std::clamp(oscSum, -5.f, 5.f);

		// Bandpass
		float bandpassCutoffFrequency = MathTools::map_value(brightnessControl, 0.f, 1.f, 1000.f, 5000.f);
		float bandpassOut = biquadBandpassFilter(oscSum, bandpassCutoffFrequency);

		// Envelopes
		float decayTimeClosed = 10.f;
		float decayTimeOpen = MathTools::map_value(decayControl, 0.0f, 1.0f, 50.0f, 250.f);
		float decayAlpha1 = std::exp(-1.0f / (sampleRate * (decayTimeClosed / 1000.0f)));
		float decayAlpha2 = std::exp(-1.0f / (sampleRate * (decayTimeOpen / 1000.0f)));

		//Choke <
		if (getState<ChokeSwitch>() == Toggle2posHoriz::State_t::RIGHT) {
			choke = 0;
		} else {
			choke = decayAlpha2;
		}

		// Closed
		if (bangRisingEdge1) {
			decayAlpha2 = choke;
			envelopeValue1 = 1.0f;
		}

		envelopeValue1 *= decayAlpha1;

		// Open
		if (bangRisingEdge2) {
			envelopeValue2 = 1.0f;
		}

		envelopeValue2 *= decayAlpha2;

		// Apply envelope to bandpass output
		float closedVCAOut = (bandpassOut * envelopeValue1);
		float openVCAOut = (bandpassOut * envelopeValue2);

		// Highpass Resonant Filter (12dB/octave)
		float hpCutoffFreq =
			MathTools::map_value(thicknessControl, 1.f, 0.f, 1000.f, 10000.f); // Base frequency for high-pass filter

		float closedHighpassOut = closedHighpass(closedVCAOut, hpCutoffFreq);
		float openHighpassOut = openHighpass(openVCAOut, hpCutoffFreq);

		// Post highpass makeup gain automatic compensation as cutoff decreases
		float finalMakeup = MathTools::map_value(thicknessControl, 0.f, 1.f, 5.0f, 1.0f);

		float finalOutputClosed = (closedHighpassOut * finalMakeup); // Makeup gain
		float finalOutputOpen = (openHighpassOut * finalMakeup);	 // Makeup gain
		finalOutputClosed = std::clamp(finalOutputClosed, -5.f, 5.f);
		finalOutputOpen = std::clamp(finalOutputOpen, -5.f, 5.f);

		setOutput<ClosedOut>(finalOutputClosed);
		setOutput<OpenOut>(finalOutputOpen);
	}

private:
	// Oscillator
	float offsets[6] = {100.f, 250.f, 400.f, 550.f, 600.f, 1000.f}; // Offsets for each oscillator
	float phases[6] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f};				// Phases for each oscillator
	float squareWaves[6] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f};			// Square wave outputs

	// Bandpass Filter
	BiquadBPF bpf{};
	BiquadHPF chh_hpf{};
	BiquadHPF ohh_hpf{};

	// Decay envelopes
	float envelopeValue1 = 0.0f;

	float envelopeValue2 = 0.0f;

	float choke = 0.f;

	bool triggerStates1[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state
	bool triggerStates2[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state
};

} // namespace MetaModule
