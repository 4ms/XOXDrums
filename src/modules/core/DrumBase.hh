#pragma once

#include "CoreModules/SmartCoreProcessor.hh"

namespace MetaModule
{

template<typename INFO>
class DrumBase : public SmartCoreProcessor<INFO> {
protected:
	float sampleRate{48000};

	void set_samplerate(float sr) override {
		sampleRate = sr;
	}
};

} // namespace MetaModule
