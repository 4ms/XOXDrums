#include "plugin-vcv.hh"
#include "CoreModules/elements/element_counter.hh"
#include "modules/HiHat.hh"
#include "modules/info/HiHat_info.hh"
#include "widgets/vcv_module_creator.hh"
#include "widgets/vcv_widget_creator.hh"

using namespace rack;
using namespace MetaModule;

struct HiHatVcv : rack::Module {
	HiHat cores[PORT_MAX_CHANNELS];

	// Params follow element ordering in HiHatInfo::Elements.
	enum ParamId {
		PITCH_PARAM,
		DECAY_PARAM,
		CHOKE_PARAM,
		BRIGHTNESS_PARAM,
		THICKNESS_PARAM,
		CH_TRIGGER_PARAM,
		OH_TRIGGER_PARAM,
		PARAMS_LEN
	};
	// Inputs
	enum InputId {
		CHH_TRIGGER_INPUT,
		PITCHCV_INPUT,
		OHH_TRIGGER_INPUT,
		DECAYCV_INPUT,
		BRIGHTNESSCV_INPUT,
		THICKNESSCV_INPUT,
		INPUTS_LEN
	};
	// Two outputs: closed and open
	enum OutputId {
		CHH_OUTPUT,
		OHH_OUTPUT,
		OUTPUTS_LEN
	};
	// Two lights: CH button and OH button
	enum LightId {
		CH_TRIGGER_LIGHT,
		OH_TRIGGER_LIGHT,
		LIGHTS_LEN
	};

	HiHatVcv() {
		auto cnt = ElementCount::count<HiHatInfo>();
		config(cnt.num_params, cnt.num_inputs, cnt.num_outputs, cnt.num_lights);

		VCVModuleParamCreator<HiHatInfo> creator{this};
		for (auto &element : HiHatInfo::Elements)
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
			inputs[CHH_TRIGGER_INPUT].getChannels(),
			inputs[OHH_TRIGGER_INPUT].getChannels(),
			inputs[PITCHCV_INPUT].getChannels(),
			inputs[DECAYCV_INPUT].getChannels(),
			inputs[BRIGHTNESSCV_INPUT].getChannels(),
			inputs[THICKNESSCV_INPUT].getChannels(),
		});
		outputs[CHH_OUTPUT].setChannels(n);
		outputs[OHH_OUTPUT].setChannels(n);

		const float pitchNorm      = getParamQuantity(PITCH_PARAM)->toScaled(params[PITCH_PARAM].getValue());
		const float decayNorm      = getParamQuantity(DECAY_PARAM)->toScaled(params[DECAY_PARAM].getValue());
		const float chokeVal       = params[CHOKE_PARAM].getValue();
		const float brightnessNorm = getParamQuantity(BRIGHTNESS_PARAM)->toScaled(params[BRIGHTNESS_PARAM].getValue());
		const float thicknessNorm  = getParamQuantity(THICKNESS_PARAM)->toScaled(params[THICKNESS_PARAM].getValue());
		const float chTrigButton   = params[CH_TRIGGER_PARAM].getValue();
		const float ohTrigButton   = params[OH_TRIGGER_PARAM].getValue();

		const int chhChans   = inputs[CHH_TRIGGER_INPUT].getChannels();
		const int ohhChans   = inputs[OHH_TRIGGER_INPUT].getChannels();
		const int pitchChans = inputs[PITCHCV_INPUT].getChannels();
		const int decayChans = inputs[DECAYCV_INPUT].getChannels();
		const int brightChans = inputs[BRIGHTNESSCV_INPUT].getChannels();
		const int thickChans  = inputs[THICKNESSCV_INPUT].getChannels();

		for (int c = 0; c < n; c++) {
			auto &core = cores[c];

			core.set_param(PITCH_PARAM,      pitchNorm);
			core.set_param(DECAY_PARAM,      decayNorm);
			core.set_param(CHOKE_PARAM,      chokeVal);
			core.set_param(BRIGHTNESS_PARAM, brightnessNorm);
			core.set_param(THICKNESS_PARAM,  thicknessNorm);
			core.set_param(CH_TRIGGER_PARAM, chTrigButton);
			core.set_param(OH_TRIGGER_PARAM, ohTrigButton);

			auto setIn = [&](int id, InputId port, int numChans) {
				if (inputs[port].isConnected())
					core.set_input(id, inputs[port].getPolyVoltage(std::min(c, numChans - 1)));
				else
					core.mark_input_unpatched(id);
			};
			setIn(CHH_TRIGGER_INPUT,  CHH_TRIGGER_INPUT,  chhChans);
			setIn(PITCHCV_INPUT,      PITCHCV_INPUT,      pitchChans);
			setIn(OHH_TRIGGER_INPUT,  OHH_TRIGGER_INPUT,  ohhChans);
			setIn(DECAYCV_INPUT,      DECAYCV_INPUT,      decayChans);
			setIn(BRIGHTNESSCV_INPUT, BRIGHTNESSCV_INPUT, brightChans);
			setIn(THICKNESSCV_INPUT,  THICKNESSCV_INPUT,  thickChans);

			core.mark_output_patched(0);
			core.mark_output_patched(1);
			core.update();
			outputs[CHH_OUTPUT].setVoltage(core.get_output(0), c);
			outputs[OHH_OUTPUT].setVoltage(core.get_output(1), c);
		}

		lights[CH_TRIGGER_LIGHT].setBrightness(cores[0].get_led_brightness(0));
		lights[OH_TRIGGER_LIGHT].setBrightness(cores[0].get_led_brightness(1));
	}
};

struct HiHatWidget : rack::app::ModuleWidget {
	HiHatWidget(HiHatVcv *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, HiHatInfo::svg_filename.data())));

		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(
			rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		VCVWidgetCreator<HiHatInfo> creator(this, module);
		for (auto &element : HiHatInfo::Elements)
			std::visit([&creator](auto &el) { creator.create(el); }, element);
	}
};

rack::Model *modelHiHat = rack::createModel<HiHatVcv, HiHatWidget>("HiHat");
