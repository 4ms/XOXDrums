#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct DuckInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Duck"};
    static constexpr std::string_view description{"DuckingModule"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Duck.svg"};
    static constexpr std::string_view png_filename{"4ms/Duck.png"};

    using enum Coords;

    static constexpr std::array<Element, 8> Elements{{
		Knob9mm{{to_mm<96>(38.4), to_mm<96>(62.174), Center, "Amount", ""}, 1.0f, 0.0, 100.0, "%"},
		Knob9mm{{to_mm<96>(38.4), to_mm<96>(151.774), Center, "Time", ""}, 0.25f, 50.0, 2000.0, "ms"},
		WhiteMomentary7mm{{to_mm<96>(38.4), to_mm<96>(238.638), Center, "Trigger", ""}},
		AnalogJackInput4ms{{to_mm<96>(38.4), to_mm<96>(111.595), Center, "Amount CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(38.4), to_mm<96>(203.44), Center, "Time CV In", ""}},
		GateJackInput4ms{{to_mm<96>(38.4), to_mm<96>(277.519), Center, "Trigger In", ""}},
		AnalogJackInput4ms{{to_mm<96>(38.4), to_mm<96>(341.772), Center, "Audio In", ""}},
		AnalogJackOutput4ms{{to_mm<96>(38.4), to_mm<96>(406.853), Center, "Audio Out", ""}},
}};

    enum class Elem {
        AmountKnob,
        TimeKnob,
        TriggerButton,
        AmountCvIn,
        TimeCvIn,
        TriggerIn,
        AudioIn,
        AudioOut,
    };
};
} // namespace MetaModule
