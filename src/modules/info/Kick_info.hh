#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct KickInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Kick"};
    static constexpr std::string_view description{"Kick Drum Module"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/Kick_info.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Kick_info.png"};

    using enum Coords;

    static constexpr std::array<Element, 13> Elements{{
		Davies1900hBlackKnob{{to_mm<72>(37.85), to_mm<72>(46.52), Center, "Pitch", ""}, 0.328125f},
		Davies1900hBlackKnob{{to_mm<72>(106.4), to_mm<72>(46.42), Center, "P Depth", ""}, 0.5f},
		Davies1900hBlackKnob{{to_mm<72>(37.85), to_mm<72>(104.62), Center, "P Decay", ""}, 0.25f},
		Davies1900hBlackKnob{{to_mm<72>(106.4), to_mm<72>(104.62), Center, "Amp Decay", ""}, 0.328125f},
		Davies1900hBlackKnob{{to_mm<72>(37.85), to_mm<72>(162.82), Center, "Saturation", ""}, 0.328125f},
		Toggle3posHoriz{{to_mm<72>(106.4), to_mm<72>(163.175), Center, "Range Switch", ""}, {"2x", "10x", "100x"}, Toggle3posHoriz::State_t::LEFT},
		GateJackInput4ms{{to_mm<72>(29.51), to_mm<72>(232.1), Center, "Trig", ""}},
		AnalogJackInput4ms{{to_mm<72>(71.7), to_mm<72>(232.1), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(114.74), to_mm<72>(232.1), Center, "Depth CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(29.51), to_mm<72>(272.23), Center, "P Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(71.7), to_mm<72>(272.23), Center, "Amp Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(114.74), to_mm<72>(272.23), Center, "Saturation CV", ""}},
		AnalogJackOutput4ms{{to_mm<72>(114.74), to_mm<72>(313.84), Center, "Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        PDepthKnob,
        PDecayKnob,
        AmpDecayKnob,
        SaturationKnob,
        RangeSwitch,
        TrigIn,
        PitchCvIn,
        DepthCvIn,
        PDecayCvIn,
        AmpDecayCvIn,
        SaturationCvIn,
        Out,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobPitch, 
        KnobP_Depth, 
        KnobP_Decay, 
        KnobAmp_Decay, 
        KnobSaturation, 
        NumKnobs,
    };
    
    enum {
        SwitchRange_Switch, 
        NumSwitches,
    };
    
    enum {
        InputTrig, 
        InputPitch_Cv, 
        InputDepth_Cv, 
        InputP_Decay_Cv, 
        InputAmp_Decay_Cv, 
        InputSaturation_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputOut, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
