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
    static constexpr std::string_view svg_filename{"res/Tom_info.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Tom_info.png"};

    using enum Coords;

    static constexpr std::array<Element, 11> Elements{{
		Toggle2posHoriz{{to_mm<25>(-93.591), to_mm<25>(155.903), Center, "Range", ""}},
		Knob9mm{{to_mm<25>(-140.726), to_mm<25>(207.669), Center, "Pitch Decay", ""}, 0.0f},
		Knob9mm{{to_mm<25>(-46.428), to_mm<25>(207.316), Center, "Env Depth", ""}, 0.0f},
		Knob9mm{{to_mm<25>(-46.428), to_mm<25>(93.493), Center, "Amp Decay", ""}, 0.0f},
		Knob9mm{{to_mm<25>(-141.042), to_mm<25>(93.918), Center, "Pitch", ""}, 0.0f},
		AnalogJackInput4ms{{to_mm<25>(-151.19), to_mm<25>(362.725), Center, "Pitch Decay CV", ""}},
		AnalogJackOutput4ms{{to_mm<25>(-37.671), to_mm<25>(418.023), Center, "Tom", ""}},
		AnalogJackInput4ms{{to_mm<25>(-37.726), to_mm<25>(362.841), Center, "Env Depth CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(-37.549), to_mm<25>(308.902), Center, "Amp Decay CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(-94.898), to_mm<25>(309.078), Center, "Pitch CV", ""}},
		AnalogJackInput4ms{{to_mm<25>(-151.372), to_mm<25>(309.084), Center, "Trig", ""}},
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
