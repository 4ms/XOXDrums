#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "info/Snare_info.hh"

#include <cmath> // for sine wave
#define TWO_PI (2.0 * M_PI)

namespace MetaModule
{

class Snare : public SmartCoreProcessor<SnareInfo> {
	using Info = SnareInfo;
	using enum Info::Elem;

public:
	Snare() = default;

	template<Info::Elem Knob, Info::Elem CV>
	float offset10vppSum() {
		float cvScale = (getInput<CV>().value_or(0.f) + 5.0f) / 10.0f;
		float cvSum = (getState<Knob>() + (cvScale - 0.5f));
		return std::clamp(cvSum, 0.0f, 1.0f);
	}

	float biquadBandpassFilter1(float input, float cutoff, float sampleRate1) {
		// Calculate the filter coefficients for the bandpass filter (using cutoff and resonance)
		float omega = 2.0f * M_PI * cutoff / sampleRate1;
		float sn = sinf(omega);
		float cs = cosf(omega);
		float alpha = sn / (2.0f); // Resonance is last number 

		// Compute the bandpass filter coefficients (Biquad)
		filterB0 = alpha;
		filterB1 = 0.0f;
		filterB2 = -alpha;
		filterA0 = 1.0f + alpha;
		filterA1 = -2.0f * cs;
		filterA2 = 1.0f - alpha;

		// Normalize coefficients
		filterB0 /= filterA0;
		filterB1 /= filterA0;
		filterB2 /= filterA0;
		filterA1 /= filterA0;
		filterA2 /= filterA0;

		// Apply the filter to the input signal
		float output = filterB0 * input + filterB1 * filterX1 + filterB2 * filterX2
					   - filterA1 * filterY1 - filterA2 * filterY2;

		// Update the filter states
		filterX2 = filterX1;
		filterX1 = input;
		filterY2 = filterY1;
		filterY1 = output;

		return output;
	}

