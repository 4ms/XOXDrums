#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct RimshotInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Rimshot"};
    static constexpr std::string_view description{"Rimshot"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Rimshot.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Rimshot.png"};

    using enum Coords;

    static constexpr std::array<Element, 6> Elements{{
		Knob9mm{{to_mm<25>(9.912), to_mm<25>(45.41), Center, "Ring", ""}, 0.0f},
		Knob9mm{{to_mm<25>(9.912), to_mm<25>(19.535), Center, "Pitch", ""}, 0.0f},
		AnalogJackOutput4ms{{to_mm<25>(9.912), to_mm<25>(110.168), Center, "Rimshot", ""}},
		AnalogJackInput4ms{{to_mm<25>(9.912), to_mm<25>(93.021), Center, "Trig", ""}},
		AnalogJackInput4ms{{to_mm<25>(9.912), to_mm<25>(60.097), Center, "Ring CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(9.912), to_mm<25>(33.947), Center, "Pitch CV", ""}},
}};

    enum class Elem {
        RingKnob,
        PitchKnob,
        RimshotOut,
        TrigIn,
        RingCvIn,
        PitchCvIn,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobRing, 
        KnobPitch, 
        NumKnobs,
    };
    
    
    enum {
        InputTrig, 
        InputRing_Cv, 
        InputPitch_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputRimshot, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
