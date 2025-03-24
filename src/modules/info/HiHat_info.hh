#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct HiHatInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"HiHat"};
    static constexpr std::string_view description{"HiHat"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/HiHat.svg"};
    static constexpr std::string_view png_filename{"4msDrums/HiHat.png"};

    using enum Coords;

    static constexpr std::array<Element, 13> Elements{{
		Toggle2posHoriz{{to_mm<25>(26), to_mm<25>(41), Center, "Off | On", ""}},
		Davies1900hBlackKnob{{to_mm<25>(38.3), to_mm<25>(54.8), Center, "Thickness", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(13.3), to_mm<25>(54.8), Center, "Brightness", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(38.3), to_mm<25>(24.8), Center, "Decay", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<25>(13.3), to_mm<25>(24.8), Center, "Pitch", ""}, 0.0f},
		AnalogJackInput4ms{{to_mm<25>(11.063), to_mm<25>(96.45), Center, "Closed Trig", ""}},
		AnalogJackOutput4ms{{to_mm<25>(41.129), to_mm<25>(111.127), Center, "Open Out", ""}},
		AnalogJackOutput4ms{{to_mm<25>(11.063), to_mm<25>(111.127), Center, "Closed Out", ""}},
		AnalogJackInput4ms{{to_mm<25>(41.129), to_mm<25>(82.292), Center, "Brightness CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(41.129), to_mm<25>(96.45), Center, "Open Trig", ""}},
		AnalogJackInput4ms{{to_mm<25>(25.44), to_mm<25>(96.45), Center, "Thickness CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(25.44), to_mm<25>(82.292), Center, "Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(11.063), to_mm<25>(82.292), Center, "Pitch CV", ""}},
}};

    enum class Elem {
        ChokeSwitch,
        ThicknessKnob,
        BrightnessKnob,
        DecayKnob,
        PitchKnob,
        ClosedTrigIn,
        OpenOut,
        ClosedOut,
        BrightnessCvIn,
        OpenTrigIn,
        ThicknessCvIn,
        DecayCvIn,
        PitchCvIn,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobThickness, 
        KnobBrightness, 
        KnobDecay, 
        KnobPitch, 
        NumKnobs,
    };
    
    enum {
        SwitchChoke, 
        NumSwitches,
    };
    
    enum {
        InputClosed_Trig, 
        InputBrightness_Cv, 
        InputOpen_Trig, 
        InputThickness_Cv, 
        InputDecay_Cv, 
        InputPitch_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputOpen_Out, 
        OutputClosed_Out, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