	void update(void) override {

		float pitchControl = offset10vppSum<PitchKnob, PitchIn>();
		float envDepthControl = offset10vppSum<PitchEnvAmountKnob, PitchEnvAmountCvIn>();
		float pitchDecayControl = offset10vppSum<PitchDecayKnob, PitchDecayCvIn>();
		float ampDecayControl = offset10vppSum<BodyDecayKnob, BodyDecayCvIn>();
		float saturationControl = offset10vppSum<SaturationKnob, SaturationCvIn>();
		float noiseVolumeControl = offset10vppSum<Body_NoiseKnob, Body_NoiseCvIn>();
		float noiseColorControl = offset10vppSum<NoiseColorKnob, NoiseColorCvIn>();
		float noiseDecayControl = offset10vppSum<NoiseDecayKnob, NoiseDecayCvIn>();

		// Trig input 
		bool bangState = getInput<TriggerIn>().value_or(0.f)> 0.5f;
		if (bangState && !lastBangState) {
			phase = 0.0f; // reset sine phase for 0 crossing
			pulseTriggered = true;
			amplitudeEnvelope = 1.0f;
			pitchEnvelope = 1.0f;
			noiseEnvelope = 1.0f;			
			pulseTime = ampDecayTime * (sampleRate / 1000.0f);
		}
		lastBangState = bangState;

		// Osc
		float dt = 1.0f / sampleRate;
		frequency = 80 + (pitchControl * 100.0f); // Body pitch range 
		modulatedFrequency = frequency + (pitchEnvelope * (envDepthControl * 500.0f)); // Envelope depth range
		phase += modulatedFrequency * TWO_PI * dt;
		phase += frequency * TWO_PI * dt;
		if (phase >= TWO_PI) {
			phase -= TWO_PI;
		}
		float sineWave = 5.0f * sinf(phase);
		sineWave = (sineWave * ((1.f-noiseVolumeControl)/2.f)); 
		
		// Envelopes 
		ampDecayTime = 5.0f + (ampDecayControl * 50.0f); // amp decay range 
		ampDecayAlpha = exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		pitchDecayTime = 5.0f + (pitchDecayControl* 30.0f); // pitch decay range  
		pitchDecayAlpha = exp(-1.0f / (sampleRate * (pitchDecayTime / 1000.0f)));
		pitchEnvelope *= pitchDecayAlpha;

		noiseDecayTime = 5.0f + (noiseDecayControl * 75.0f); // pitch decay range  
		noiseDecayAlpha = exp(-1.0f / (sampleRate * (noiseDecayTime / 1000.0f)));
		noiseEnvelope *= noiseDecayAlpha;

		if (pulseTriggered) {
			if (amplitudeEnvelope < 0.0f) {
				pitchEnvelope = 0.0f;			
				amplitudeEnvelope = 0.0f;
				noiseEnvelope = 0.0f;			
				pulseTriggered = false;
			}
		} else {
			pitchEnvelope = 0.0f; 
			amplitudeEnvelope = 0.0f;
			noiseEnvelope = 0.0f;			
		}
       
		// Noise + filter 
		float noise = (rand() / (float)RAND_MAX) * 10.0f - 5.0f;
		float cutoffFrequency = 1000.0f + (noiseColorControl * 5000.0f);
		float modulatedCutoffFrequency = cutoffFrequency + (pitchEnvelope * (envDepthControl * 5000.0f)); // Envelope depth range
		float filteredNoise = (biquadBandpassFilter1(noise, modulatedCutoffFrequency, sampleRate)) * 2.0f;
		filteredNoise = ((filteredNoise * noiseEnvelope) * noiseVolumeControl); 
		filteredNoise = std::clamp(filteredNoise, -5.0f, 5.0f); 

		if (getState<RangeSwitch>() == Toggle3pos::State_t::UP) {
			saturation = 1 + (saturationControl * 100);
		}
		if (getState<RangeSwitch>() == Toggle3pos::State_t::CENTER) 
		{
			saturation = 1 + (saturationControl * 10);
		}
		if (getState<RangeSwitch>() == Toggle3pos::State_t::DOWN) {
			saturation = 1 + (saturationControl * 2);
		} 
		
		finalOutput = ((sineWave * amplitudeEnvelope) + filteredNoise) * saturation; 
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);

		setOutput<SnareOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	
	// Sine oscillator 
	float phase = 0.0f;
	float frequency = 100.0f; 
	float modulatedFrequency;

    // Body decay envelope 
    float amplitudeEnvelope = 1.0f;  // Envelope output value (for volume control)
    float ampDecayTime = 5.0f;      // Decay time in ms (5ms as requested)
    float ampDecayAlpha = 0.0f;     // Exponential decay coefficient

    // Pitch decay envelope 
	float pitchEnvelope = 1.0f;  // Envelope output value (for volume control)
    float pitchDecayTime = 5.0f;      // Decay time in ms (5ms as requested)
    float pitchDecayAlpha = 0.0f;     // Exponential decay coefficient

	// Noise decay envelope
	float noiseEnvelope = 1.0f; 
	float noiseDecayTime = 5.0f;      // Decay time in ms (5ms as requested)
    float noiseDecayAlpha = 0.0f;     // Exponential decay coefficient

	// Trig
    bool pulseTriggered = false; // Flag to check if pulse was triggered
    bool lastBangState = false;  // Previous state of the Bang input
	float pulseTime = 0.0f;      // Time tracking for pulse duration

	// Output 
	float saturation;
	float finalOutput; 

	// Bandpass 
	float filterB0 = 0.0f, filterB1 = 0.0f, filterB2 = 0.0f, filterA0 = 0.0f, filterA1 = 0.0f, filterA2 = 0.0f;
    float filterX1 = 0.0f, filterX2 = 0.0f, filterY1 = 0.0f, filterY2 = 0.0f;

	float sampleRate = 44100.0f;
};

} // namespace MetaModule
