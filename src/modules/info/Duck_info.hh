#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct DuckInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Duck"};
    static constexpr std::string_view description{"Duck"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Duck.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Duck.png"};

    using enum Coords;

    static constexpr std::array<Element, 7> Elements{{
		Knob9mm{{to_mm<72>(28.562), to_mm<72>(121.586), Center, "Time", ""}, 0.0f},
		Knob9mm{{to_mm<72>(29.22), to_mm<72>(47.182), Center, "Amount", ""}, 0.0f},
		AnalogJackOutput4ms{{to_mm<72>(28.865), to_mm<72>(312.821), Center, "Accented", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.115), to_mm<72>(263.279), Center, "Audio", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.201), to_mm<72>(162.862), Center, "Time CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.56), to_mm<72>(88.276), Center, "Amount CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(28.56), to_mm<72>(215.63), Center, "Trig", ""}},
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
        KnobTime, 
        KnobAmount, 
        NumKnobs,
    };
    
    
    enum {
        InputAudio, 
        InputTime_Cv, 
        InputAmount_Cv, 
        InputTrig, 
        NumInJacks,
    };
    
    enum {
        OutputAccented, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
