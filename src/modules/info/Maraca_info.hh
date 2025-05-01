#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct MaracaInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Maraca"};
    static constexpr std::string_view description{"Maraca DrumModule"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Maraca.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Maraca.png"};

    using enum Coords;

    static constexpr std::array<Element, 4> Elements{{
		Knob9mm{{to_mm<96>(38.56), to_mm<96>(62.36), Center, "Decay", ""}, 0.5f},
		AnalogJackInput4ms{{to_mm<96>(38.56), to_mm<96>(126.614), Center, "Decay CV", ""}},
		GateJackInput4ms{{to_mm<96>(38.4), to_mm<96>(341.8), Center, "Trigger", ""}},
		AnalogJackOutput4ms{{to_mm<96>(38.4), to_mm<96>(406.867), Center, "Out", ""}},
}};

    enum class Elem {
        DecayKnob,
        DecayCvIn,
        TriggerIn,
        Out,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobDecay, 
        NumKnobs,
    };
    
    
    enum {
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
