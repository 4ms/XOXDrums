#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct ClapInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Clap"};
    static constexpr std::string_view description{"Clap Drum Module"};
    static constexpr uint32_t width_hp = 24;
    static constexpr std::string_view svg_filename{"res/Clap.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Clap.png"};

    using enum Coords;

    static constexpr std::array<Element, 14> Elements{{
		Davies1900hBlackKnob{{to_mm<72>(37.85), to_mm<72>(46.52), Center, "Energy", ""}, 0.5f},
		Davies1900hBlackKnob{{to_mm<72>(106.4), to_mm<72>(46.42), Center, "Spread", ""}, 0.5f},
		Davies1900hBlackKnob{{to_mm<72>(37.85), to_mm<72>(104.62), Center, "Color", ""}, 0.5f},
		Davies1900hBlackKnob{{to_mm<72>(106.4), to_mm<72>(104.62), Center, "Verb Decay", ""}, 0.5f},
		Davies1900hBlackKnob{{to_mm<72>(37.85), to_mm<72>(162.82), Center, "Verb Volume", ""}, 0.5f},
		Davies1900hBlackKnob{{to_mm<72>(106.4), to_mm<72>(162.82), Center, "Saturation", ""}, 0.5f},
		GateJackInput4ms{{to_mm<72>(29.47), to_mm<72>(232.1), Center, "Trig", ""}},
		AnalogJackInput4ms{{to_mm<72>(71.65), to_mm<72>(232.1), Center, "Energy CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(114.7), to_mm<72>(232.1), Center, "Spread CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(29.47), to_mm<72>(272.23), Center, "Color CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(71.65), to_mm<72>(272.23), Center, "Verb Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(114.7), to_mm<72>(272.23), Center, "Verb Volume CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(29.47), to_mm<72>(313.84), Center, "Saturation CV", ""}},
		AnalogJackOutput4ms{{to_mm<72>(114.78), to_mm<72>(313.84), Center, "Out", ""}},
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
