#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct ClapInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Clap"};
    static constexpr std::string_view description{"Clap DrumModule"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/Clap.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Clap.png"};

    using enum Coords;

    static constexpr std::array<Element, 14> Elements{{
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(62.027), Center, "Energy", ""}, 0.25f},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(61.894), Center, "Spread", ""}, 0.25f},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(139.494), Center, "Color", ""}, 0.25f},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(139.494), Center, "Verb Decay", ""}, 0.25f},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(217.095), Center, "Verb Volume", ""}, 0.25f},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(217.095), Center, "Saturation", ""}, 0.25f},
		GateJackInput4ms{{to_mm<96>(39.294), to_mm<96>(309.468), Center, "Trig", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.534), to_mm<96>(309.468), Center, "Energy CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.934), to_mm<96>(309.468), Center, "Spread CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(39.294), to_mm<96>(362.975), Center, "Color CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.534), to_mm<96>(362.975), Center, "Verb Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.934), to_mm<96>(362.975), Center, "Verb Volume CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(39.294), to_mm<96>(418.456), Center, "Saturation CV", ""}},
		AnalogJackOutput4ms{{to_mm<96>(153.041), to_mm<96>(418.456), Center, "Out", ""}},
}};

    enum class Elem {
        EnergyKnob,
        SpreadKnob,
        ColorKnob,
        VerbDecayKnob,
        VerbVolumeKnob,
        SaturationKnob,
        TrigIn,
        EnergyCvIn,
        SpreadCvIn,
        ColorCvIn,
        VerbDecayCvIn,
        VerbVolumeCvIn,
        SaturationCvIn,
        Out,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobEnergy, 
        KnobSpread, 
        KnobColor, 
        KnobVerb_Decay, 
        KnobVerb_Volume, 
        KnobSaturation, 
        NumKnobs,
    };
    
    
    enum {
        InputTrig, 
        InputEnergy_Cv, 
        InputSpread_Cv, 
        InputColor_Cv, 
        InputVerb_Decay_Cv, 
        InputVerb_Volume_Cv, 
        InputSaturation_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputOut, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
