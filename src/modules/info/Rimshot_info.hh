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
    static constexpr std::string_view png_filename{"XOXDrums/Rimshot.png"};

    using enum Coords;

    static constexpr std::array<Element, 6> Elements{{
		Knob9mm{{to_mm<72>(28.8), to_mm<72>(54.535), Center, "Pitch", ""}, 0.375f},
		Knob9mm{{to_mm<72>(28.8), to_mm<72>(136.735), Center, "Ring", ""}, 0.5f},
		AnalogJackInput4ms{{to_mm<72>(28.8), to_mm<72>(102.725), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.8), to_mm<72>(184.925), Center, "Ring CV", ""}},
		GateJackInput4ms{{to_mm<72>(28.8), to_mm<72>(253.827), Center, "Trigger", ""}},
		AnalogJackOutput4ms{{to_mm<72>(28.8), to_mm<72>(302.637), Center, "Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        RingKnob,
        PitchCvIn,
        RingCvIn,
        TriggerIn,
        Out,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobPitch, 
        KnobRing, 
        NumKnobs,
    };
    
    
    enum {
        InputPitch_Cv, 
        InputRing_Cv, 
        InputTrigger, 
        NumInJacks,
    };
    
    enum {
        OutputOut, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
