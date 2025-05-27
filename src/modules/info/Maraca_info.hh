#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct MaracaInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Maraca"};
    static constexpr std::string_view description{"Maraca DrumModule"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Maraca.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Maraca.png"};

    using enum Coords;

    static constexpr std::array<Element, 4> Elements{{
		Knob9mm{{to_mm<96>(38.56), to_mm<96>(62.36), Center, "Decay", ""}, 0.5f, 2.0, 20.0, "ms"},
		AnalogJackInput4ms{{to_mm<96>(38.56), to_mm<96>(126.614), Center, "Decay CV In", ""}},
		GateJackInput4ms{{to_mm<96>(38.4), to_mm<96>(341.8), Center, "Trigger In", ""}},
		AnalogJackOutput4ms{{to_mm<96>(38.4), to_mm<96>(406.867), Center, "Audio Out", ""}},
}};

    enum class Elem {
        DecayKnob,
        DecayCvIn,
        TriggerIn,
        AudioOut,
    };
};
} // namespace MetaModule
