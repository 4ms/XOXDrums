#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct CowbellInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Cowbell"};
    static constexpr std::string_view description{"Cowbell"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Cowbell.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Cowbell.png"};

    using enum Coords;

    static constexpr std::array<Element, 6> Elements{{
		Knob9mm{{to_mm<25>(10.212), to_mm<25>(45.61), Center, "Decay", ""}, 1.0f},
		Knob9mm{{to_mm<25>(10.212), to_mm<25>(19.735), Center, "Pitch", ""}, 1.0f},
		AnalogJackOutput4ms{{to_mm<25>(10.212), to_mm<25>(110), Center, "Cowbell", ""}},
		GateJackInput4ms{{to_mm<25>(10.212), to_mm<25>(92.5), Center, "Trig", ""}},
		GateJackInput4ms{{to_mm<25>(10.212), to_mm<25>(60), Center, "Decay CV", ""}},
		GateJackInput4ms{{to_mm<25>(10.212), to_mm<25>(34), Center, "Pitch CV", ""}},
}};

    enum class Elem {
        DecayKnob,
        PitchKnob,
        CowbellOut,
        TrigIn,
        DecayCvIn,
        PitchCvIn,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobDecay, 
        KnobPitch, 
        NumKnobs,
    };
    
    
    enum {
        InputTrig, 
        InputDecay_Cv, 
        InputPitch_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputCowbell, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
