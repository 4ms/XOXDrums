#pragma once

#include "CoreModules/SmartCoreProcessor.hh"

namespace MetaModule
{

template<typename INFO>
class DrumBase : public SmartCoreProcessor<INFO> {
protected:
	float sampleRate{48000};
	float rSampleRate{1.f / 48000};

	void set_samplerate(float sr) override {
		sampleRate = sr;
		rSampleRate = 1.f / sampleRate;
	}

	template<INFO::Elem EL>
	bool getInputAsGate() {
		return getInput<EL>().value_or(0.f) > 0.5f;
	}
};

} // namespace MetaModule
