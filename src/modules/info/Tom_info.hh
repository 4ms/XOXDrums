#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct TomInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Tom"};
    static constexpr std::string_view description{"Tom DrumModule"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/Tom.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Tom_info.png"};

    using enum Coords;

    static constexpr std::array<Element, 11> Elements{{
		Davies1900hBlackKnob{{to_mm<96>(50.267), to_mm<96>(76.934), Center, "Pitch", ""}, 0.390625f},
		Davies1900hBlackKnob{{to_mm<96>(141.734), to_mm<96>(76.934), Center, "Amp Decay", ""}, 0.390625f},
		Toggle3posHoriz{{to_mm<96>(95.847), to_mm<96>(148.256), Center, "Range Switch", ""}, {"2x", "10x", "100x"}, Toggle3posHoriz::State_t::CENTER},
		Davies1900hBlackKnob{{to_mm<96>(50.267), to_mm<96>(216.934), Center, "Pitch Decay", ""}, 0.0f},
		Davies1900hBlackKnob{{to_mm<96>(141.734), to_mm<96>(216.934), Center, "Env Depth", ""}, 0.390625f},
		GateJackInput4ms{{to_mm<96>(39.2), to_mm<96>(309.335), Center, "Trigger", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.467), to_mm<96>(309.335), Center, "Pitch CV", ""}},
		GateJackInput4ms{{to_mm<96>(152.801), to_mm<96>(309.335), Center, "A Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(39.2), to_mm<96>(362.802), Center, "P Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.801), to_mm<96>(362.802), Center, "Env Depth CV", ""}},
		AnalogJackOutput4ms{{to_mm<96>(152.934), to_mm<96>(418.269), Center, "Out", ""}},
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
