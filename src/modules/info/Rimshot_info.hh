#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct RimshotInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Rimshot"};
    static constexpr std::string_view description{"Rimshot DrumModule"};
    static constexpr uint32_t width_hp = 302;
    static constexpr std::string_view svg_filename{"res/Rimshot.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Rimshot.png"};

    using enum Coords;

    static constexpr std::array<Element, 7> Elements{{
		Knob9mm{{to_mm<72>(28.8), to_mm<72>(54.535), Center, "Pitch", ""}, 0.375f, 0.2, 2.0, "khz"},
		Knob9mm{{to_mm<72>(28.8), to_mm<72>(136.735), Center, "Ring", ""}, 0.5f, 0.0, 100.0, "%"},
		WhiteMomentary7mm{{to_mm<72>(28.921), to_mm<72>(216.341), Center, "Trigger", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.8), to_mm<72>(102.725), Center, "Pitch CV In", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.8), to_mm<72>(184.925), Center, "Ring CV In", ""}},
		GateJackInput4ms{{to_mm<72>(28.921), to_mm<72>(253.827), Center, "Trigger In", ""}},
		AnalogJackOutput4ms{{to_mm<72>(28.8), to_mm<72>(302.637), Center, "Audio Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        RingKnob,
        TriggerButton,
        PitchCvIn,
        RingCvIn,
        TriggerIn,
        AudioOut,
    };
};
} // namespace MetaModule
