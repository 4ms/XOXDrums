#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "info/Kick_info.hh"

#include <cmath> // for sine wave
#define TWO_PI (2.0 * M_PI)

namespace MetaModule
{

class Kick : public SmartCoreProcessor<KickInfo> {
	using Info = KickInfo;
	using enum Info::Elem;

public:
	Kick() = default;

	template<Info::Elem Knob, Info::Elem CV>
	float offset10vppSum() {
		float cvScale = (getInput<CV>().value_or(0.f) + 5.0f) / 10.0f;
		float cvSum = (getState<Knob>() + (cvScale - 0.5f));
		return std::clamp(cvSum, 0.0f, 1.0f);
	}

	void update(void) override {

		float pitchControl = offset10vppSum<PitchKnob, PitchCvIn>();
		float pitchDecayControl = offset10vppSum<PitchDecayKnob, PitchDecayCvIn>();
		float ampDecayControl = offset10vppSum<AmpDecayKnob, AmpDecayCvIn>();
		float envDepthControl = offset10vppSum<PitchDepthKnob, DepthCvIn>();
		float saturationControl = offset10vppSum<SaturationKnob, SaturationCvIn>();

		// Trig input 
		bool bangState =  getInput<TrigIn>().value_or(0.f) > 0.5f;
		if (bangState && !lastBangState) {
			phase = 0.0f; // reset sine phase for 0 crossing
			pulseTriggered = true;
			amplitudeEnvelope = 1.0f;
			pitchEnvelope = 1.0f;
			pulseTime = ampDecayTime * (sampleRate / 1000.0f);
		}
		lastBangState = bangState;

		// Osc
		float dt = 1.0f / sampleRate;
		frequency = 10 + (pitchControl * 40.0f); // 10hz - 40hz range 
		modulatedFrequency = frequency + (pitchEnvelope * (envDepthControl * 500.0f)); // Envelope depth range
		phase += modulatedFrequency * TWO_PI * dt;
		phase += frequency * TWO_PI * dt;
		if (phase >= TWO_PI) {
			phase -= TWO_PI;
		}
		float sineWave = 5.0f * sinf(phase);
		
		// Envelopes 
		ampDecayTime = 5.0f + (ampDecayControl * 300.0f); // amp decay range (5ms - 300ms)
		ampDecayAlpha = exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
		amplitudeEnvelope *= ampDecayAlpha;

		pitchDecayTime = 5.0f + (pitchDecayControl* 30.0f); // pitch decay range (5ms - 30ms)
		pitchDecayAlpha = exp(-1.0f / (sampleRate * (pitchDecayTime / 1000.0f)));
		pitchEnvelope *= pitchDecayAlpha;

		if (pulseTriggered) {
			if (amplitudeEnvelope < 0.0f) {
				pitchEnvelope = 0.0f;			
				amplitudeEnvelope = 0.0f;
				pulseTriggered = false;
			}
		} else {
			pitchEnvelope = 0.0f; 
			amplitudeEnvelope = 0.0f;
		}
       
       // Final output 
	   //int range = (int)params[RANGE_PARAM].getValue(); // Read the RANGE_PARAM value

	   int range = 0; 

		switch (range) {
		case 0:
		saturation = 1 + (saturationControl * 2);
		break;
		case 1:
		saturation = 1 + (saturationControl * 10);
		break;
		case 2:
		saturation = 1 + (saturationControl * 100);
		break;
		}

		finalOutput = (sineWave * amplitudeEnvelope) * saturation; 
		finalOutput = std::clamp(finalOutput, -5.0f, 5.0f);
		setOutput<KickOut>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	// Sine oscillator 
	float phase = 0.0f;
	float frequency = 100.0f; 
	float modulatedFrequency;

    // Amp decay envelope 
    float amplitudeEnvelope = 1.0f;  // Envelope output value (for volume control)
    float ampDecayTime = 5.0f;      // Decay time in ms (5ms as requested)
    float ampDecayAlpha = 0.0f;     // Exponential decay coefficient

    // Pitch decay envelope 
	float pitchEnvelope = 1.0f;  // Envelope output value (for volume control)
    float pitchDecayTime = 5.0f;      // Decay time in ms (5ms as requested)
    float pitchDecayAlpha = 0.0f;     // Exponential decay coefficient

	// Trig
    bool pulseTriggered = false; // Flag to check if pulse was triggered
    bool lastBangState = false;  // Previous state of the Bang input
	float pulseTime = 0.0f;      // Time tracking for pulse duration

	// Output 
	float saturation;
	float finalOutput; 
	float ledBrightness; 

	// INTERFACE 
	float pitchControl = 0.0f;
	float envDepthControl = 0.0f;
	float pitchDecayControl = 0.0f;
	float ampDecayControl = 0.0f;
	float saturationControl = 0.0f;

	float sampleRate = 44100.0f;
};

} // namespace MetaModule
