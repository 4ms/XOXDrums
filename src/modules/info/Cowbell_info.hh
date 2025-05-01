#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct CowbellInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Cowbell"};
    static constexpr std::string_view description{"Cowbell Drum Module"};
    static constexpr uint32_t width_hp = 288;
    static constexpr std::string_view svg_filename{"res/Cowbell.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Cowbell.png"};

    using enum Coords;

    static constexpr std::array<Element, 6> Elements{{
		Knob9mm{{to_mm<72>(28.92), to_mm<72>(46.77), Center, "Pitch", ""}, 0.5f},
		Knob9mm{{to_mm<72>(28.92), to_mm<72>(143.15), Center, "Decay", ""}, 0.5f},
		AnalogJackInput4ms{{to_mm<72>(28.92), to_mm<72>(94.96), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.93), to_mm<72>(191.34), Center, "Decay CV", ""}},
		GateJackInput4ms{{to_mm<72>(28.8), to_mm<72>(265.04), Center, "Trigger", ""}},
		AnalogJackOutput4ms{{to_mm<72>(28.8), to_mm<72>(313.84), Center, "Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        DecayKnob,
        PitchCvIn,
        DecayCvIn,
        TriggerIn,
        Out,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobPitch, 
        KnobDecay, 
        NumKnobs,
    };
    
    
    enum {
        InputPitch_Cv, 
        InputDecay_Cv, 
        InputTrigger, 
        NumInJacks,
    };
    
    enum {
        OutputOut, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
