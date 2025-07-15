#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct ClaveInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Clave"};
    static constexpr std::string_view description{"Clave DrumModule"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Clave.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Clave.png"};

    using enum Coords;

    static constexpr std::array<Element, 7> Elements{{
		WhiteMomentary7mm{{to_mm<96>(38.56), to_mm<96>(299.037), Center, "pushButton", ""}},
		Knob9mm{{to_mm<96>(38.56), to_mm<96>(70.115), Center, "Pitch", ""}, 0.5f, 1.0, 2.0, "khz"},
		Knob9mm{{to_mm<96>(38.56), to_mm<96>(198.622), Center, "Decay", ""}, 0.5f, 5.0, 25.0, "ms"},
		AnalogJackInput4ms{{to_mm<96>(38.56), to_mm<96>(134.369), Center, "Pitch CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(38.56), to_mm<96>(262.876), Center, "Decay CV In", ""}},
		GateJackInput4ms{{to_mm<96>(38.56), to_mm<96>(341.757), Center, "Trigger In", ""}},
		AnalogJackOutput4ms{{to_mm<96>(38.56), to_mm<96>(406.824), Center, "Clave Out", ""}},
}};

    enum class Elem {
        PushButton,
        PitchKnob,
        DecayKnob,
        PitchCvIn,
        DecayCvIn,
        TriggerIn,
        ClaveOut,
    };
};
} // namespace MetaModule
