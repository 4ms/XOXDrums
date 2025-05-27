#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct TomInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Tom"};
    static constexpr std::string_view description{"Tom DrumModule"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/Tom.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Tom.png"};

    using enum Coords;

    static constexpr std::array<Element, 11> Elements{{
		Davies1900hBlackKnob{{to_mm<96>(50.267), to_mm<96>(76.934), Center, "Pitch", ""}, 0.390625f, 0.0, 100.0, "%"},
		Davies1900hBlackKnob{{to_mm<96>(141.734), to_mm<96>(76.934), Center, "Amp Decay", ""}, 0.390625f, 25.0, 100.0, "ms"},
		Toggle3posHoriz{{to_mm<96>(95.847), to_mm<96>(148.256), Center, "Pitch Range", ""}, {"Low", "Med", "High"}, Toggle3posHoriz::State_t::CENTER},
		Davies1900hBlackKnob{{to_mm<96>(50.267), to_mm<96>(216.934), Center, "Pitch Decay", ""}, 0.0f, 5.0, 50.0, "ms"},
		Davies1900hBlackKnob{{to_mm<96>(141.734), to_mm<96>(216.934), Center, "Env Depth", ""}, 0.390625f, 0.0, 100.0, "%"},
		GateJackInput4ms{{to_mm<96>(39.2), to_mm<96>(309.335), Center, "Trigger In", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.467), to_mm<96>(309.335), Center, "Pitch CV In", ""}},
		GateJackInput4ms{{to_mm<96>(152.801), to_mm<96>(309.335), Center, "Amp Decay CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(39.2), to_mm<96>(362.802), Center, "Pitch Decay CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.801), to_mm<96>(362.802), Center, "Env Depth CV In", ""}},
		AnalogJackOutput4ms{{to_mm<96>(152.934), to_mm<96>(418.269), Center, "Audio Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        AmpDecayKnob,
        PitchRangeSwitch,
        PitchDecayKnob,
        EnvDepthKnob,
        TriggerIn,
        PitchCvIn,
        AmpDecayCvIn,
        PitchDecayCvIn,
        EnvDepthCvIn,
        AudioOut,
    };
};
} // namespace MetaModule
