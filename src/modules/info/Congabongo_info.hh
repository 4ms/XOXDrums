#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct CongabongoInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Congabongo"};
    static constexpr std::string_view description{"Congabongo"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/Congabongo.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Congabongo.png"};

    using enum Coords;

    static constexpr std::array<Element, 11> Elements{{
		Toggle2posHoriz{{to_mm<25>(25.44), to_mm<25>(61.232), Center, "Select", ""}},
        Davies1900hBlackKnob{{to_mm<25>(25.44), to_mm<25>(40.701), Center, "Decay", ""}, 0.0f},
        Davies1900hBlackKnob{{to_mm<25>(25.44), to_mm<25>(20.169), Center, "Pitch", ""}, 0.0f},
		AnalogJackOutput4ms{{to_mm<25>(11.063), to_mm<25>(111.127), Center, "Out HI", ""}},
		AnalogJackOutput4ms{{to_mm<25>(41.129), to_mm<25>(111.127), Center, "Out LO", ""}},
		AnalogJackInput4ms{{to_mm<25>(11.063), to_mm<25>(96.45), Center, "Tone HI Gate", ""}},
		AnalogJackInput4ms{{to_mm<25>(25.44), to_mm<25>(96.45), Center, "Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(41.129), to_mm<25>(96.45), Center, "Tone LO Gate", ""}},
		AnalogJackInput4ms{{to_mm<25>(11.063), to_mm<25>(82.292), Center, "Slap HI Gate", ""}},
		AnalogJackInput4ms{{to_mm<25>(25.44), to_mm<25>(82.292), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(41.129), to_mm<25>(82.292), Center, "Slap LO Gate", ""}},
}};

    enum class Elem {
        RangeSwitch,
        DecayKnob,
        PitchKnob,
        OutHiOut,
        OutLoOut,
        ToneHiGateIn,
        DecayCvIn,
        ToneLoGateIn,
        SlapHiGateIn,
        PitchCvIn,
        SlapLoGateIn,
    };

    // Legacy naming (safe to remove once all legacy 4ms CoreModules are converted)
    
    enum {
        KnobDecay, 
        KnobPitch, 
        NumKnobs,
    };
    
    enum {
        SwitchRange, 
        NumSwitches,
    };
    
    enum {
        InputTone_Hi_Cv, 
        InputDecay_Cv, 
        InputTone_Lo_Cv, 
        InputSlap_Hi_Cv, 
        InputPitch_Cv, 
        InputSlap_Lo_Cv, 
        NumInJacks,
    };
    
    enum {
        OutputOut_Hi, 
        OutputOut_Lo, 
        NumOutJacks,
    };
    
    
};
} // namespace MetaModule
