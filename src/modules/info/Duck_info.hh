#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct DuckInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Duck"};
    static constexpr std::string_view description{"DuckingModule"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Duck.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Duck_info.png"};

    using enum Coords;

    static constexpr std::array<Element, 7> Elements{{
		Knob9mm{{to_mm<96>(38.4), to_mm<96>(62.174), Center, "Amount", ""}, 1.0f},
		Knob9mm{{to_mm<96>(38.4), to_mm<96>(171.774), Center, "Time", ""}, 0.25f},
		AnalogJackInput4ms{{to_mm<96>(38.4), to_mm<96>(115.595), Center, "Amount CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(38.4), to_mm<96>(227.44), Center, "Time CV", ""}},
		GateJackInput4ms{{to_mm<96>(38.4), to_mm<96>(277.519), Center, "Trigger", ""}},
		AnalogJackInput4ms{{to_mm<96>(38.4), to_mm<96>(341.772), Center, "Input", ""}},
		AnalogJackOutput4ms{{to_mm<96>(38.4), to_mm<96>(406.853), Center, "Out", ""}},
}};

    enum class Elem {
        AmountKnob,
        TimeKnob,
        AmountCvIn,
        TimeCvIn,
        TriggerIn,
        InputIn,
        Out,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobAmount, 
        KnobTime, 
        NumKnobs,
    };
    
    
    enum {
        InputAmount_Cv, 
        InputTime_Cv, 
        InputTrigger, 
        InputInput, 
        NumInJacks,
    };
    
    enum {
        OutputOut, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
