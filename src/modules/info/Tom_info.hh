#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct TomInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Tom"};
    static constexpr std::string_view description{"Tom"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/Tom.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Tom.png"};

    using enum Coords;

    static constexpr std::array<Element, 11> Elements{{
		Toggle3pos{{to_mm<25>(26), to_mm<25>(41), Center, "Range", ""}, {"Low", "Med", "High"}, Toggle3pos::State_t::DOWN},
		Davies1900hBlackKnob{{to_mm<25>(13.3), to_mm<25>(54.8), Center, "Pitch Decay", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(38.3), to_mm<25>(54.8), Center, "Env Depth", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(38.3), to_mm<25>(24.8), Center, "Amp Decay", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(13.3), to_mm<25>(24.8), Center, "Pitch", ""}, 0.0f},
		AnalogJackInput4ms{{to_mm<25>(11.063), to_mm<25>(96.45), Center, "Pitch Decay CV", ""}},
		AnalogJackOutput4ms{{to_mm<25>(41.129), to_mm<25>(111.127), Center, "Tom", ""}},
		AnalogJackInput4ms{{to_mm<25>(41.129), to_mm<25>(96.45), Center, "Env Depth CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(41.129), to_mm<25>(82.292), Center, "Amp Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(25.44), to_mm<25>(82.292), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(11.063), to_mm<25>(82.292), Center, "Trig", ""}},
}};

    enum class Elem {
        RangeSwitch,
        PitchDecayKnob,
        EnvDepthKnob,
        AmpDecayKnob,
        PitchKnob,
        PitchDecayCvIn,
        TomOut,
        EnvDepthCvIn,
        AmpDecayCvIn,
        PitchCvIn,
        TrigIn,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobPitch_Decay, 
        KnobEnv_Depth, 
        KnobAmp_Decay, 
        KnobPitch, 
        NumKnobs,
    };
    
    enum {
        SwitchRange, 
        NumSwitches,
    };
    
    enum {
        InputPitch_Decay_Cv, 
        InputEnv_Depth_Cv, 
        InputAmp_Decay_Cv, 
        InputPitch_Cv, 
        InputTrig, 
        NumInJacks,
    };
    
    enum {
        OutputTom, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
