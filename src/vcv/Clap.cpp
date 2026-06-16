#include "plugin-vcv.hh"
#include "CoreModules/elements/element_counter.hh"
#include "modules/Clap.hh"
#include "modules/info/Clap_info.hh"
#include "widgets/vcv_module_creator.hh"
#include "widgets/vcv_widget_creator.hh"

using namespace rack;
using namespace MetaModule;

// VCV-level struct uses ClapVcv to avoid collision with the MetaModule Clap core.
struct ClapVcv : rack::Module {
	Clap cores[PORT_MAX_CHANNELS];

	enum ParamId {
		ENERGY_PARAM,
		SPREAD_PARAM,
		COLOR_PARAM,
		VERBDECAY_PARAM,
		VERBVOLUME_PARAM,
		SATURATION_PARAM,
		TRIGGER_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		TRIGGERIN_INPUT,
		ENERGYCV_INPUT,
		SPREADCV_INPUT,
		COLORCV_INPUT,
		VERBDECAYCV_INPUT,
		VERBVOLUMECV_INPUT,
		SATURATIONCV_INPUT,
		INPUTS_LEN
	};
	enum OutputId { AUDIO_OUTPUT, OUTPUTS_LEN };
	enum LightId { TRIGGER_LIGHT, LIGHTS_LEN };

	ClapVcv() {
		auto cnt = ElementCount::count<ClapInfo>();
		config(cnt.num_params, cnt.num_inputs, cnt.num_outputs, cnt.num_lights);

		VCVModuleParamCreator<ClapInfo> creator{this};
		for (auto &element : ClapInfo::Elements)
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
		// Channel count = max across every connected input.
		// A mono cable on any port broadcasts to all voices.
		const int n = std::max({1,
			inputs[TRIGGERIN_INPUT].getChannels(),
			inputs[ENERGYCV_INPUT].getChannels(),
			inputs[SPREADCV_INPUT].getChannels(),
			inputs[COLORCV_INPUT].getChannels(),
			inputs[VERBDECAYCV_INPUT].getChannels(),
			inputs[VERBVOLUMECV_INPUT].getChannels(),
			inputs[SATURATIONCV_INPUT].getChannels(),
		});
		outputs[AUDIO_OUTPUT].setChannels(n);

		const float energyNorm     = getParamQuantity(ENERGY_PARAM)->toScaled(params[ENERGY_PARAM].getValue());
		const float spreadNorm     = getParamQuantity(SPREAD_PARAM)->toScaled(params[SPREAD_PARAM].getValue());
		const float colorNorm      = getParamQuantity(COLOR_PARAM)->toScaled(params[COLOR_PARAM].getValue());
		const float verbDecayNorm  = getParamQuantity(VERBDECAY_PARAM)->toScaled(params[VERBDECAY_PARAM].getValue());
		const float verbVolumeNorm = getParamQuantity(VERBVOLUME_PARAM)->toScaled(params[VERBVOLUME_PARAM].getValue());
		const float satNorm        = getParamQuantity(SATURATION_PARAM)->toScaled(params[SATURATION_PARAM].getValue());
		const float triggerButton  = params[TRIGGER_PARAM].getValue();

		const int trigChans = inputs[TRIGGERIN_INPUT].getChannels();
		const int enChans   = inputs[ENERGYCV_INPUT].getChannels();
		const int spChans   = inputs[SPREADCV_INPUT].getChannels();
		const int colChans  = inputs[COLORCV_INPUT].getChannels();
		const int vdChans   = inputs[VERBDECAYCV_INPUT].getChannels();
		const int vvChans   = inputs[VERBVOLUMECV_INPUT].getChannels();
		const int satChans  = inputs[SATURATIONCV_INPUT].getChannels();

		for (int c = 0; c < n; c++) {
			auto &core = cores[c];

			core.set_param(ENERGY_PARAM,     energyNorm);
			core.set_param(SPREAD_PARAM,     spreadNorm);
			core.set_param(COLOR_PARAM,      colorNorm);
			core.set_param(VERBDECAY_PARAM,  verbDecayNorm);
			core.set_param(VERBVOLUME_PARAM, verbVolumeNorm);
			core.set_param(SATURATION_PARAM, satNorm);
			core.set_param(TRIGGER_PARAM,    triggerButton);

			// Every input: mono cable broadcasts to all voices; poly routes per-channel.
			auto setIn = [&](int id, InputId port, int numChans) {
				if (inputs[port].isConnected())
					core.set_input(id, inputs[port].getPolyVoltage(std::min(c, numChans - 1)));
				else
					core.mark_input_unpatched(id);
			};
			setIn(TRIGGERIN_INPUT,    TRIGGERIN_INPUT,    trigChans);
			setIn(ENERGYCV_INPUT,     ENERGYCV_INPUT,     enChans);
			setIn(SPREADCV_INPUT,     SPREADCV_INPUT,     spChans);
			setIn(COLORCV_INPUT,      COLORCV_INPUT,      colChans);
			setIn(VERBDECAYCV_INPUT,  VERBDECAYCV_INPUT,  vdChans);
			setIn(VERBVOLUMECV_INPUT, VERBVOLUMECV_INPUT, vvChans);
			setIn(SATURATIONCV_INPUT, SATURATIONCV_INPUT, satChans);

			core.mark_output_patched(0);
			core.update();
			outputs[AUDIO_OUTPUT].setVoltage(core.get_output(0), c);
		}

		lights[TRIGGER_LIGHT].setBrightness(cores[0].get_led_brightness(0));
	}
};

struct ClapWidget : rack::app::ModuleWidget {
	ClapWidget(ClapVcv *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, ClapInfo::svg_filename.data())));

		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(
			rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		VCVWidgetCreator<ClapInfo> creator(this, module);
		for (auto &element : ClapInfo::Elements)
			std::visit([&creator](auto &el) { creator.create(el); }, element);
	}
};

rack::Model *modelClap = rack::createModel<ClapVcv, ClapWidget>("Clap");
