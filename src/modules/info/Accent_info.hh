#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct AccentInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Accent"};
    static constexpr std::string_view description{"DrumAccentModule"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Accent.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Accent.png"};

    using enum Coords;

    static constexpr std::array<Element, 5> Elements{{
		Knob9mm{{to_mm<96>(38.4), to_mm<96>(73.373), Center, "Amount", ""}, 0.0f},
		AnalogJackInput4ms{{to_mm<96>(38.4), to_mm<96>(137.626), Center, "Amount CV", ""}},
		GateJackInput4ms{{to_mm<96>(38.24), to_mm<96>(277.337), Center, "Trigger", ""}},
		AnalogJackInput4ms{{to_mm<96>(38.24), to_mm<96>(341.577), Center, "Input", ""}},
		AnalogJackOutput4ms{{to_mm<96>(38.24), to_mm<96>(406.644), Center, "Out", ""}},
}};

    enum class Elem {
        AmountKnob,
        AmountCvIn,
        TriggerIn,
        InputIn,
        Out,
    };
};
} // namespace MetaModule
