#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/HiHat_info.hh"
#include "util/math.hh"

namespace MetaModule
{

class HiHat : public SmartCoreProcessor<HiHatInfo> {
	using Info = HiHatInfo;
	using enum Info::Elem;

public:
	HiHat() = default;

	// Function to apply a biquad bandpass filter
	float biquadBandpassFilter(float input, float cutoff, float sampleRate1) {
		// Calculate the filter coefficients for the bandpass filter (using cutoff and resonance)
		float omega = 2.0f * M_PI * cutoff / sampleRate1;
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

	// Closed high-pass filter with 'closed' prefix for variables
	float closedHighpass(float input,
						 float &closedPrevInput1,
						 float &closedPrevInput2,
						 float &closedPrevOutput1,
						 float &closedPrevOutput2,
						 float cutoffFreq,
						 float sampleRate2,
						 float resonance) {
		// Calculate the filter coefficients for the 12dB/octave high-pass filter
		float omegaClosed = 2.0f * M_PI * cutoffFreq / sampleRate2; // Angular frequency
		float sinOmegaClosed = sinf(omegaClosed);
		float cosOmegaClosed = cosf(omegaClosed);
		float alphaClosed = sinOmegaClosed / (2.0f * resonance); // Q factor

		// Coefficients for the high-pass filter (resonant, 12dB per octave)
		float b0Closed = (1.0f + cosOmegaClosed) / 2.0f;
		float b1Closed = -(1.0f + cosOmegaClosed);
		float b2Closed = b0Closed;
		float a0Closed = 1.0f + alphaClosed;
		float a1Closed = -2.0f * cosOmegaClosed;
		float a2Closed = 1.0f - alphaClosed;

		// Normalize coefficients
		b0Closed /= a0Closed;
		b1Closed /= a0Closed;
		b2Closed /= a0Closed;
		a1Closed /= a0Closed;
		a2Closed /= a0Closed;

		// Apply the high-pass filter (biquad filter)
		float outputClosed = b0Closed * input + b1Closed * closedPrevInput1 + b2Closed * closedPrevInput2 -
							 a1Closed * closedPrevOutput1 - a2Closed * closedPrevOutput2;

		// Update the filter state variables
		closedPrevInput2 = closedPrevInput1;
		closedPrevInput1 = input;
		closedPrevOutput2 = closedPrevOutput1;
		closedPrevOutput1 = outputClosed;

		return outputClosed;
	}

	// Open high-pass filter with different variable names
	float openHighpass(float input,
					   float &prevIn1,
					   float &prevIn2,
					   float &prevOut1,
					   float &prevOut2,
					   float cutoffFreq,
					   float sampleRate3,
					   float resonance) {
		// Calculate the filter coefficients for the 12dB/octave high-pass filter
		float omegaOpen = 2.0f * M_PI * cutoffFreq / sampleRate3; // Angular frequency
		float sinOmegaOpen = sinf(omegaOpen);
		float cosOmegaOpen = cosf(omegaOpen);
		float alphaOpen = sinOmegaOpen / (2.0f * resonance); // Q factor

		// Coefficients for the high-pass filter (resonant, 12dB per octave)
		float b0Open = (1.0f + cosOmegaOpen) / 2.0f;
		float b1Open = -(1.0f + cosOmegaOpen);
		float b2Open = b0Open;
		float a0Open = 1.0f + alphaOpen;
		float a1Open = -2.0f * cosOmegaOpen;
		float a2Open = 1.0f - alphaOpen;

		// Normalize coefficients
		b0Open /= a0Open;
		b1Open /= a0Open;
		b2Open /= a0Open;
		a1Open /= a0Open;
		a2Open /= a0Open;

		// Apply the high-pass filter (biquad filter)
		float outputOpen = b0Open * input + b1Open * prevIn1 + b2Open * prevIn2 - a1Open * prevOut1 - a2Open * prevOut2;

		// Update the filter state variables
		prevIn2 = prevIn1;
		prevIn1 = input;
		prevOut2 = prevOut1;
		prevOut1 = outputOpen;

		return outputOpen;
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
		float bandpassOut = biquadBandpassFilter(oscSum, bandpassCutoffFrequency, sampleRate);

		// Envelopes
		float decayTimeClosed = 10.f;
		float decayTimeOpen = MathTools::map_value(decayControl, 0.0f, 1.0f, 50.0f, 250.f);
		float decayAlpha1 = exp(-1.0f / (sampleRate * (decayTimeClosed / 1000.0f)));
		float decayAlpha2 = exp(-1.0f / (sampleRate * (decayTimeOpen / 1000.0f)));

		//Choke <
		if (getState<ChokeSwitch>() == Toggle2posHoriz::State_t::RIGHT) {
			choke = 0;
		} else {
			choke = decayAlpha2;
		}

		// Closed
		if (bangRisingEdge1) {
			decayAlpha2 = choke;
			pulseTriggered1 = true;
			envelopeValue1 = 1.0f;
		}
		if (pulseTriggered1) {
			envelopeValue1 *= decayAlpha1;
		} else {
			envelopeValue1 = 0.0f;
		}

		// Open
		if (bangRisingEdge2) {
			pulseTriggered2 = true;
			envelopeValue2 = 1.0f;
		}
		if (pulseTriggered2) {
			envelopeValue2 *= decayAlpha2;
		} else {
			envelopeValue2 = 0.0f;
		}

		// Apply envelope to bandpass output
		float closedVCAOut = (bandpassOut * envelopeValue1);
		float openVCAOut = (bandpassOut * envelopeValue2);

		// Highpass Resonant Filter (12dB/octave)
		float highpassResonance = 1.f;
		float hpCutoffFreq = MathTools::map_value(thicknessControl, 1.f, 0.f, 1000.f, 10000.f); // Base frequency for high-pass filter

		float closedHighpassOut = closedHighpass(closedVCAOut,
										   closedPrevIn1,
										   closedPrevIn2,
										   closedPrevOut1,
										   closedPrevOut2,
										   hpCutoffFreq,
										   sampleRate,
										   highpassResonance);
		float openHighpassOut = openHighpass(openVCAOut,
									   prevIn1Open,
									   prevIn2Open,
									   prevOut1Open,
									   prevOut2Open,
									   hpCutoffFreq,
									   sampleRate,
									   highpassResonance);

		// Post highpass makeup gain automatic compensation as cutoff decreases
		float finalMakeup = MathTools::map_value(thicknessControl, 0.f, 1.f, 5.0f, 1.0f);

		float finalOutputClosed = (closedHighpassOut * finalMakeup); // Makeup gain
		float finalOutputOpen = (openHighpassOut * finalMakeup);	   // Makeup gain
		finalOutputClosed = std::clamp(finalOutputClosed, -5.f, 5.f);
		finalOutputOpen = std::clamp(finalOutputOpen, -5.f, 5.f);

		setOutput<ClosedOut>(finalOutputClosed);
		setOutput<OpenOut>(finalOutputOpen);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	// Oscillator
	float offsets[6] = {100.f, 250.f, 400.f, 550.f, 600.f, 1000.f}; // Offsets for each oscillator
	float phases[6] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f};				// Phases for each oscillator
	float squareWaves[6] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f};			// Square wave outputs

	// Bandpass Filter
	float filterB0 = 0.0f, filterB1 = 0.0f, filterB2 = 0.0f, filterA0 = 0.0f, filterA1 = 0.0f, filterA2 = 0.0f;
	float filterX1 = 0.0f, filterX2 = 0.0f, filterY1 = 0.0f, filterY2 = 0.0f;

	// Decay envelopes
	float envelopeValue1 = 0.0f;
	bool pulseTriggered1 = false;

	float envelopeValue2 = 0.0f;
	bool pulseTriggered2 = false;

	float resonance = 1.f;

	// High-pass filter variables
	float closedPrevIn1 = 0.0f, closedPrevIn2 = 0.0f;
	float closedPrevOut1 = 0.0f, closedPrevOut2 = 0.0f;

	float prevIn1Open = 0.0f, prevIn2Open = 0.0f;
	float prevOut1Open = 0.0f, prevOut2Open = 0.0f;

	float choke = 0.f;

	bool triggerStates1[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state
	bool triggerStates2[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state
	float sampleRate = 48000.0f;

};

} // namespace MetaModule
