#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "helpers/param_cv.hh"
#include "info/Congabongo_info.hh"
#include "util/edge_detector.hh"
#include "util/math.hh"
#include <cmath>

namespace MetaModule
{

class Congabongo : public SmartCoreProcessor<CongabongoInfo> {
	using Info = CongabongoInfo;
	using enum Info::Elem;

public:
	Congabongo() = default;

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);
		if (param_id == static_cast<int>(DecayKnob) || param_id == static_cast<int>(PitchKnob) ||
			param_id == static_cast<int>(ModeSwitch))
		{
			recalc();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);
		if (input_id == static_cast<int>(DecayCvIn) || input_id == static_cast<int>(PitchCvIn)) {
			recalc();
		}
	}

	void update(void) override {
		if (trigToneLo.update(getInputAsGate<ToneLoIn>())) {
			phase1 = 0.0f;
			amplitudeEnvelopeToneLo = 1.0f;
		}

		if (trigSlapLo.update(getInputAsGate<SlapLoIn>()) && amplitudeEnvelopeToneLo > 0.f) {
			amplitudeEnvelopeToneLo = 0.f;
			phase1 = 0.0f;
			amplitudeEnvelopeSlapLo = 1.0f;
		}

		if (trigToneHi.update(getInputAsGate<ToneHiIn>())) {
			phase2 = 0.0f;
			amplitudeEnvelopeToneHigh = 1.0f;
		}

		if (trigSlapHi.update(getInputAsGate<SlapHiIn>()) && amplitudeEnvelopeToneHigh > 0.f) {
			amplitudeEnvelopeToneHigh = 0.f;
			phase2 = 0.0f;
			amplitudeEnvelopeSlapHigh = 1.0f;
		}

		// Osc 1
		using MathTools::M_PIF;
		phase1 += phaseInc1;
		phase1 -= static_cast<int>(phase1);
		float sineWave1 = 5.0f * std::sin(2 * M_PIF * phase1);

		// Osc 2
		phase2 += phaseInc2;
		phase2 -= static_cast<int>(phase2);
		float sineWave2 = 5.0f * std::sin(2 * M_PIF * phase2);

		amplitudeEnvelopeToneLo *= ampDecayToneAlpha;
		amplitudeEnvelopeToneHigh *= ampDecayToneAlpha;
		amplitudeEnvelopeSlapLo *= ampDecaySlapAlpha;
		amplitudeEnvelopeSlapHigh *= ampDecaySlapAlpha;

		auto finalOutput1 = (sineWave1 * amplitudeEnvelopeToneLo) + ((sineWave1 * amplitudeEnvelopeSlapLo) * 2);
		finalOutput1 = std::clamp(finalOutput1, -5.0f, 5.0f);

		auto finalOutput2 = (sineWave2 * amplitudeEnvelopeToneHigh) + ((sineWave2 * amplitudeEnvelopeSlapHigh) * 2);
		finalOutput2 = std::clamp(finalOutput2, -5.0f, 5.0f);

		setOutput<OutLoOut>(finalOutput1);
		setOutput<OutHiOut>(finalOutput2);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
		ampDecaySlapAlpha = std::exp(-1.0f / (sampleRate * 0.01f)); // Slap time
		recalc();
	}

private:
	template<Info::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}

	void recalc() {
		float ampDecayControl = combineKnobBipolarCV(getState<DecayKnob>(), getInput<DecayCvIn>());
		float pitchControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());

		float ampDecayTime{};
		float freq_hz{};

		//Congabongo switch
		if (getState<ModeSwitch>() == Toggle2posHoriz::State_t::RIGHT) {
			freq_hz = 200 + (pitchControl * 250.0f);
			ampDecayTime = 15.0f + (ampDecayControl * 25.0f);
		} else {
			freq_hz = 98 + (pitchControl * 100.0f);
			ampDecayTime = 40.0f + (ampDecayControl * 80.0f);
		}

		const auto rSampleRate = 1.f / sampleRate;
		phaseInc1 = freq_hz * rSampleRate * 2;
		phaseInc2 = freq_hz * (3.f / 4.f) * rSampleRate * 2;
		ampDecayToneAlpha = std::exp(-1.0f / (sampleRate * (ampDecayTime / 1000.0f)));
	}

	// Sine oscillator
	float phaseInc1 = 0.f;
	float phaseInc2 = 0.f;
	float phase1 = 0.0f;
	float phase2 = 0.0f;

	// Amp decay envelope
	float amplitudeEnvelopeToneLo = 0.f;
	float amplitudeEnvelopeToneHigh = 0.f;
	float amplitudeEnvelopeSlapLo = 0.f;
	float amplitudeEnvelopeSlapHigh = 0.f;

	float ampDecayToneAlpha = 0.0f; // Exponential decay coefficient
	float ampDecaySlapAlpha = 0.0f; // Exponential decay coefficient

	float sampleRate{48000};

	RisingEdgeDetector trigToneLo{};
	RisingEdgeDetector trigSlapLo{};
	RisingEdgeDetector trigToneHi{};
	RisingEdgeDetector trigSlapHi{};
};

} // namespace MetaModule
