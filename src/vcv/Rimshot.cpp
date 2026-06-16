#include "plugin-vcv.hh"
#include "CoreModules/elements/element_counter.hh"
#include "modules/Rimshot.hh"
#include "modules/info/Rimshot_info.hh"
#include "widgets/vcv_module_creator.hh"
#include "widgets/vcv_widget_creator.hh"

using namespace rack;
using namespace MetaModule;

struct RimshotVcv : rack::Module {
	Rimshot cores[PORT_MAX_CHANNELS];

	enum ParamId {
		PITCH_PARAM,
		RING_PARAM,
		TRIGGER_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		PITCHCV_INPUT,
		RINGCV_INPUT,
		TRIGGERIN_INPUT,
		INPUTS_LEN
	};
	enum OutputId { AUDIO_OUTPUT, OUTPUTS_LEN };
	enum LightId { TRIGGER_LIGHT, LIGHTS_LEN };

	RimshotVcv() {
		auto cnt = ElementCount::count<RimshotInfo>();
		config(cnt.num_params, cnt.num_inputs, cnt.num_outputs, cnt.num_lights);

		VCVModuleParamCreator<RimshotInfo> creator{this};
		for (auto &element : RimshotInfo::Elements)
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
			inputs[RINGCV_INPUT].getChannels(),
		});
		outputs[AUDIO_OUTPUT].setChannels(n);

		const float pitchNorm  = getParamQuantity(PITCH_PARAM)->toScaled(params[PITCH_PARAM].getValue());
		const float ringNorm   = getParamQuantity(RING_PARAM)->toScaled(params[RING_PARAM].getValue());
		const float trigButton = params[TRIGGER_PARAM].getValue();

		const int trigChans  = inputs[TRIGGERIN_INPUT].getChannels();
		const int pitchChans = inputs[PITCHCV_INPUT].getChannels();
		const int ringChans  = inputs[RINGCV_INPUT].getChannels();

		for (int c = 0; c < n; c++) {
			auto &core = cores[c];

			core.set_param(PITCH_PARAM,   pitchNorm);
			core.set_param(RING_PARAM,    ringNorm);
			core.set_param(TRIGGER_PARAM, trigButton);

			auto setIn = [&](int id, InputId port, int numChans) {
				if (inputs[port].isConnected())
					core.set_input(id, inputs[port].getPolyVoltage(std::min(c, numChans - 1)));
				else
					core.mark_input_unpatched(id);
			};
			setIn(PITCHCV_INPUT,   PITCHCV_INPUT,   pitchChans);
			setIn(RINGCV_INPUT,    RINGCV_INPUT,     ringChans);
			setIn(TRIGGERIN_INPUT, TRIGGERIN_INPUT,  trigChans);

			core.mark_output_patched(0);
			core.update();
			outputs[AUDIO_OUTPUT].setVoltage(core.get_output(0), c);
		}

		lights[TRIGGER_LIGHT].setBrightness(cores[0].get_led_brightness(0));
	}
};

struct RimshotWidget : rack::app::ModuleWidget {
	RimshotWidget(RimshotVcv *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, RimshotInfo::svg_filename.data())));

		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		VCVWidgetCreator<RimshotInfo> creator(this, module);
		for (auto &element : RimshotInfo::Elements)
			std::visit([&creator](auto &el) { creator.create(el); }, element);
	}
};

rack::Model *modelRimshot = rack::createModel<RimshotVcv, RimshotWidget>("Rimshot");
