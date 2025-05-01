#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct HiHatInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"HiHat"};
    static constexpr std::string_view description{"Hi Hat Drum Module"};
    static constexpr uint32_t width_hp = 720;
    static constexpr std::string_view svg_filename{"res/HiHat.svg"};
    static constexpr std::string_view png_filename{"4msDrums/HiHat.png"};

    using enum Coords;

    static constexpr std::array<Element, 13> Elements{{
		Davies1900hBlackKnob{{to_mm<72>(37.85), to_mm<72>(57.85), Center, "Pitch", ""}, 0.5f},
		Davies1900hBlackKnob{{to_mm<72>(106.4), to_mm<72>(57.85), Center, "Decay", ""}, 0.0f},
		Toggle2posHoriz{{to_mm<72>(72.13), to_mm<72>(110.335), Center, "Choke Switch", ""}, {"Off", "On"}},
		Davies1900hBlackKnob{{to_mm<72>(37.85), to_mm<72>(162.82), Center, "Brightness", ""}, 1.0f},
		Davies1900hBlackKnob{{to_mm<72>(106.4), to_mm<72>(162.82), Center, "Thickness", ""}, 1.0f},
		GateJackInput4ms{{to_mm<72>(29.51), to_mm<72>(232.1), Center, "CH Trig", ""}},
		AnalogJackInput4ms{{to_mm<72>(71.7), to_mm<72>(232.1), Center, "Pitch CV", ""}},
		GateJackInput4ms{{to_mm<72>(114.74), to_mm<72>(232.1), Center, "OH Trig", ""}},
		AnalogJackInput4ms{{to_mm<72>(29.51), to_mm<72>(272.23), Center, "Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(71.7), to_mm<72>(272.23), Center, "Brightness CV", ""}},
		AnalogJackInput4ms{{to_mm<72>(114.74), to_mm<72>(272.23), Center, "Thickness CV", ""}},
		AnalogJackOutput4ms{{to_mm<72>(29.47), to_mm<72>(313.84), Center, "CH Out", ""}},
		AnalogJackOutput4ms{{to_mm<72>(114.78), to_mm<72>(313.84), Center, "OH Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        DecayKnob,
        ChokeSwitch,
        BrightnessKnob,
        ThicknessKnob,
        ChTrigIn,
        PitchCvIn,
        OhTrigIn,
        DecayCvIn,
        BrightnessCvIn,
        ThicknessCvIn,
        ChOut,
        OhOut,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobPitch, 
        KnobDecay, 
        KnobBrightness, 
        KnobThickness, 
        NumKnobs,
    };
    
    enum {
        SwitchChoke_Switch, 
        NumSwitches,
    };
    
    enum {
        InputCh_Trig, 
        InputPitch_Cv, 
        InputOh_Trig, 
        InputDecay_Cv, 
        InputBrightness_Cv, 
        InputThickness_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputCh_Out, 
        OutputOh_Out, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
