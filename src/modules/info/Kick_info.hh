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

    static constexpr std::array<Element, 14> Elements{{
		WhiteMomentary7mm{{to_mm<96>(39.347), to_mm<96>(309.468), Center, "Trigger", ""}},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(62.027), Center, "Pitch", ""}, 0.328125f, 10.0, 40.0, "hz"},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(61.894), Center, "Pitch Depth", ""}, 0.5f, 0.0, 100.0, "%"},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(139.494), Center, "Pitch Decay", ""}, 0.5f, 5.0, 30.0, "ms"},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(139.494), Center, "Amp Decay", ""}, 0.328125f, 5.0, 300.0, "ms"},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(217.095), Center, "Saturation", ""}, 0.328125f, 0.0, 100.0, "%"},
		Toggle3posHoriz{{to_mm<96>(140.688), to_mm<96>(217.429), Center, "Range Switch", ""}, {"2x", "10x", "100x"}},
		GateJackInput4ms{{to_mm<96>(39.347), to_mm<96>(362.975), Center, "Trigger In", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.601), to_mm<96>(309.468), Center, "Pitch CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.987), to_mm<96>(309.468), Center, "Depth CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(39.347), to_mm<96>(418.456), Center, "Pitch Decay CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.601), to_mm<96>(362.975), Center, "Amp Decay CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.987), to_mm<96>(362.975), Center, "Saturation CV In", ""}},
		AnalogJackOutput4ms{{to_mm<96>(152.987), to_mm<96>(418.456), Center, "Audio Out", ""}},
}};

    enum class Elem {
        TriggerButton,
        PitchKnob,
        PitchDepthKnob,
        PitchDecayKnob,
        AmpDecayKnob,
        SaturationKnob,
        RangeSwitch,
        TriggerIn,
        PitchCvIn,
        DepthCvIn,
        PitchDecayCvIn,
        AmpDecayCvIn,
        SaturationCvIn,
        AudioOut,
    };
};
} // namespace MetaModule
