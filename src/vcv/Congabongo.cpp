#include "plugin-vcv.hh"
#include "CoreModules/elements/element_counter.hh"
#include "modules/Congabongo.hh"
#include "modules/info/Congabongo_info.hh"
#include "widgets/vcv_module_creator.hh"
#include "widgets/vcv_widget_creator.hh"

using namespace rack;
using namespace MetaModule;

struct CongabongoVcv : rack::Module {
	Congabongo cores[PORT_MAX_CHANNELS];

	enum ParamId {
		PITCH_PARAM,
		DECAY_PARAM,
		MODE_PARAM,
		TONEHI_PARAM,
		SLAPHI_PARAM,
		TONELO_PARAM,
		SLAPLO_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		SLAPLO_INPUT,
		PITCHCV_INPUT,
		SLAPHI_INPUT,
		TONELO_INPUT,
		DECAYCV_INPUT,
		TONEHI_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		LO_OUTPUT,
		HI_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		TONEHI_LIGHT,
		SLAPHI_LIGHT,
		TONELO_LIGHT,
		SLAPLO_LIGHT,
		LIGHTS_LEN
	};

	CongabongoVcv() {
		auto cnt = ElementCount::count<CongabongoInfo>();
		config(cnt.num_params, cnt.num_inputs, cnt.num_outputs, cnt.num_lights);

		VCVModuleParamCreator<CongabongoInfo> creator{this};
		for (auto &element : CongabongoInfo::Elements)
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
			inputs[SLAPLO_INPUT].getChannels(),
			inputs[PITCHCV_INPUT].getChannels(),
			inputs[SLAPHI_INPUT].getChannels(),
			inputs[TONELO_INPUT].getChannels(),
			inputs[DECAYCV_INPUT].getChannels(),
			inputs[TONEHI_INPUT].getChannels(),
		});
		outputs[LO_OUTPUT].setChannels(n);
		outputs[HI_OUTPUT].setChannels(n);

		const float pitchNorm  = getParamQuantity(PITCH_PARAM)->toScaled(params[PITCH_PARAM].getValue());
		const float decayNorm  = getParamQuantity(DECAY_PARAM)->toScaled(params[DECAY_PARAM].getValue());
		const float modeVal    = params[MODE_PARAM].getValue();
		const float toneHiBtn  = params[TONEHI_PARAM].getValue();
		const float slapHiBtn  = params[SLAPHI_PARAM].getValue();
		const float toneLoBtn  = params[TONELO_PARAM].getValue();
		const float slapLoBtn  = params[SLAPLO_PARAM].getValue();

		const int slapLoChans  = inputs[SLAPLO_INPUT].getChannels();
		const int pitchChans   = inputs[PITCHCV_INPUT].getChannels();
		const int slapHiChans  = inputs[SLAPHI_INPUT].getChannels();
		const int toneLoChans  = inputs[TONELO_INPUT].getChannels();
		const int decayChans   = inputs[DECAYCV_INPUT].getChannels();
		const int toneHiChans  = inputs[TONEHI_INPUT].getChannels();

		for (int c = 0; c < n; c++) {
			auto &core = cores[c];

			core.set_param(PITCH_PARAM,  pitchNorm);
			core.set_param(DECAY_PARAM,  decayNorm);
			core.set_param(MODE_PARAM,   modeVal);
			core.set_param(TONEHI_PARAM, toneHiBtn);
			core.set_param(SLAPHI_PARAM, slapHiBtn);
			core.set_param(TONELO_PARAM, toneLoBtn);
			core.set_param(SLAPLO_PARAM, slapLoBtn);

			auto setIn = [&](int id, InputId port, int numChans) {
				if (inputs[port].isConnected())
					core.set_input(id, inputs[port].getPolyVoltage(std::min(c, numChans - 1)));
				else
					core.mark_input_unpatched(id);
			};
			setIn(SLAPLO_INPUT,  SLAPLO_INPUT,  slapLoChans);
			setIn(PITCHCV_INPUT, PITCHCV_INPUT, pitchChans);
			setIn(SLAPHI_INPUT,  SLAPHI_INPUT,  slapHiChans);
			setIn(TONELO_INPUT,  TONELO_INPUT,  toneLoChans);
			setIn(DECAYCV_INPUT, DECAYCV_INPUT, decayChans);
			setIn(TONEHI_INPUT,  TONEHI_INPUT,  toneHiChans);

			core.mark_output_patched(0);
			core.mark_output_patched(1);
			core.update();
			outputs[LO_OUTPUT].setVoltage(core.get_output(0), c);
			outputs[HI_OUTPUT].setVoltage(core.get_output(1), c);
		}

		lights[TONEHI_LIGHT].setBrightness(cores[0].get_led_brightness(0));
		lights[SLAPHI_LIGHT].setBrightness(cores[0].get_led_brightness(1));
		lights[TONELO_LIGHT].setBrightness(cores[0].get_led_brightness(2));
		lights[SLAPLO_LIGHT].setBrightness(cores[0].get_led_brightness(3));
	}
};

struct CongabongoWidget : rack::app::ModuleWidget {
	CongabongoWidget(CongabongoVcv *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, CongabongoInfo::svg_filename.data())));

		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(
			rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		VCVWidgetCreator<CongabongoInfo> creator(this, module);
		for (auto &element : CongabongoInfo::Elements)
			std::visit([&creator](auto &el) { creator.create(el); }, element);
	}
};

rack::Model *modelCongabongo = rack::createModel<CongabongoVcv, CongabongoWidget>("Congabongo");
