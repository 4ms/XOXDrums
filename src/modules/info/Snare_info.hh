#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct SnareInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Snare"};
    static constexpr std::string_view description{"Snare DrumModule"};
    static constexpr uint32_t width_hp = 15;
    static constexpr std::string_view svg_filename{"res/Snare.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Snare.png"};

    using enum Coords;

    static constexpr std::array<Element, 19> Elements{{
		Davies1900hBlackKnob{{to_mm<96>(58.0), to_mm<96>(61.88), Center, "Pitch", ""}, 0.1953125f},
		Davies1900hBlackKnob{{to_mm<96>(144.001), to_mm<96>(61.88), Center, "Pitch Decay", ""}, 0.1171875f},
		Davies1900hBlackKnob{{to_mm<96>(230.001), to_mm<96>(61.88), Center, "Pitch Env Amount", ""}, 0.1953125f},
		Davies1900hBlackKnob{{to_mm<96>(58.0), to_mm<96>(139.494), Center, "Noise Color", ""}, 0.1953125f},
		Davies1900hBlackKnob{{to_mm<96>(144.001), to_mm<96>(139.494), Center, "Noise Decay", ""}, 0.2578125f},
		Davies1900hBlackKnob{{to_mm<96>(230.001), to_mm<96>(139.494), Center, "Body Decay", ""}, 0.3125f},
		Davies1900hBlackKnob{{to_mm<96>(58.0), to_mm<96>(217.735), Center, "Body Noise", ""}, 0.3125f},
		Toggle3posHoriz{{to_mm<96>(143.355), to_mm<96>(217.53), Center, "Range Switch", ""}, {"2x", "10x", "100x"}},
		Davies1900hBlackKnob{{to_mm<96>(229.855), to_mm<96>(217.615), Center, "Saturation", ""}, 0.78125f},
		GateJackInput4ms{{to_mm<96>(47.387), to_mm<96>(309.615), Center, "Trigger", ""}},
		AnalogJackInput4ms{{to_mm<96>(111.761), to_mm<96>(309.615), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(176.134), to_mm<96>(309.615), Center, "P Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(240.508), to_mm<96>(309.615), Center, "P Amt CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(47.387), to_mm<96>(363.122), Center, "N Color CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(111.761), to_mm<96>(363.122), Center, "N Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(176.134), to_mm<96>(363.122), Center, "B Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(240.508), to_mm<96>(363.122), Center, "BN CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(176.134), to_mm<96>(418.602), Center, "Saturation CV", ""}},
		AnalogJackOutput4ms{{to_mm<96>(240.508), to_mm<96>(418.602), Center, "Out", ""}},
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
