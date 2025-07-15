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
    static constexpr std::string_view png_filename{"4ms-XOXDrums/HiHat.png"};

    using enum Coords;

    static constexpr std::array<Element, 15> Elements{{
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(77.134), Center, "Pitch", ""}, 0.5f, 1.0, 2.0, "khz"},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(77.134), Center, "Decay", ""}, 0.0f, 50.0, 300.0, "ms"},
		Toggle2pos{{to_mm<96>(96.876), to_mm<96>(133.592), Center, "Choke Switch", ""}, {"Off", "On"}},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(201.095), Center, "Brightness", ""}, 1.0f, 1.0, 15.0, "khz"},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(201.095), Center, "Thickness", ""}, 1.0f, 10.0, 1.0, "khz"},
		WhiteMomentary7mm{{to_mm<96>(39.347), to_mm<96>(264.589), Center, "ChPushButton", ""}},
		WhiteMomentary7mm{{to_mm<96>(152.987), to_mm<96>(264.589), Center, "OhPushButton", ""}},
		GateJackInput4ms{{to_mm<96>(39.347), to_mm<96>(309.468), Center, "Closed HiHat Trigger In", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.601), to_mm<96>(309.468), Center, "Pitch CV In", ""}},
		GateJackInput4ms{{to_mm<96>(152.987), to_mm<96>(309.468), Center, "Open HiHat Trigger In", ""}},
		AnalogJackInput4ms{{to_mm<96>(39.347), to_mm<96>(362.975), Center, "Decay CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.601), to_mm<96>(362.975), Center, "Brightness CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.987), to_mm<96>(362.975), Center, "Thickness CV In", ""}},
		AnalogJackOutput4ms{{to_mm<96>(39.294), to_mm<96>(418.456), Center, "Closed HiHat Audio Out", ""}},
		AnalogJackOutput4ms{{to_mm<96>(153.041), to_mm<96>(418.456), Center, "Open HiHat Audio Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        DecayKnob,
        ChokeSwitch,
        BrightnessKnob,
        ThicknessKnob,
        ChpushButton,
        OhpushButton,
        ClosedHihatTriggerIn,
        PitchCvIn,
        OpenHihatTriggerIn,
        DecayCvIn,
        BrightnessCvIn,
        ThicknessCvIn,
        ClosedHihatAudioOut,
        OpenHihatAudioOut,
    };
};
} // namespace MetaModule
