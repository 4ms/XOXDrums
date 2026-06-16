#include "plugin-vcv.hh"
#include "CoreModules/elements/element_counter.hh"
#include "modules/Tom.hh"
#include "modules/info/Tom_info.hh"
#include "widgets/vcv_module_creator.hh"
#include "widgets/vcv_widget_creator.hh"

using namespace rack;
using namespace MetaModule;

struct TomVcv : rack::Module {
	Tom cores[PORT_MAX_CHANNELS];

	enum ParamId {
		PITCH_PARAM,
		AMPDECAY_PARAM,
		PITCHRANGE_PARAM,
		PITCHDECAY_PARAM,
		ENVDEPTH_PARAM,
		TRIGGER_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		TRIGGERIN_INPUT,
		PITCHCV_INPUT,
		AMPDECAYCV_INPUT,
		PITCHDECAYCV_INPUT,
		ENVDEPTHCV_INPUT,
		INPUTS_LEN
	};
	enum OutputId { AUDIO_OUTPUT, OUTPUTS_LEN };
	enum LightId { TRIGGER_LIGHT, LIGHTS_LEN };

	TomVcv() {
		auto cnt = ElementCount::count<TomInfo>();
		config(cnt.num_params, cnt.num_inputs, cnt.num_outputs, cnt.num_lights);

		VCVModuleParamCreator<TomInfo> creator{this};
		for (auto &element : TomInfo::Elements)
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
			inputs[AMPDECAYCV_INPUT].getChannels(),
			inputs[PITCHDECAYCV_INPUT].getChannels(),
			inputs[ENVDEPTHCV_INPUT].getChannels(),
		});
		outputs[AUDIO_OUTPUT].setChannels(n);

		const float pitchNorm      = getParamQuantity(PITCH_PARAM)->toScaled(params[PITCH_PARAM].getValue());
		const float ampDecayNorm   = getParamQuantity(AMPDECAY_PARAM)->toScaled(params[AMPDECAY_PARAM].getValue());
		const float pitchRangeNorm = getParamQuantity(PITCHRANGE_PARAM)->toScaled(params[PITCHRANGE_PARAM].getValue());
		const float pitchDecayNorm = getParamQuantity(PITCHDECAY_PARAM)->toScaled(params[PITCHDECAY_PARAM].getValue());
		const float envDepthNorm   = getParamQuantity(ENVDEPTH_PARAM)->toScaled(params[ENVDEPTH_PARAM].getValue());
		const float trigButton     = params[TRIGGER_PARAM].getValue();

		const int trigChans      = inputs[TRIGGERIN_INPUT].getChannels();
		const int pitchChans     = inputs[PITCHCV_INPUT].getChannels();
		const int ampDecChans    = inputs[AMPDECAYCV_INPUT].getChannels();
		const int pitchDecChans  = inputs[PITCHDECAYCV_INPUT].getChannels();
		const int envDepthChans  = inputs[ENVDEPTHCV_INPUT].getChannels();

		for (int c = 0; c < n; c++) {
			auto &core = cores[c];

			core.set_param(PITCH_PARAM,      pitchNorm);
			core.set_param(AMPDECAY_PARAM,   ampDecayNorm);
			core.set_param(PITCHRANGE_PARAM, pitchRangeNorm);
			core.set_param(PITCHDECAY_PARAM, pitchDecayNorm);
			core.set_param(ENVDEPTH_PARAM,   envDepthNorm);
			core.set_param(TRIGGER_PARAM,    trigButton);

			auto setIn = [&](int id, InputId port, int numChans) {
				if (inputs[port].isConnected())
					core.set_input(id, inputs[port].getPolyVoltage(std::min(c, numChans - 1)));
				else
					core.mark_input_unpatched(id);
			};
			setIn(TRIGGERIN_INPUT,    TRIGGERIN_INPUT,    trigChans);
			setIn(PITCHCV_INPUT,      PITCHCV_INPUT,      pitchChans);
			setIn(AMPDECAYCV_INPUT,   AMPDECAYCV_INPUT,   ampDecChans);
			setIn(PITCHDECAYCV_INPUT, PITCHDECAYCV_INPUT, pitchDecChans);
			setIn(ENVDEPTHCV_INPUT,   ENVDEPTHCV_INPUT,   envDepthChans);

			core.mark_output_patched(0);
			core.update();
			outputs[AUDIO_OUTPUT].setVoltage(core.get_output(0), c);
		}

		lights[TRIGGER_LIGHT].setBrightness(cores[0].get_led_brightness(0));
	}
};

struct TomWidget : rack::app::ModuleWidget {
	TomWidget(TomVcv *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, TomInfo::svg_filename.data())));

		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(
			rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		VCVWidgetCreator<TomInfo> creator(this, module);
		for (auto &element : TomInfo::Elements)
			std::visit([&creator](auto &el) { creator.create(el); }, element);
	}
};

rack::Model *modelTom = rack::createModel<TomVcv, TomWidget>("Tom");
