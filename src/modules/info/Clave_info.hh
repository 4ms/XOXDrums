#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct ClaveInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Clave"};
    static constexpr std::string_view description{"Clave DrumModule"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Clave.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Clave.png"};

    using enum Coords;

    static constexpr std::array<Element, 6> Elements{{
		Knob9mm{{to_mm<96>(38.56), to_mm<96>(70.115), Center, "Pitch", ""}, 0.5f},
		Knob9mm{{to_mm<96>(38.56), to_mm<96>(198.622), Center, "Decay", ""}, 0.5f},
		AnalogJackInput4ms{{to_mm<96>(38.56), to_mm<96>(134.369), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(38.56), to_mm<96>(262.876), Center, "Decay CV", ""}},
		GateJackInput4ms{{to_mm<96>(38.56), to_mm<96>(341.757), Center, "Trigger", ""}},
		AnalogJackOutput4ms{{to_mm<96>(38.56), to_mm<96>(406.824), Center, "Out", ""}},
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
