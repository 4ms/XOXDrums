#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct HiHatInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"HiHat"};
    static constexpr std::string_view description{"Hi Hat DrumModule"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/HiHat.svg"};
    static constexpr std::string_view png_filename{"4msDrums/HiHat.png"};

    using enum Coords;

    static constexpr std::array<Element, 13> Elements{{
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(77.134), Center, "Pitch", ""}, 0.5f},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(77.134), Center, "Decay", ""}, 0.0f},
		Toggle2pos{{to_mm<96>(96.876), to_mm<96>(145.592), Center, "Choke Switch", ""}, {"Off", "On"}},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(217.095), Center, "Brightness", ""}, 1.0f},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(217.095), Center, "Thickness", ""}, 1.0f},
		GateJackInput4ms{{to_mm<96>(39.347), to_mm<96>(309.468), Center, "CH Trig", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.601), to_mm<96>(309.468), Center, "Pitch CV", ""}},
		GateJackInput4ms{{to_mm<96>(152.987), to_mm<96>(309.468), Center, "OH Trig", ""}},
		AnalogJackInput4ms{{to_mm<96>(39.347), to_mm<96>(362.975), Center, "Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.601), to_mm<96>(362.975), Center, "Brightness CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.987), to_mm<96>(362.975), Center, "Thickness CV", ""}},
		AnalogJackOutput4ms{{to_mm<96>(39.294), to_mm<96>(418.456), Center, "CH Out", ""}},
		AnalogJackOutput4ms{{to_mm<96>(153.041), to_mm<96>(418.456), Center, "OH Out", ""}},
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
