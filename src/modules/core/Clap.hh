#pragma once

#include "core/Biquad.hh"
#include "core/DrumBase.hh"
#include "helpers/param_cv.hh"
#include "info/Clap_info.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Clap : public DrumBase<ClapInfo> {
	using Info = ClapInfo;
	using enum Info::Elem;

	static constexpr auto filter_q = 2.f;

public:
	Clap() {
		bpf.setQ(filter_q);
	}

	// Function to generate white noise
	float whiteNoise() {
		return (rand() / (float)RAND_MAX) * 10.0f - 5.0f; // Random between -5V and +5V
	}

	// Function to apply a biquad bandpass filter
	float biquadBandpassFilter(float input, float cutoff) {
		bpf.setFc(cutoff, sampleRate);
		return bpf.process(input);
	}

	void update(void) override {

		float energyControl = combineKnobBipolarCV(getState<EnergyKnob>(), getInput<EnergyCvIn>());
		float spreadControl = combineKnobBipolarCV(getState<SpreadKnob>(), getInput<SpreadCvIn>());
		float verbDecayControl = combineKnobBipolarCV(getState<VerbDecayKnob>(), getInput<VerbDecayCvIn>());
		float colorControl = combineKnobBipolarCV(getState<ColorKnob>(), getInput<ColorCvIn>());
		float verbVolumeControl = combineKnobBipolarCV(getState<VerbVolumeKnob>(), getInput<VerbVolumeCvIn>());
		float saturationControl = combineKnobBipolarCV(getState<SaturationKnob>(), getInput<SaturationCvIn>());

		// Check if the trigger input is high
		bool currentTriggerState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge = !triggerStates[0] && currentTriggerState;
		triggerStates[0] = triggerStates[1];
		triggerStates[1] = currentTriggerState;

		// Envelope decay times 1-3 (short) 5-15ms
		float decayTimeShort = MathTools::map_value(energyControl, 0.0f, 1.0f, 10.0f, 20.f);
		float decayAlphaShort = std::exp(-1.0f / (sampleRate * (decayTimeShort / 1000.0f)));

		// Last envelope (reverb time)
		float decayTimeLong = MathTools::map_value(verbDecayControl, 0.0f, 1.0f, 20.0f, 100.f);
		float decayAlphaLong = std::exp(-1.0f / (sampleRate * (decayTimeLong / 1000.0f)));

		// Spread knob (delay times between each envelope)
		float delayTime1 = MathTools::map_value(spreadControl, 0.0f, 1.0f, 20.0f, 40.f);
		float delayTime2 = MathTools::map_value(spreadControl, 0.0f, 1.0f, 35.0f, 50.f);
		float delayTime3 = MathTools::map_value(spreadControl, 0.0f, 1.0f, 45.0f, 60.f);

		// Delay counters
		delayCounter1++;
		delayCounter2++;
		delayCounter3++;

		// Samples to ms conversion
		int delayInSamples1 = delayTime1 * (sampleRate / 1000);
		int delayInSamples2 = delayTime2 * (sampleRate / 1000);
		int delayInSamples3 = delayTime3 * (sampleRate / 1000);

		// Envelope logic
		if (bangRisingEdge) {
			envelopeValue1 = 1.0f;
			delayCounter1 = 0;
			delayCounter2 = 0;
			delayCounter3 = 0;
		}

		// Env 2 delay
		if (delayCounter1 == delayInSamples1) {
			envelopeValue2 = 1.0f;
		}

		// Env 3 delay
		if (delayCounter2 == delayInSamples2) {
			envelopeValue3 = 1.0f;
		}

		// Env 4 delay
		if (delayCounter3 == delayInSamples3) {
			envelopeValue4 = 1.0f;
		}

		// Short envs
		envelopeValue1 *= decayAlphaShort;
		envelopeValue2 *= decayAlphaShort;
		envelopeValue3 *= decayAlphaShort;

		// Long env
		envelopeValue4 *= decayAlphaLong;

		// Filtered noise
		float cutoffFrequency = MathTools::map_value(colorControl, 0.f, 1.f, 800.f, 1600.f);
		float filteredNoise = biquadBandpassFilter(whiteNoise(), cutoffFrequency);

		// Apply envelopes
		float outputSignal1 = filteredNoise * envelopeValue1;
		float outputSignal2 = filteredNoise * envelopeValue2;
		float outputSignal3 = filteredNoise * envelopeValue3;
		float outputSignal4 = filteredNoise * envelopeValue4;

		// Mixer
		float saturation = MathTools::map_value(saturationControl, 0.f, 1.f, 1.f, 10.f);
		float finalOutput =
			((outputSignal1 + outputSignal2 + outputSignal3 + ((outputSignal4 * verbVolumeControl)) * 0.75f) * 5.f) *
			saturation;

		finalOutput = std::clamp(finalOutput, -5.f, 5.f);

		setOutput<ClapOut>(finalOutput);
	}

private:
	// Filter
	BiquadBPF bpf{};

	float envelopeValue1 = 0.0f;
	float envelopeValue2 = 0.0f;
	float envelopeValue3 = 0.0f;
	float envelopeValue4 = 0.0f;

	int delayCounter1 = 0;
	int delayCounter2 = 0;
	int delayCounter3 = 0;

	bool triggerStates[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state
};

} // namespace MetaModule
