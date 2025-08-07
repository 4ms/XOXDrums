#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct ClapInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Clap"};
    static constexpr std::string_view description{"Clap DrumModule"};
    static constexpr uint32_t width_hp = 10;
    static constexpr std::string_view svg_filename{"res/Clap.svg"};
    static constexpr std::string_view png_filename{"4ms/Clap.png"};

    using enum Coords;

    static constexpr std::array<Element, 15> Elements{{
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(62.027), Center, "Energy", ""}, 0.25f, 5.0, 15.0, "ms"},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(61.894), Center, "Spread", ""}, 0.25f, 0.0, 100.0, "%"},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(139.494), Center, "Color", ""}, 0.25f, 0.8, 16.0, "khz"},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(139.494), Center, "Verb Decay", ""}, 0.25f, 20.0, 100.0, "ms"},
		Davies1900hBlackKnob{{to_mm<96>(50.467), to_mm<96>(217.095), Center, "Verb Volume", ""}, 0.25f, 0.0, 100.0, "%"},
		Davies1900hBlackKnob{{to_mm<96>(141.867), to_mm<96>(217.095), Center, "Saturation", ""}, 0.25f, 0.0, 100.0, "%"},
		WhiteMomentary7mm{{to_mm<96>(39.674), to_mm<96>(309.468), Center, "Trigger", ""}},
		GateJackInput4ms{{to_mm<96>(39.674), to_mm<96>(362.975), Center, "Trigger In", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.534), to_mm<96>(309.468), Center, "Energy CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.934), to_mm<96>(309.468), Center, "Spread CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(39.674), to_mm<96>(418.456), Center, "Color CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.534), to_mm<96>(362.975), Center, "Verb Decay CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(152.934), to_mm<96>(362.975), Center, "Verb Volume CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(95.153), to_mm<96>(418.456), Center, "Saturation CV In", ""}},
		AnalogJackOutput4ms{{to_mm<96>(153.041), to_mm<96>(418.456), Center, "Audio Out", ""}},
}};

    enum class Elem {
        EnergyKnob,
        SpreadKnob,
        ColorKnob,
        VerbDecayKnob,
        VerbVolumeKnob,
        SaturationKnob,
        TriggerButton,
        TriggerIn,
        EnergyCvIn,
        SpreadCvIn,
        ColorCvIn,
        VerbDecayCvIn,
        VerbVolumeCvIn,
        SaturationCvIn,
        AudioOut,
    };
};
} // namespace MetaModule
