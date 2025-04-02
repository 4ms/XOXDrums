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

};

} // namespace MetaModule
