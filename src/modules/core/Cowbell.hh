#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "info/Cowbell_info.hh"
#include <cmath> // for sine wave
#define TWO_PI (2.0 * M_PI)

namespace MetaModule
{

class Cowbell : public SmartCoreProcessor<CowbellInfo> {
	using Info = CowbellInfo;
	using enum Info::Elem;

public:
	Cowbell() = default;

	template<Info::Elem Knob, Info::Elem CV>
	float offset10vppSum() {
		float cvScale = (getInput<CV>().value_or(0.f) + 5.0f) / 10.0f;
		float cvSum = (getState<Knob>() + (cvScale - 0.5f));
		return std::clamp(cvSum, 0.0f, 1.0f);
	}

	void update(void) override {

float pitchControl = offset10vppSum<PitchKnob, PitchCvIn>();
float ampDecayControl = offset10vppSum<DecayKnob, DecayCvIn>();


		// Check if the trigger input is high
		bool currentTriggerState = getInput<TrigIn>().value_or(0.f) > 0.5f;
		bool bangRisingEdge = !triggerStates[0] && currentTriggerState;
		triggerStates[0] = triggerStates[1];
		triggerStates[1] = currentTriggerState;

		// Envelopes 
float ampDecayTime = 20.0f + (ampDecayControl * 100.0f); 
float ampDecayAlpha = exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
amplitudeEnvelope *= ampDecayAlpha;

if (bangRisingEdge) {
	phase = 0.0f; // reset sine phase for 0 crossing
	pulseTriggered = true;
	amplitudeEnvelope = 1.0f;
}

// Osc 1
float dt = 1.0f / sampleRate; 
float frequency = 500.0f + (pitchControl * 300.0f); 
phase += frequency * TWO_PI * dt;
phase += frequency * TWO_PI * dt;
if (phase >= TWO_PI) {
	phase -= TWO_PI;
}
float squareWave = (phase < M_PI) ? 5.0f : -5.0f; 

// Osc 2
float frequency2 = (frequency - 260.0f); 
phase2 += frequency2 * TWO_PI * dt;
phase2 += frequency2 * TWO_PI * dt;
if (phase2 >= TWO_PI) {
	phase2 -= TWO_PI;
}
float squareWave2 = (phase2 < M_PI) ? 5.0f : -5.0f; 

if (pulseTriggered) {
	if (amplitudeEnvelope < 0.0f) {
		amplitudeEnvelope = 0.0f;
		pulseTriggered = false;
	}
} else {
	amplitudeEnvelope = 0.0f;
}

// Combine Oscillators
float sumOutput = ((squareWave + squareWave2) * 0.5) * amplitudeEnvelope; 
sumOutput = std::clamp(sumOutput, -5.0f, 5.0f);

// Low-pass filter
lowpassOutput = (lowpassAlpha * sumOutput) + ((1.0f - lowpassAlpha) * lowpassOutput);

// High-pass filter (after low-pass)
highpassOutput = highpassAlpha * (highpassOutput + lowpassOutput - prevHighpassOutput);
prevHighpassOutput = lowpassOutput;

float finalOutput = (highpassOutput * 5.0f);
finalOutput = std::clamp(finalOutput, -5.0f, 5.0f); 

// Output the final filtered signal
setOutput<CowbellOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	// Amp decay envelope 
	float amplitudeEnvelope = 1.0f;  

	bool triggerStates[2] = {false, false}; // triggerStates[0] = last state, triggerStates[1] = current state

	// Trig
	bool pulseTriggered = false; 

	// Sine oscillator 
	float phase = 0.0f;
	float phase2 = 0.0f;

	// Low-pass filter variables
	float lowpassAlpha = 0.1f;    

	// High-pass filter variables
	float lowpassOutput = 0.f; 
	float highpassOutput = 0.f; 
	float highpassAlpha = 0.6f; // Filter cutoff highpass, lower number = more filtering
	float prevHighpassOutput = 0.0f;

	float sampleRate = 44100.0f;
};

} // namespace MetaModule
