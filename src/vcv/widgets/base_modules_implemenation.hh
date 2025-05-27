#include "CoreModules/elements/element_counter.hh"
#include "CoreModules/elements/elements.hh"
#include "vcv_creation_context.hh"

namespace MetaModule::VCVImplementation::Module
{
using Indices = ElementCount::Indices;

inline void do_config_element(JackInput el, const Indices &indices, const ModuleContext_t &context) {
	context.module->configInput(indices.input_idx, el.short_name.data());
};

inline void do_config_element(JackOutput el, const Indices &indices, const ModuleContext_t &context) {
	context.module->configOutput(indices.output_idx, el.short_name.data());
};

inline void do_config_element(Pot el, const Indices &indices, const ModuleContext_t &context) {
	float display_mult = el.display_mult;
	std::string units{el.units};

	if (el.min_value == 0 && el.max_value == 1 && el.units == "" && el.display_mult == 1) {
		display_mult = 100;
		units = "%";
	}

	context.module->configParam(indices.param_idx,
								el.min_value,
								el.max_value,
								el.default_value,
								std::string{el.short_name},
								units,
								el.display_base,
								display_mult,
								el.display_offset);
};

inline void do_config_element(LightElement el, const Indices &indices, const ModuleContext_t &context) {
	context.module->configLight(indices.light_idx, el.short_name.data());
};

inline void do_config_element(MomentaryButton el, const Indices &indices, const ModuleContext_t &context) {
	context.module->configParam(indices.param_idx, 0, 1, 0, el.short_name.data());
}

inline void do_config_element(LatchingButton el, const Indices &indices, const ModuleContext_t &context) {
	float defaultValue = (el.default_value == LatchingButton::State_t::UP) ? 0 : 1;
	context.module->configParam(indices.param_idx, 0, 1, defaultValue, el.short_name.data());
}

inline void do_config_element(SlideSwitch el, const Indices &indices, const ModuleContext_t &context) {
	context.module->configParam(indices.param_idx, 0, el.num_pos - 1, el.default_value, el.short_name.data());
};

inline void do_config_element(FlipSwitch el, const Indices &indices, const ModuleContext_t &context) {
	std::vector<std::string> labels;
	for (auto p : el.pos_names)
		labels.push_back(std::string{p});
	context.module->configSwitch(indices.param_idx, 0, el.num_pos - 1, el.default_value, el.short_name.data(), labels);
};

inline void do_config_element(Encoder el, const Indices &indices, const ModuleContext_t &context) {
	context.module->configParam(indices.param_idx, -INFINITY, INFINITY, 0, el.short_name.data());
};

inline void do_config_element(AltParamContinuous el, const Indices &indices, const ModuleContext_t &context) {
	context.module->configParam(indices.param_idx, el.MinValue, el.MaxValue, el.default_value, el.short_name.data());
}

inline void do_config_element(AltParamChoice el, const Indices &indices, const ModuleContext_t &context) {
	context.module->configParam(indices.param_idx, 0, el.num_pos - 1, el.default_value, el.short_name.data());
}

} // namespace MetaModule::VCVImplementation::Module
