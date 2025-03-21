#pragma once
#include "helpers/4ms_elements.hh"
#include "CoreModules/elements/element_info.hh"

#include <array>

namespace MetaModule
{
struct ClaveInfo : ModuleInfoBase {
    static constexpr std::string_view slug{"Clave"};
    static constexpr std::string_view description{"Clave"};
    static constexpr uint32_t width_hp = 4;
    static constexpr std::string_view svg_filename{"res/Clave.svg"};
    static constexpr std::string_view png_filename{"4msDrums/Clave.png"};

    using enum Coords;

    static constexpr std::array<Element, 6> Elements{{
		Knob9mm{{to_mm<25>(9.912), to_mm<25>(45.41), Center, "Decay", ""}, 0.5f},
		Knob9mm{{to_mm<25>(9.912), to_mm<25>(19.535), Center, "Pitch", ""}, 0.5f},
		AnalogJackInput4ms{{to_mm<25>(9.912), to_mm<25>(93.021), Center, "Trig", ""}},
		AnalogJackOutput4ms{{to_mm<25>(9.912), to_mm<25>(110.168), Center, "Clave", ""}},
        AnalogJackInput4ms{{to_mm<25>(9.912), to_mm<25>(33.947), Center, "Pitch CV", ""}},
        AnalogJackInput4ms{{to_mm<25>(9.912), to_mm<25>(60.097), Center, "Decay CV", ""}},
}};

    enum class Elem {
        DecayKnob,
        PitchKnob,
        TrigIn,
        ClaveOut,
        PitchCvIn,
        DecayCvIn,
    };
};
} // namespace MetaModule
