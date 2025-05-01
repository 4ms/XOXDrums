#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct TomInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Tom"};
    static constexpr std::string_view description{"Tom Drum Module"};
    static constexpr uint32_t width_hp = 720;
    static constexpr std::string_view svg_filename{"res/Tom.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Tom.png"};

    using enum Coords;

    static constexpr std::array<Element, 11> Elements{{
		Davies1900hBlackKnob{{to_mm<72>(37.7), to_mm<72>(57.7), Center, "Pitch", ""}, 0.390625f},
		Davies1900hBlackKnob{{to_mm<72>(106.3), to_mm<72>(57.7), Center, "Amp Decay", ""}, 0.390625f},
		Toggle3posHoriz{{to_mm<72>(72.0), to_mm<72>(110.155), Center, "Range Switch", ""}, {"2x", "10x", "100x"}, Toggle3posHoriz::State_t::CENTER},
		Davies1900hBlackKnob{{to_mm<72>(37.7), to_mm<72>(162.7), Center, "Pitch Decay", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<72>(106.3), to_mm<72>(162.7), Center, "Env Depth", ""}, 0.390625f},
		GateJackInput4ms{{to_mm<72>(29.4), to_mm<72>(232.0), Center, "Trigger", ""}},
		AnalogJackInput4ms{{to_mm<72>(71.6), to_mm<72>(232.0), Center, "Pitch CV", ""}},
		GateJackInput4ms{{to_mm<72>(114.6), to_mm<72>(232.0), Center, "A Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(29.4), to_mm<72>(272.1), Center, "P Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(114.6), to_mm<72>(272.1), Center, "Env Depth CV", ""}},
		AnalogJackOutput4ms{{to_mm<72>(114.7), to_mm<72>(313.7), Center, "Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        AmpDecayKnob,
        RangeSwitch,
        PitchDecayKnob,
        EnvDepthKnob,
        TriggerIn,
        PitchCvIn,
        ADecayCvIn,
        PDecayCvIn,
        EnvDepthCvIn,
        Out,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobPitch, 
        KnobAmp_Decay, 
        KnobPitch_Decay, 
        KnobEnv_Depth, 
        NumKnobs,
    };
    
    enum {
        SwitchRange_Switch, 
        NumSwitches,
    };
    
    enum {
        InputTrigger, 
        InputPitch_Cv, 
        InputA_Decay_Cv, 
        InputP_Decay_Cv, 
        InputEnv_Depth_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputOut, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
