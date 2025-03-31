#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Clap_info.hh"
#include <cmath>
#include "util/math.hh"

namespace MetaModule
{

class Clap : public SmartCoreProcessor<ClapInfo> {
	using Info = ClapInfo;
	using enum Info::Elem;

public:
	Clap() = default;

	// Function to generate white noise
	float whiteNoise() {
		return (rand() / (float)RAND_MAX) * 10.0f - 5.0f; // Random between -5V and +5V
	}

	// Function to apply a biquad bandpass filter
	float biquadBandpassFilter(float input, float cutoff, float sampleRate) {
		// Calculate the filter coefficients for the bandpass filter (using cutoff and resonance)
		float omega = 2.0f * M_PI * cutoff / sampleRate;
		float sn = sinf(omega);
		float cs = cosf(omega);
		float alpha1 = sn / (2.0f * resonance);

		// Compute the bandpass filter coefficients (Biquad)
		filterB0 = alpha1;
		filterB1 = 0.0f;
		filterB2 = -alpha1;
		filterA0 = 1.0f + alpha1;
		filterA1 = -2.0f * cs;
		filterA2 = 1.0f - alpha1;

		// Normalize coefficients
		filterB0 /= filterA0;
		filterB1 /= filterA0;
		filterB2 /= filterA0;
		filterA1 /= filterA0;
		filterA2 /= filterA0;

		// Apply the filter to the input signal
		float output1 =
			filterB0 * input + filterB1 * filterX1 + filterB2 * filterX2 - filterA1 * filterY1 - filterA2 * filterY2;

		// Update the filter states
		filterX2 = filterX1;
		filterX1 = input;
		filterY2 = filterY1;
		filterY1 = output1;

		return output1;
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
		float decayTime1 = MathTools::map_value(energyControl, 0.0f, 1.0f, 10.0f, 20.f);
		float decayAlpha1 = exp(-1.0f / (sampleRate * (decayTime1 / 1000.0f)));
		float decayAlpha2 = exp(-1.0f / (sampleRate * (decayTime1 / 1000.0f)));
		float decayAlpha3 = exp(-1.0f / (sampleRate * (decayTime1 / 1000.0f)));

		// Last envelope (reverb time)
		float decayTime2 = MathTools::map_value(verbDecayControl, 0.0f, 1.0f, 20.0f, 100.f);
		float decayAlpha4 = exp(-1.0f / (sampleRate * (decayTime2 / 1000.0f)));

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
			pulseTriggered1 = true;
			envelopeValue1 = 1.0f;
			delayCounter1 = 0;
			delayCounter2 = 0;
			delayCounter3 = 0;
		}

		// Env 2 delay
		if (delayCounter1 == delayInSamples1) {
			pulseTriggered2 = true;
			envelopeValue2 = 1.0f;
		}

		// Env 3 delay
		if (delayCounter2 == delayInSamples2) {
			pulseTriggered3 = true;
			envelopeValue3 = 1.0f;
		}

		// Env 4 delay
		if (delayCounter3 == delayInSamples3) {
			pulseTriggered4 = true;
			envelopeValue4 = 1.0f;
		}

		// Env 1 (short)
		if (pulseTriggered1) {
			envelopeValue1 *= decayAlpha1;
		} else {
			envelopeValue1 = 0.0f;
		}

		// Env 2 (short)
		if (pulseTriggered2) {
			envelopeValue2 *= decayAlpha2;
		} else {
			envelopeValue2 = 0.0f;
		}

		// Env 3 (short)
		if (pulseTriggered3) {
			envelopeValue3 *= decayAlpha3;
		} else {
			envelopeValue3 = 0.0f;
		}

		// Env 4 (long)
		if (pulseTriggered4) {
			envelopeValue4 *= decayAlpha4;
		} else {
			envelopeValue4 = 0.0f;
		}

		// Filtered noise
		float cutoffFrequency = MathTools::map_value(colorControl, 0.f, 1.f, 800.f, 1600.f);
		float filteredNoise = biquadBandpassFilter(whiteNoise(), cutoffFrequency, sampleRate);

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

		setOutput<Out>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	// Filter
	float filterB0 = 0.0f, filterB1 = 0.0f, filterB2 = 0.0f, filterA0 = 0.0f, filterA1 = 0.0f, filterA2 = 0.0f;
	float filterX1 = 0.0f, filterX2 = 0.0f, filterY1 = 0.0f, filterY2 = 0.0f;
	float resonance = 2.f;

	float envelopeValue1 = 0.0f;
	float envelopeValue2 = 0.0f;
	float envelopeValue3 = 0.0f;
	float envelopeValue4 = 0.0f;

	bool pulseTriggered1 = false;
	bool pulseTriggered2 = false;
	bool pulseTriggered3 = false;
	bool pulseTriggered4 = false;

	int delayCounter1 = 0; 
	int delayCounter2 = 0; 
	int delayCounter3 = 0; 
	int delayCounter4 = 0; 

	bool triggerStates[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state

	float sampleRate = 48000.0f;
};

} // namespace MetaModule
