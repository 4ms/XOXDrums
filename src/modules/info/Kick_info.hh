#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct KickInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Kick"};
    static constexpr std::string_view description{"Kick DrumModule"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/Kick.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Kick.png"};

    using enum Coords;

    static constexpr std::array<Element, 13> Elements{{
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(62.027), Center, "Pitch", ""}, 0.328125f},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(61.894), Center, "P Depth", ""}, 0.5f},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(139.494), Center, "P Decay", ""}, 0.5f},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(139.494), Center, "Amp Decay", ""}, 0.328125f},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(217.095), Center, "Saturation", ""}, 0.328125f},
		Toggle3posHoriz{{to_mm<96>(140.688), to_mm<96>(217.429), Center, "Range Switch", ""}, {"2x", "10x", "100x"}},
		GateJackInput4ms{{to_mm<96>(39.347), to_mm<96>(309.468), Center, "Trig", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.601), to_mm<96>(309.468), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.987), to_mm<96>(309.468), Center, "Depth CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(39.347), to_mm<96>(362.975), Center, "P Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.601), to_mm<96>(362.975), Center, "Amp Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.987), to_mm<96>(362.975), Center, "Saturation CV", ""}},
		AnalogJackOutput4ms{{to_mm<96>(152.987), to_mm<96>(418.456), Center, "Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        PDepthKnob,
        PDecayKnob,
        AmpDecayKnob,
        SaturationKnob,
        RangeSwitch,
        TrigIn,
        PitchCvIn,
        DepthCvIn,
        PDecayCvIn,
        AmpDecayCvIn,
        SaturationCvIn,
        Out,
    };
};
} // namespace MetaModule
