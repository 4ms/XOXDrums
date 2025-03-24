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
		Knob9mm{{to_mm<25>(10.309), to_mm<25>(42.895), Center, "Time", ""}, 0.0f},
		Knob9mm{{to_mm<25>(10.309), to_mm<25>(16.645), Center, "Amount", ""}, 0.0f},
		AnalogJackOutput4ms{{to_mm<25>(10.309), to_mm<25>(110.361), Center, "Ducked", ""}},
		AnalogJackInput4ms{{to_mm<25>(10.309), to_mm<25>(92.883), Center, "Audio", ""}},
		AnalogJackInput4ms{{to_mm<25>(10.309), to_mm<25>(57.456), Center, "Time CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(10.309), to_mm<25>(31.143), Center, "Amount CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(10.309), to_mm<25>(76.072), Center, "Trig", ""}},
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
        OutputDucked, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
