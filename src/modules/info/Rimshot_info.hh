#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct RimshotInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Rimshot"};
    static constexpr std::string_view description{"Rimshot Drum Module"};
    static constexpr uint32_t width_hp = 288;
    static constexpr std::string_view svg_filename{"res/Rimshot.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Rimshot.png"};

    using enum Coords;

    static constexpr std::array<Element, 5> Elements{{
		Knob9mm{{to_mm<72>(28.8), to_mm<72>(128.83), Center, "Ring", ""}, 0.5f},
		AnalogJackInput4ms{{to_mm<72>(28.8), to_mm<72>(94.82), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.8), to_mm<72>(177.02), Center, "Ring CV", ""}},
		GateJackInput4ms{{to_mm<72>(28.8), to_mm<72>(264.9), Center, "Trigger", ""}},
		AnalogJackOutput4ms{{to_mm<72>(28.8), to_mm<72>(313.71), Center, "Out", ""}},
}};

    enum class Elem {
        RingKnob,
        PitchCvIn,
        RingCvIn,
        TriggerIn,
        Out,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
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
