#pragma once
#include "CoreModules/SmartCoreProcessor.hh"
#include "info/Congabongo_info.hh"

#include <cmath> // for sine wave
#define TWO_PI (2.0 * M_PI)

namespace MetaModule
{

class Congabongo : public SmartCoreProcessor<CongabongoInfo> {
	using Info = CongabongoInfo;
	using enum Info::Elem;

public:
	Congabongo() = default;

	template<Info::Elem Knob, Info::Elem CV>
	float offset10vppSum() {
		float cvScale = (getInput<CV>().value_or(0.f) + 5.0f) / 10.0f;
		float cvSum = (getState<Knob>() + (cvScale - 0.5f));
		return std::clamp(cvSum, 0.0f, 1.0f);
	}

	void update(void) override {

		float pitchControl = offset10vppSum<PitchKnob, PitchCvIn>();
		float ampDecayControl = offset10vppSum<DecayKnob, DecayCvIn>();

		// Tone Hi input
		bool bangState1 = getInput<ToneHiGateIn>().value_or(0.f) > 0.5f;
		if (bangState1 && !lastbangState1) {
			phase1 = 0.0f; // reset sine phase1 for 0 crossing
			pulseTriggered1 = true;
			amplitudeEnvelope1 = 1.0f;
			pulseTime1 = ampDecayTime * (sampleRate / 1000.0f);
		}
		lastbangState1 = bangState1;

		// Slap Hi input
		bool bangState3 = getInput<SlapHiGateIn>().value_or(0.f) > 0.5f;
		if (bangState3 && !lastbangState3 && pulseTriggered1) {
			amplitudeEnvelope1 *= 0;
			phase1 = 0.0f; // reset sine phase1 for 0 crossing
			pulseTime1 = 0;

			pulseTriggered3 = true;
			amplitudeEnvelope3 = 1.0f;
			pulseTime3 = 5.0f * (sampleRate / 1000.0f);
		} else {
			ampDecayAlpha1 = exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
			amplitudeEnvelope1 *= ampDecayAlpha1;
		}
		lastbangState3 = bangState3;

		// Lo trig input
		bool bangState2 = getInput<ToneLoGateIn>().value_or(0.f) > 0.5f;
		if (bangState2 && !lastbangState2) {
			phase2 = 0.0f; // reset sine phase1 for 0 crossing
			pulseTriggered2 = true;
			amplitudeEnvelope2 = 1.0f;
			pulseTime2 = ampDecayTime * (sampleRate / 1000.0f);
		}
		lastbangState2 = bangState2;

		// Slap LO input
		bool bangState4 = getInput<SlapLoGateIn>().value_or(0.f) > 0.5f;
		if (bangState4 && !lastbangState4 && pulseTriggered2) {
			amplitudeEnvelope2 *= 0;
			phase2 = 0.0f; // reset sine phase1 for 0 crossing
			pulseTime2 = 0;

			pulseTriggered4 = true;
			amplitudeEnvelope4 = 1.0f;
			pulseTime4 = 5.0f * (sampleRate / 1000.0f);
		} else {
			ampDecayAlpha2 = exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
			amplitudeEnvelope2 *= ampDecayAlpha2;
		}
		lastbangState4 = bangState4;

		//Congabongo switch
		if (getState<RangeSwitch>() == Toggle2posHoriz::State_t::RIGHT) {
			frequency1 = 200 + (pitchControl * 250.0f);		  //
			ampDecayTime = 15.0f + (ampDecayControl * 25.0f); // amp decay range (5ms - 25ms)
		} else {
			frequency1 = 98 + (pitchControl * 100.0f);		  //
			ampDecayTime = 40.0f + (ampDecayControl * 80.0f); // amp decay range (5ms - 25ms)
		}

		// Osc 1
		float dt = 1.0f / sampleRate;
		phase1 += frequency1 * TWO_PI * dt;
		phase1 += frequency1 * TWO_PI * dt;
		if (phase1 >= TWO_PI) {
			phase1 -= TWO_PI;
		}
		float sineWave1 = 5.0f * sinf(phase1);

		// Osc 2
		frequency2 = frequency1 * (3.0f / 4.0f); // Low
		phase2 += frequency2 * TWO_PI * dt;
		phase2 += frequency2 * TWO_PI * dt;
		if (phase2 >= TWO_PI) {
			phase2 -= TWO_PI;
		}
		float sineWave2 = 5.0f * sinf(phase2);

		// Slap 1
		ampDecayAlpha3 = exp(-1.0f / (sampleRate * 0.01f)); // Slap time
		amplitudeEnvelope3 *= ampDecayAlpha3;
		// Slap 2
		ampDecayAlpha4 = exp(-1.0f / (sampleRate * 0.01f)); // Slap time
		amplitudeEnvelope4 *= ampDecayAlpha4;

		if (pulseTriggered1) {
			if (amplitudeEnvelope1 < 0.0f) {
				amplitudeEnvelope1 = 0.0f;
				pulseTriggered1 = false;
			}
		} else {
			amplitudeEnvelope1 = 0.0f;
		}

		if (pulseTriggered2) {
			if (amplitudeEnvelope2 < 0.0f) {
				amplitudeEnvelope2 = 0.0f;
				pulseTriggered2 = false;
			}
		} else {
			amplitudeEnvelope2 = 0.0f;
		}

		if (pulseTriggered3) {
			if (amplitudeEnvelope3 < 0.0f) {
				amplitudeEnvelope3 = 0.0f;
				pulseTriggered3 = false;
			}
		} else {
			amplitudeEnvelope3 = 0.0f;
		}

		if (pulseTriggered4) {
			if (amplitudeEnvelope4 < 0.0f) {
				amplitudeEnvelope4 = 0.0f;
				pulseTriggered4 = false;
			}
		} else {
			amplitudeEnvelope4 = 0.0f;
		}

		finalOutput1 = (sineWave1 * amplitudeEnvelope1) + ((sineWave1 * amplitudeEnvelope3) * 2);
		finalOutput1 = std::clamp(finalOutput1, -5.0f, 5.0f);

		finalOutput2 = (sineWave2 * amplitudeEnvelope2) + ((sineWave2 * amplitudeEnvelope4) * 2);
		finalOutput2 = std::clamp(finalOutput2, -5.0f, 5.0f);

		setOutput<OutHiOut>(finalOutput1);
		setOutput<OutLoOut>(finalOutput2);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}

private:
	int select = 0;
	// Sine oscillator
	float phase1 = 0.0f;
	float frequency1 = 100.0f;
	float phase2 = 0.0f;
	float frequency2 = 100.0f;

