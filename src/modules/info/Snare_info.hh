#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct SnareInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Snare"};
    static constexpr std::string_view description{"Snare"};
    static constexpr uint32_t width_hp = 15;
    static constexpr std::string_view svg_filename{"res/Snare.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Snare.png"};

    using enum Coords;

    static constexpr std::array<Element, 19> Elements{{
        Toggle3pos{{to_mm<25>(38.537), to_mm<25>(57.823), Center, "Range", ""}, {"2x", "10x", "100x"}, Toggle3pos::State_t::DOWN},
        Davies1900hBlackKnob{{to_mm<25>(61.279), to_mm<25>(57.823), Center, "Saturation", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(15.783), to_mm<25>(57.823), Center, "Body/Noise", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(15.783), to_mm<25>(37.122), Center, "Noise Color", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(38.537), to_mm<25>(37.122), Center, "Noise Decay", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(61.279), to_mm<25>(37.122), Center, "Body Decay", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(61.279), to_mm<25>(16.465), Center, "Pitch Env Amount", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(38.537), to_mm<25>(16.465), Center, "Pitch Decay", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(15.783), to_mm<25>(16.465), Center, "Pitch", ""}, 0.0f},

		AnalogJackInput4ms{{to_mm<25>(13.784), to_mm<25>(93.55), Center, "Noise Color CV", ""}},
		AnalogJackOutput4ms{{to_mm<25>(64.883), to_mm<25>(110.738), Center, "Snare", ""}},
		AnalogJackInput4ms{{to_mm<25>(39.037), to_mm<25>(110.973), Center, "Saturation CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(30.817), to_mm<25>(93.55), Center, "Noise Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(47.585), to_mm<25>(93.55), Center, "Body Decay CV", ""}},

		AnalogJackInput4ms{{to_mm<25>(64.883), to_mm<25>(93.55), Center, "Body/Noise CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(64.883), to_mm<25>(76.362), Center, "Pitch Env Amount CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(47.585), to_mm<25>(76.362), Center, "Pitch Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(30.817), to_mm<25>(76.362), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(13.784), to_mm<25>(76.362), Center, "Trigger", ""}},
}};

    enum class Elem {
        RangeSwitch,
        SaturationKnob,
        Body_NoiseKnob,
        NoiseColorKnob,
        NoiseDecayKnob,
        BodyDecayKnob,
        PitchEnvAmountKnob,
        PitchDecayKnob,
        PitchKnob,
        NoiseColorCvIn,
        SnareOut,
        SaturationCvIn,
        NoiseDecayCvIn,
        BodyDecayCvIn,
        Body_NoiseCvIn,
        PitchEnvAmountCvIn,
        PitchDecayCvIn,
        PitchIn,
        TriggerIn,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobSaturation, 
        KnobBody_Noise, 
        KnobNoise_Color, 
        KnobNoise_Decay, 
        KnobBody_Decay, 
        KnobPitch_Env_Amount, 
        KnobPitch_Decay, 
        KnobPitch, 
        NumKnobs,
    };
    
    enum {
        SwitchRange, 
        NumSwitches,
    };
    
    enum {
        InputNoise_Color_Cv, 
        InputSaturation_Cv, 
        InputNoise_Decay_Cv, 
        InputBody_Decay_Cv, 
        InputBody_Noise_Cv, 
        InputPitch_Env_Amount_Cv, 
        InputPitch_Decay_Cv, 
        InputPitch, 
        InputTrigger, 
        NumInJacks,
    };
    
    enum {
        OutputSnare, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
