#pragma once

#include "CoreModules/SmartCoreProcessor.hh"
#include "core/Biquad.hh"
#include "helpers/param_cv.hh"
#include "info/Rimshot_info.hh"
#include "util/edge_detector.hh"
#include "util/math.hh"

namespace MetaModule
{

class Rimshot : public SmartCoreProcessor<RimshotInfo> {
	using Info = RimshotInfo;
	using enum Info::Elem;

	static constexpr auto impulse_num_samples{2};

public:
	Rimshot() = default;

	void set_param(int param_id, float val) override {
		SmartCoreProcessor::set_param(param_id, val);
		if (param_id == static_cast<int>(PitchKnob)) {
			recalc_hpf();
		} else if (param_id == static_cast<int>(RingKnob)) {
			recalc_hpf();
		}
	}

	void set_input(int input_id, float val) override {
		SmartCoreProcessor::set_input(input_id, val);
		if (input_id == static_cast<int>(PitchCvIn)) {
			recalc_hpf();
		} else if (input_id == static_cast<int>(RingCvIn)) {
			recalc_hpf();
		}
	}

	void recalc_hpf() {
		const auto cutoffControl = combineKnobBipolarCV(getState<PitchKnob>(), getInput<PitchCvIn>());
		const auto resonanceControl = combineKnobBipolarCV(getState<RingKnob>(), getInput<RingCvIn>());
		const auto highpassResonance = MathTools::map_value(resonanceControl, 0.f, 1.f, 1.f, 10.f);
		const auto hpCutoffFreq = MathTools::map_value(cutoffControl, 0.f, 1.f, 200.f, 2000.f);
		hpf.setBiquad(hpCutoffFreq, sampleRate, highpassResonance);
	}

	void update(void) override {
		const auto level = getInput<TriggerIn>().value_or(0.f);

		if (trig.update(level > .5f)) {
			count = 0;
		}

		const auto impulse = count >= impulse_num_samples ? 0.f : level;
		count++;
		const auto finalOutput = std::clamp(hpf.process(impulse) * 3, -5.0f, 5.0f);
		setOutput<Out>(finalOutput);
	}

	void set_samplerate(float sr) override {
		sampleRate = sr;
		recalc_hpf();
	}

private:
	BiquadHPF hpf{};

	unsigned count{};

	float sampleRate{48000};

	RisingEdgeDetector trig{};
};

} // namespace MetaModule