	// Amp decay envelope
	float amplitudeEnvelope1 = 1.0f; // Envelope output value (for volume control)
	float amplitudeEnvelope2 = 1.0f; // Envelope output value (for volume control)
	float amplitudeEnvelope3 = 1.0f; // Envelope output value (for volume control)
	float amplitudeEnvelope4 = 1.0f; // Envelope output value (for volume control)

	float ampDecayAlpha1 = 0.0f; // Exponential decay coefficient
	float ampDecayAlpha2 = 0.0f; // Exponential decay coefficient
	float ampDecayAlpha3 = 0.0f; // Exponential decay coefficient
	float ampDecayAlpha4 = 0.0f; // Exponential decay coefficient

	float ampDecayTime = 5.0f; // Decay time in ms (5ms as requested)

	// Trig
	bool pulseTriggered1 = false; // Flag to check if pulse was triggered
	bool pulseTriggered2 = false; // Flag to check if pulse was triggered
	bool pulseTriggered3 = false; // Flag to check if pulse was triggered
	bool pulseTriggered4 = false; // Flag to check if pulse was triggered

	bool lastbangState1 = false; // Previous state of the Bang input
	bool lastbangState2 = false; // Previous state of the Bang input
	bool lastbangState3 = false; // Previous state of the Bang input
	bool lastbangState4 = false; // Previous state of the Bang input

	float pulseTime1 = 0.0f; // Time tracking for pulse duration
	float pulseTime2 = 0.0f; // Time tracking for pulse duration
	float pulseTime3 = 0.0f; // Time tracking for pulse duration
	float pulseTime4 = 0.0f; // Time tracking for pulse duration

	// Output
	float finalOutput1;
	float finalOutput2;

	// INTERFACE
	float sampleRate = 44100.0f;
};

} // namespace MetaModule
