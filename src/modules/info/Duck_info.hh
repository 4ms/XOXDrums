#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct DuckInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Duck"};
    static constexpr std::string_view description{"Ducking Module"};
    static constexpr uint32_t width_hp = 288;
    static constexpr std::string_view svg_filename{"res/Duck.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Duck.png"};

    using enum Coords;

    static constexpr std::array<Element, 7> Elements{{
		Knob9mm{{to_mm<72>(28.8), to_mm<72>(46.63), Center, "Amount", ""}, 1.0f},
		Knob9mm{{to_mm<72>(28.8), to_mm<72>(128.83), Center, "Time", ""}, 0.25f},
		AnalogJackInput4ms{{to_mm<72>(28.8), to_mm<72>(94.82), Center, "Amount CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.8), to_mm<72>(177.02), Center, "Time CV", ""}},
		GateJackInput4ms{{to_mm<72>(28.8), to_mm<72>(216.71), Center, "Trigger", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.8), to_mm<72>(264.9), Center, "Input", ""}},
		AnalogJackOutput4ms{{to_mm<72>(28.8), to_mm<72>(313.71), Center, "Out", ""}},
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
