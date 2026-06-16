#include "plugin-vcv.hh"
#include "CoreModules/elements/element_counter.hh"
#include "modules/Clave.hh"
#include "modules/info/Clave_info.hh"
#include "widgets/vcv_module_creator.hh"
#include "widgets/vcv_widget_creator.hh"

using namespace rack;
using namespace MetaModule;

struct ClaveVcv : rack::Module {
	Clave cores[PORT_MAX_CHANNELS];

	enum ParamId {
		PITCH_PARAM,
		DECAY_PARAM,
		TRIGGER_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		PITCHCV_INPUT,
		DECAYCV_INPUT,
		TRIGGERIN_INPUT,
		INPUTS_LEN
	};
	enum OutputId { CLAVE_OUTPUT, OUTPUTS_LEN };
	enum LightId { TRIGGER_LIGHT, LIGHTS_LEN };

	ClaveVcv() {
		auto cnt = ElementCount::count<ClaveInfo>();
		config(cnt.num_params, cnt.num_inputs, cnt.num_outputs, cnt.num_lights);

		VCVModuleParamCreator<ClaveInfo> creator{this};
		for (auto &element : ClaveInfo::Elements)
			std::visit([&creator](auto &el) { creator.config_element(el); }, element);

		for (auto &core : cores) {
			core.mark_all_inputs_unpatched();
			core.mark_all_outputs_unpatched();
		}
	}

	void onSampleRateChange(const SampleRateChangeEvent &e) override {
		for (auto &core : cores)
			core.set_samplerate(e.sampleRate);
	}

	void process(const ProcessArgs &args) override {
		const int n = std::max({1,
			inputs[TRIGGERIN_INPUT].getChannels(),
			inputs[PITCHCV_INPUT].getChannels(),
			inputs[DECAYCV_INPUT].getChannels(),
		});
		outputs[CLAVE_OUTPUT].setChannels(n);

		const float pitchNorm   = getParamQuantity(PITCH_PARAM)->toScaled(params[PITCH_PARAM].getValue());
		const float decayNorm   = getParamQuantity(DECAY_PARAM)->toScaled(params[DECAY_PARAM].getValue());
		const float trigButton  = params[TRIGGER_PARAM].getValue();

		const int trigChans   = inputs[TRIGGERIN_INPUT].getChannels();
		const int pitchChans  = inputs[PITCHCV_INPUT].getChannels();
		const int decayChans  = inputs[DECAYCV_INPUT].getChannels();

		for (int c = 0; c < n; c++) {
			auto &core = cores[c];

			core.set_param(PITCH_PARAM,   pitchNorm);
			core.set_param(DECAY_PARAM,   decayNorm);
			core.set_param(TRIGGER_PARAM, trigButton);

			auto setIn = [&](int id, InputId port, int numChans) {
				if (inputs[port].isConnected())
					core.set_input(id, inputs[port].getPolyVoltage(std::min(c, numChans - 1)));
				else
					core.mark_input_unpatched(id);
			};
			setIn(PITCHCV_INPUT,   PITCHCV_INPUT,   pitchChans);
			setIn(DECAYCV_INPUT,   DECAYCV_INPUT,   decayChans);
			setIn(TRIGGERIN_INPUT, TRIGGERIN_INPUT,  trigChans);

			core.mark_output_patched(0);
			core.update();
			outputs[CLAVE_OUTPUT].setVoltage(core.get_output(0), c);
		}

		lights[TRIGGER_LIGHT].setBrightness(cores[0].get_led_brightness(0));
	}
};

struct ClaveWidget : rack::app::ModuleWidget {
	ClaveWidget(ClaveVcv *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, ClaveInfo::svg_filename.data())));

		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		VCVWidgetCreator<ClaveInfo> creator(this, module);
		for (auto &element : ClaveInfo::Elements)
			std::visit([&creator](auto &el) { creator.create(el); }, element);
	}
};

rack::Model *modelClave = rack::createModel<ClaveVcv, ClaveWidget>("Clave");
