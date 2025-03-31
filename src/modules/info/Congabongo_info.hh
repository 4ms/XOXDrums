#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct CongabongoInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Congabongo"};
    static constexpr std::string_view description{"Conga Bongo Drum Module"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/Congabongo_info.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Congabongo_info.png"};

    using enum Coords;

    static constexpr std::array<Element, 11> Elements{{
		Davies1900hBlackKnob{{to_mm<72>(72.11), to_mm<72>(57.85), Center, "Pitch", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<72>(72.11), to_mm<72>(119.09), Center, "Decay", ""}, 0.25f},
		Toggle2posHoriz{{to_mm<72>(72.11), to_mm<72>(173.825), Center, "Mode Switch", ""}, {"Conga", "Bongo"}},
		GateJackInput4ms{{to_mm<72>(29.51), to_mm<72>(232.2), Center, "Slap LO", ""}},
		AnalogJackInput4ms{{to_mm<72>(71.7), to_mm<72>(232.1), Center, "Pitch CV", ""}},
		GateJackInput4ms{{to_mm<72>(114.74), to_mm<72>(232.1), Center, "Slap HI", ""}},
		AnalogJackInput4ms{{to_mm<72>(29.51), to_mm<72>(272.23), Center, "Tone LO", ""}},
		AnalogJackInput4ms{{to_mm<72>(71.7), to_mm<72>(272.23), Center, "Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(114.74), to_mm<72>(272.23), Center, "Tone HI", ""}},
		AnalogJackOutput4ms{{to_mm<72>(29.51), to_mm<72>(313.84), Center, "Out LO", ""}},
		AnalogJackOutput4ms{{to_mm<72>(114.74), to_mm<72>(313.84), Center, "Out HI", ""}},
}};

    enum class Elem {
        PitchKnob,
        DecayKnob,
        ModeSwitch,
        SlapLoIn,
        PitchCvIn,
        SlapHiIn,
        ToneLoIn,
        DecayCvIn,
        ToneHiIn,
        OutLoOut,
        OutHiOut,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobPitch, 
        KnobDecay, 
        NumKnobs,
    };
    
    enum {
        SwitchMode_Switch, 
        NumSwitches,
    };
    
    enum {
        InputSlap_Lo, 
        InputPitch_Cv, 
        InputSlap_Hi, 
        InputTone_Lo, 
        InputDecay_Cv, 
        InputTone_Hi, 
        NumInJacks,
    };
    
    enum {
        OutputOut_Lo, 
        OutputOut_Hi, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
