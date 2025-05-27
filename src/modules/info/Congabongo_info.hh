#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct CongabongoInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Congabongo"};
    static constexpr std::string_view description{"Conga Bongo DrumModule"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/Congabongo.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Congabongo.png"};

    using enum Coords;

    static constexpr std::array<Element, 11> Elements{{
		Davies1900hBlackKnob{{to_mm<96>(96.147), to_mm<96>(77.134), Center, "Pitch", ""}, 0.0f, 0.0, 100.0, "%"},
		Davies1900hBlackKnob{{to_mm<96>(96.147), to_mm<96>(158.788), Center, "Decay", ""}, 0.5f, 0.0, 100.0, "%"},
		Toggle2posHoriz{{to_mm<96>(96.206), to_mm<96>(231.104), Center, "Mode Switch", ""}, {"Conga", "Bongo"}},
		GateJackInput4ms{{to_mm<96>(39.347), to_mm<96>(309.602), Center, "Slap LO Trigger In", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.601), to_mm<96>(309.468), Center, "Pitch CV In", ""}},
		GateJackInput4ms{{to_mm<96>(152.987), to_mm<96>(309.468), Center, "Slap HI Trigger In", ""}},
		AnalogJackInput4ms{{to_mm<96>(39.347), to_mm<96>(362.975), Center, "Tone LO Trigger In", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.601), to_mm<96>(362.975), Center, "Decay CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.987), to_mm<96>(362.975), Center, "Tone HI Trigger In", ""}},
		AnalogJackOutput4ms{{to_mm<96>(39.347), to_mm<96>(418.456), Center, "LO Audio Out", ""}},
		AnalogJackOutput4ms{{to_mm<96>(152.987), to_mm<96>(418.456), Center, "HI Audio Out", ""}},
}};

    enum class Elem {
        PitchKnob,
        DecayKnob,
        ModeSwitch,
        SlapLoTriggerIn,
        PitchCvIn,
        SlapHiTriggerIn,
        ToneLoTriggerIn,
        DecayCvIn,
        ToneHiTriggerIn,
        LoAudioOut,
        HiAudioOut,
    };
};
} // namespace MetaModule
