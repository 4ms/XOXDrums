#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct KickInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Kick"};
    static constexpr std::string_view description{"Kick"};
    static constexpr uint32_t width_hp = 12;
    static constexpr std::string_view svg_filename{"res/Kick.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Kick.png"};

    using enum Coords;

    static constexpr std::array<Element, 13> Elements{{
        Davies1900hBlackKnob{{to_mm<25>(19.101), to_mm<25>(57.44), Center, "Saturation", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(19.101), to_mm<25>(36.908), Center, "Pitch Decay", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(43.281), to_mm<25>(36.908), Center, "Amp Decay", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(43.2818), to_mm<25>(16.377), Center, "Pitch Depth", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(19.101), to_mm<25>(16.377), Center, "Pitch", ""}, 0.0f},
		Toggle2pos{{to_mm<25>(43.281), to_mm<25>(57.44), Center, "Range", ""}},
		AnalogJackOutput4ms{{to_mm<25>(46.866), to_mm<25>(111.215), Center, "Kick", ""}},
		AnalogJackInput4ms{{to_mm<25>(46.866), to_mm<25>(96.538), Center, "Saturation CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(31.691), to_mm<25>(96.538), Center, "Amp Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(16.810), to_mm<25>(96.538), Center, "Pitch Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(46.876), to_mm<25>(82.38), Center, "Depth CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(31.691), to_mm<25>(82.38), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(16.81), to_mm<25>(82.38), Center, "Trig", ""}},
}};

    enum class Elem {
        SaturationKnob,
        PitchDecayKnob,
        AmpDecayKnob,
        PitchDepthKnob,
        PitchKnob,
        RangeSwitch,
        KickOut,
        SaturationCvIn,
        AmpDecayCvIn,
        PitchDecayCvIn,
        DepthCvIn,
        PitchCvIn,
        TrigIn,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobSaturation, 
        KnobPitch_Decay, 
        KnobAmp_Decay, 
        KnobPitch_Depth, 
        KnobPitch, 
        NumKnobs,
    };
    
    enum {
        SwitchRange, 
        NumSwitches,
    };
    
    enum {
        InputSaturation_Cv, 
        InputAmp_Decay_Cv, 
        InputPitch_Decay_Cv, 
        InputDepth_Cv, 
        InputPitch_Cv, 
        InputTrig, 
        NumInJacks,
    };
    
    enum {
        OutputKick, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
