#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct CowbellInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Cowbell"};
    static constexpr std::string_view description{"Cowbell DrumModule"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Cowbell.svg"};
    static constexpr std::string_view png_filename{"4ms-XOXDrums/Cowbell.png"};

    using enum Coords;

    static constexpr std::array<Element, 7> Elements{{
		WhiteMomentary7mm{{to_mm<96>(38.574), to_mm<96>(297.949), Center, "Trigger", ""}},
		Knob9mm{{to_mm<96>(38.56), to_mm<96>(70.114), Center, "Pitch", ""}, 0.5f, 240.0, 540.0, "hz"},
		Knob9mm{{to_mm<96>(38.56), to_mm<96>(198.622), Center, "Decay", ""}, 0.5f, 20.0, 120.0, "ms"},
		AnalogJackInput4ms{{to_mm<96>(38.56), to_mm<96>(134.368), Center, "Pitch CV In", ""}},
		AnalogJackInput4ms{{to_mm<96>(38.574), to_mm<96>(262.875), Center, "Decay CV In", ""}},
		GateJackInput4ms{{to_mm<96>(38.4), to_mm<96>(341.765), Center, "Trigger In", ""}},
		AnalogJackOutput4ms{{to_mm<96>(38.4), to_mm<96>(406.832), Center, "Audio Out", ""}},
}};

    enum class Elem {
        TriggerButton,
        PitchKnob,
        DecayKnob,
        PitchCvIn,
        DecayCvIn,
        TriggerIn,
        AudioOut,
    };
};
} // namespace MetaModule
