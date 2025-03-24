#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct MaracaInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Maraca"};
    static constexpr std::string_view description{"Maraca"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Maraca.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Maraca.png"};

    using enum Coords;

    static constexpr std::array<Element, 4> Elements{{
		Knob9mm{{to_mm<25>(9.912), to_mm<25>(19.535), Center, "Decay", ""}, 0.0f},
		AnalogJackOutput4ms{{to_mm<25>(9.912), to_mm<25>(110.168), Center, "Maraca", ""}},
		AnalogJackInput4ms{{to_mm<25>(9.912), to_mm<25>(93.021), Center, "Trig", ""}},
		AnalogJackInput4ms{{to_mm<25>(9.912), to_mm<25>(33.947), Center, "Decay CV", ""}},
}};

    enum class Elem {
        DecayKnob,
        MaracaOut,
        TrigIn,
        DecayCvIn,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobDecay, 
        NumKnobs,
    };
    
    
    enum {
        InputTrig, 
        InputDecay_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputMaraca, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
