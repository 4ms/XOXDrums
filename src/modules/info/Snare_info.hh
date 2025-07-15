#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct SnareInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Snare"};
    static constexpr std::string_view description{"Snare DrumModule"};
    static constexpr uint32_t width_hp = 15;
    static constexpr std::string_view svg_filename{"res/Snare.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Snare.png"};

    using enum Coords;

    static constexpr std::array<Element, 20> Elements{{
		Davies1900hBlackKnob{{to_mm<96>(58.0), to_mm<96>(61.88), Center, "Pitch", ""}, 0.1953125f, 150.0, 380.0, "hz"},
		Davies1900hBlackKnob{{to_mm<96>(144.001), to_mm<96>(61.88), Center, "Pitch Decay", ""}, 0.1171875f, 5.0, 25.0, "ms"},
		Davies1900hBlackKnob{{to_mm<96>(230.001), to_mm<96>(61.88), Center, "Pitch Env Amount", ""}, 0.1953125f, 0.0, 100.0, "%"},
		Davies1900hBlackKnob{{to_mm<96>(58.0), to_mm<96>(139.494), Center, "Noise Color", ""}, 0.1953125f, 1.0, 5.0, "khz"},
		Davies1900hBlackKnob{{to_mm<96>(144.001), to_mm<96>(139.494), Center, "Noise Decay", ""}, 0.2578125f, 5.0, 80.0, "ms"},
		Davies1900hBlackKnob{{to_mm<96>(230.001), to_mm<96>(139.494), Center, "Body Decay", ""}, 0.3125f, 5.0, 50.0, "ms"},
		Davies1900hBlackKnob{{to_mm<96>(58.0), to_mm<96>(217.735), Center, "Body/Noise", ""}, 0.3125f, 0.0, 100.0, "%"},
		Toggle3posHoriz{{to_mm<96>(143.355), to_mm<96>(217.53), Center, "Range Switch", ""}, {"2x", "10x", "100x"}},
		Davies1900hBlackKnob{{to_mm<96>(229.855), to_mm<96>(217.615), Center, "Saturation", ""}, 0.78125f, 0.0, 100.0, "%"},
		WhiteMomentary7mm{{to_mm<96>(47.387), to_mm<96>(309.615), Center, "pushButton", ""}},
		GateJackInput4ms{{to_mm<96>(47.387), to_mm<96>(363.615), Center, "Trigger In", ""}},
		AnalogJackInput4ms{{to_mm<96>(111.761), to_mm<96>(309.615), Center, "Pitch CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(176.134), to_mm<96>(309.615), Center, "Pitch Decay CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(240.508), to_mm<96>(309.615), Center, "Pitch Amount CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(47.387), to_mm<96>(417.122), Center, "Noise Color CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(111.761), to_mm<96>(363.615), Center, "Noise Decay CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(176.134), to_mm<96>(363.122), Center, "Body Decay CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(240.508), to_mm<96>(363.122), Center, "Body/Noise CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(111.761), to_mm<96>(418.602), Center, "Saturation CV In", ""}},
		AnalogJackOutput4ms{{to_mm<96>(240.508), to_mm<96>(418.602), Center, "Audio Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        PitchDecayKnob,
        PitchEnvAmountKnob,
        NoiseColorKnob,
        NoiseDecayKnob,
        BodyDecayKnob,
        Body_NoiseKnob,
        RangeSwitch,
        SaturationKnob,
        PushButton,
        TriggerIn,
        PitchCvIn,
        PitchDecayCvIn,
        PitchAmountCvIn,
        NoiseColorCvIn,
        NoiseDecayCvIn,
        BodyDecayCvIn,
        Body_NoiseCvIn,
        SaturationCvIn,
        AudioOut,
    };
};
} // namespace MetaModule
