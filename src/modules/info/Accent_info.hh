#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct AccentInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Accent"};
    static constexpr std::string_view description{"Drum Accent Module"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Accent_info.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Accent_info.png"};

    using enum Coords;

    static constexpr std::array<Element, 5> Elements{{
		Knob9mm{{to_mm<72>(28.8), to_mm<72>(46.65), Center, "Amount", ""}, 0.5f},
		AnalogJackInput4ms{{to_mm<72>(28.8), to_mm<72>(94.84), Center, "Amount CV", ""}},
		GateJackInput4ms{{to_mm<72>(28.68), to_mm<72>(216.73), Center, "Trigger", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.68), to_mm<72>(264.91), Center, "Input", ""}},
		AnalogJackOutput4ms{{to_mm<72>(28.68), to_mm<72>(313.71), Center, "Out", ""}},
}};

    enum class Elem {
        AmountKnob,
        AmountCvIn,
        TriggerIn,
        InputIn,
        Out,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobAmount, 
        NumKnobs,
    };
    
    
    enum {
        InputAmount_Cv, 
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
