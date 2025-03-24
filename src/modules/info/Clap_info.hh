#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct ClapInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Clap"};
    static constexpr std::string_view description{"Clap"};
    static constexpr uint32_t width_hp = 12;
    static constexpr std::string_view svg_filename{"res/Clap.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Clap.png"};

    using enum Coords;

    static constexpr std::array<Element, 14> Elements{{
		Davies1900hBlackKnob{{to_mm<25>(43.435), to_mm<25>(57.5), Center, "Saturation", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(19.175), to_mm<25>(57.5), Center, "Verb Volume", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(43.435), to_mm<25>(37), Center, "Verb Decay", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(19.175), to_mm<25>(37), Center, "Color", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(43.435), to_mm<25>(16.383), Center, "Spread", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(19.175), to_mm<25>(16.383), Center, "Energy", ""}, 0.0f},
		AnalogJackOutput4ms{{to_mm<25>(46), to_mm<25>(111), Center, "Clap", ""}},
		AnalogJackInput4ms{{to_mm<25>(17.5), to_mm<25>(111), Center, "Trig", ""}},
		AnalogJackInput4ms{{to_mm<25>(17.5), to_mm<25>(96.3), Center, "Verb Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(32.3), to_mm<25>(96.3), Center, "Verb Volume CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(46), to_mm<25>(96.3), Center, "Saturation CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(46), to_mm<25>(81.5), Center, "Color CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(32.3), to_mm<25>(81.5), Center, "Spread CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(17.5), to_mm<25>(81.5), Center, "Energy CV", ""}},
}};

    enum class Elem {
        SaturationKnob,
        VerbVolumeKnob,
        VerbDecayKnob,
        ColorKnob,
        SpreadKnob,
        EnergyKnob,
        ClapOut,
        TrigIn,
        VerbDecayCvIn,
        VerbVolumeCvIn,
        SaturationCvIn,
        ColorCvIn,
        SpreadCvIn,
        EnergyCvIn,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobSaturation, 
        KnobVerb_Volume, 
        KnobVerb_Decay, 
        KnobColor, 
        KnobSpread, 
        KnobEnergy, 
        NumKnobs,
    };
    
    
    enum {
        InputTrig, 
        InputVerb_Decay_Cv, 
        InputVerb_Volume_Cv, 
        InputSaturation_Cv, 
        InputColor_Cv, 
        InputSpread_Cv, 
        InputEnergy_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputClap, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
