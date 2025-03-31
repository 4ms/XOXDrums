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
    static constexpr std::string_view svg_filename{"res/Rimshot_info.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Rimshot_info.png"};

    using enum Coords;

    static constexpr std::array<Element, 6> Elements{{
		Knob9mm{{to_mm<25>(-75.926), to_mm<25>(171.669), Center, "Ring", ""}, 0.0f},
		Knob9mm{{to_mm<25>(-76.926), to_mm<25>(75.008), Center, "Pitch", ""}, 0.0f},
		AnalogJackOutput4ms{{to_mm<25>(-76.926), to_mm<25>(417.669), Center, "Rimshot", ""}},
		AnalogJackInput4ms{{to_mm<25>(-77.926), to_mm<25>(349.669), Center, "Trig", ""}},
		AnalogJackInput4ms{{to_mm<25>(-76.926), to_mm<25>(221.669), Center, "Ring CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(-76.926), to_mm<25>(127.669), Center, "Pitch CV", ""}},
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
