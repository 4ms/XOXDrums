#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct SnareInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Snare"};
    static constexpr std::string_view description{"Snare Drum Module"};
    static constexpr uint32_t width_hp = 15;
    static constexpr std::string_view svg_filename{"res/Snare_info.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Snare_info.png"};

    using enum Coords;

    static constexpr std::array<Element, 19> Elements{{
		Davies1900hBlackKnob{{to_mm<72>(43.5), to_mm<72>(46.41), Center, "Pitch", ""}, 0.1953125f},
		Davies1900hBlackKnob{{to_mm<72>(108.0), to_mm<72>(46.41), Center, "Pitch Decay", ""}, 0.1171875f},
		Davies1900hBlackKnob{{to_mm<72>(172.5), to_mm<72>(46.41), Center, "Pitch Env Amount", ""}, 0.1953125f},
		Davies1900hBlackKnob{{to_mm<72>(43.5), to_mm<72>(104.62), Center, "Noise Color", ""}, 0.1953125f},
		Davies1900hBlackKnob{{to_mm<72>(108.0), to_mm<72>(104.62), Center, "Noise Decay", ""}, 0.2578125f},
		Davies1900hBlackKnob{{to_mm<72>(172.5), to_mm<72>(104.62), Center, "Body Decay", ""}, 0.3125f},
		Davies1900hBlackKnob{{to_mm<72>(43.5), to_mm<72>(163.3), Center, "Body Noise", ""}, 0.3125f},
		Toggle2pos{{to_mm<72>(108.0), to_mm<72>(163.175), Center, "Range Switch", ""}, {"Low", "High"}},
		Davies1900hBlackKnob{{to_mm<72>(172.39), to_mm<72>(163.21), Center, "Saturation", ""}, 0.0f},
		GateJackInput4ms{{to_mm<72>(35.54), to_mm<72>(232.21), Center, "Trigger", ""}},
		AnalogJackInput4ms{{to_mm<72>(83.82), to_mm<72>(232.21), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(132.1), to_mm<72>(232.21), Center, "P Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(180.38), to_mm<72>(232.21), Center, "P Amt CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(35.54), to_mm<72>(272.34), Center, "N Color CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(83.82), to_mm<72>(272.34), Center, "N Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(132.1), to_mm<72>(272.34), Center, "B Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(180.38), to_mm<72>(272.34), Center, "BN CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(132.1), to_mm<72>(313.95), Center, "Saturation CV", ""}},
		AnalogJackOutput4ms{{to_mm<72>(180.38), to_mm<72>(313.95), Center, "Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        PitchDecayKnob,
        PitchEnvAmountKnob,
        NoiseColorKnob,
        NoiseDecayKnob,
        BodyDecayKnob,
        BodyNoiseKnob,
        RangeSwitch,
        SaturationKnob,
        TriggerIn,
        PitchCvIn,
        PDecayCvIn,
        PAmtCvIn,
        NColorCvIn,
        NDecayCvIn,
        BDecayCvIn,
        BnCvIn,
        SaturationCvIn,
        Out,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobPitch, 
        KnobPitch_Decay, 
        KnobPitch_Env_Amount, 
        KnobNoise_Color, 
        KnobNoise_Decay, 
        KnobBody_Decay, 
        KnobBody_Noise, 
        KnobSaturation, 
        NumKnobs,
    };
    
    enum {
        SwitchRange_Switch, 
        NumSwitches,
    };
    
    enum {
        InputTrigger, 
        InputPitch_Cv, 
        InputP_Decay_Cv, 
        InputP_Amt_Cv, 
        InputN_Color_Cv, 
        InputN_Decay_Cv, 
        InputB_Decay_Cv, 
        InputBn_Cv, 
        InputSaturation_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputOut, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
