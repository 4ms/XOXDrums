#include "plugin-vcv.hh"
#include "CoreModules/elements/element_counter.hh"
#include "modules/Accent.hh"
#include "modules/info/Accent_info.hh"
#include "widgets/vcv_module_creator.hh"
#include "widgets/vcv_widget_creator.hh"

using namespace rack;
using namespace MetaModule;

struct AccentVcv : rack::Module {
	Accent cores[PORT_MAX_CHANNELS];

	enum ParamId {
		AMOUNT_PARAM,
		TRIGGER_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		AMOUNTCV_INPUT,
		TRIGGERIN_INPUT,
		AUDIOIN_INPUT,
		INPUTS_LEN
	};
	enum OutputId { ACCENTED_OUTPUT, OUTPUTS_LEN };
	enum LightId { TRIGGER_LIGHT, LIGHTS_LEN };

	AccentVcv() {
		auto cnt = ElementCount::count<AccentInfo>();
		config(cnt.num_params, cnt.num_inputs, cnt.num_outputs, cnt.num_lights);

		VCVModuleParamCreator<AccentInfo> creator{this};
		for (auto &element : AccentInfo::Elements)
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
			inputs[AMOUNTCV_INPUT].getChannels(),
			inputs[AUDIOIN_INPUT].getChannels(),
		});
		outputs[ACCENTED_OUTPUT].setChannels(n);

		const float amountNorm = getParamQuantity(AMOUNT_PARAM)->toScaled(params[AMOUNT_PARAM].getValue());
		const float trigButton = params[TRIGGER_PARAM].getValue();

		const int trigChans   = inputs[TRIGGERIN_INPUT].getChannels();
		const int amountChans = inputs[AMOUNTCV_INPUT].getChannels();
		const int audioChans  = inputs[AUDIOIN_INPUT].getChannels();

		for (int c = 0; c < n; c++) {
			auto &core = cores[c];

			core.set_param(AMOUNT_PARAM,  amountNorm);
			core.set_param(TRIGGER_PARAM, trigButton);

			auto setIn = [&](int id, InputId port, int numChans) {
				if (inputs[port].isConnected()) {
					core.mark_input_patched(id);
					core.set_input(id, inputs[port].getPolyVoltage(std::min(c, numChans - 1)));
				} else {
					core.mark_input_unpatched(id);
				}
			};
			setIn(AMOUNTCV_INPUT,  AMOUNTCV_INPUT,  amountChans);
			setIn(TRIGGERIN_INPUT, TRIGGERIN_INPUT,  trigChans);
			setIn(AUDIOIN_INPUT,   AUDIOIN_INPUT,    audioChans);

			core.mark_output_patched(0);
			core.update();
			outputs[ACCENTED_OUTPUT].setVoltage(core.get_output(0), c);
		}

		lights[TRIGGER_LIGHT].setBrightness(cores[0].get_led_brightness(0));
	}
};

struct AccentWidget : rack::app::ModuleWidget {
	AccentWidget(AccentVcv *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, AccentInfo::svg_filename.data())));

		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		VCVWidgetCreator<AccentInfo> creator(this, module);
		for (auto &element : AccentInfo::Elements)
			std::visit([&creator](auto &el) { creator.create(el); }, element);
	}
};

rack::Model *modelAccent = rack::createModel<AccentVcv, AccentWidget>("Accent");
