#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct AccentInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Accent"};
    static constexpr std::string_view description{"Accent"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Accent.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Accent.png"};

    using enum Coords;

    static constexpr std::array<Element, 5> Elements{{
		Knob9mm{{to_mm<25>(10.16), to_mm<25>(16.4), Center, "Amount", ""}, 1.0f},
		AnalogJackOutput4ms{{to_mm<25>(10.16), to_mm<25>(110.361), Center, "Accented", ""}},
		GateJackInput4ms{{to_mm<25>(10.16), to_mm<25>(92.883), Center, "Audio", ""}},
		GateJackInput4ms{{to_mm<25>(10.16), to_mm<25>(31.143), Center, "Amount CV", ""}},
		GateJackInput4ms{{to_mm<25>(10.16), to_mm<25>(76.072), Center, "Trig", ""}},
}};

    enum class Elem {
        AmountKnob,
        AccentedOut,
        AudioIn,
        AmountCvIn,
        TrigIn,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobAmount, 
        NumKnobs,
    };
    
    
    enum {
        InputAudio, 
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
