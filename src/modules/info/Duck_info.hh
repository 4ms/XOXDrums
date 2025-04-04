#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct DuckInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Duck"};
    static constexpr std::string_view description{"Ducking Module"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Duck.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Duck_info.png"};

    using enum Coords;

    static constexpr std::array<Element, 7> Elements{{

        Knob9mm{{to_mm<72>(28.92), to_mm<72>(46.77), Center, "Time", ""}, 0.0f},
		Knob9mm{{to_mm<72>(28.92), to_mm<72>(143.15), Center, "Amount", ""}, 0.0f},
		AnalogJackOutput4ms{{to_mm<72>(28.92), to_mm<72>(313.94), Center, "Ducked", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.92), to_mm<72>(265.04), Center, "Audio", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.92), to_mm<72>(94.96), Center, "Time CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.92), to_mm<72>(191.34), Center, "Amount CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.92), to_mm<72>(216.73), Center, "Trigger", ""}},
}};

    enum class Elem {
        TimeKnob,
        AmountKnob,
        DuckedOut,
        AudioIn,
        TimeCvIn,
        AmountCvIn,
        TrigIn,
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
        InputInput, 
        NumInJacks,
    };
    
    enum {
        OutputOut, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
