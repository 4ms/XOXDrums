#include "plugin-vcv.hh"
#include "CoreModules/elements/element_counter.hh"
#include "modules/Kick.hh"
#include "modules/info/Kick_info.hh"
#include "widgets/vcv_module_creator.hh"
#include "widgets/vcv_widget_creator.hh"

using namespace rack;
using namespace MetaModule;

struct KickVcv : rack::Module {
	Kick cores[PORT_MAX_CHANNELS];

	enum ParamId {
		PITCH_PARAM,
		PITCHDEPTH_PARAM,
		PITCHDECAY_PARAM,
		AMPDECAY_PARAM,
		SATURATION_PARAM,
		RANGE_PARAM,
		TRIGGER_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		TRIGGERIN_INPUT,
		PITCHCV_INPUT,
		DEPTHCV_INPUT,
		PITCHDECAYCV_INPUT,
		AMPDECAYCV_INPUT,
		SATURATIONCV_INPUT,
		INPUTS_LEN
	};
	enum OutputId { AUDIO_OUTPUT, OUTPUTS_LEN };
	enum LightId { TRIGGER_LIGHT, LIGHTS_LEN };

	KickVcv() {
		auto cnt = ElementCount::count<KickInfo>();
		config(cnt.num_params, cnt.num_inputs, cnt.num_outputs, cnt.num_lights);

		VCVModuleParamCreator<KickInfo> creator{this};
		for (auto &element : KickInfo::Elements)
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
			inputs[DEPTHCV_INPUT].getChannels(),
			inputs[PITCHDECAYCV_INPUT].getChannels(),
			inputs[AMPDECAYCV_INPUT].getChannels(),
			inputs[SATURATIONCV_INPUT].getChannels(),
		});
		outputs[AUDIO_OUTPUT].setChannels(n);

		const float pitchNorm      = getParamQuantity(PITCH_PARAM)->toScaled(params[PITCH_PARAM].getValue());
		const float pitchDepthNorm = getParamQuantity(PITCHDEPTH_PARAM)->toScaled(params[PITCHDEPTH_PARAM].getValue());
		const float pitchDecayNorm = getParamQuantity(PITCHDECAY_PARAM)->toScaled(params[PITCHDECAY_PARAM].getValue());
		const float ampDecayNorm   = getParamQuantity(AMPDECAY_PARAM)->toScaled(params[AMPDECAY_PARAM].getValue());
		const float satNorm        = getParamQuantity(SATURATION_PARAM)->toScaled(params[SATURATION_PARAM].getValue());
		const float rangeNorm      = getParamQuantity(RANGE_PARAM)->toScaled(params[RANGE_PARAM].getValue());
		const float trigButton     = params[TRIGGER_PARAM].getValue();

		const int trigChans      = inputs[TRIGGERIN_INPUT].getChannels();
		const int pitchChans     = inputs[PITCHCV_INPUT].getChannels();
		const int depthChans     = inputs[DEPTHCV_INPUT].getChannels();
		const int pitchDecChans  = inputs[PITCHDECAYCV_INPUT].getChannels();
		const int ampDecChans    = inputs[AMPDECAYCV_INPUT].getChannels();
		const int satChans       = inputs[SATURATIONCV_INPUT].getChannels();

		for (int c = 0; c < n; c++) {
			auto &core = cores[c];

			core.set_param(PITCH_PARAM,      pitchNorm);
			core.set_param(PITCHDEPTH_PARAM, pitchDepthNorm);
			core.set_param(PITCHDECAY_PARAM, pitchDecayNorm);
			core.set_param(AMPDECAY_PARAM,   ampDecayNorm);
			core.set_param(SATURATION_PARAM, satNorm);
			core.set_param(RANGE_PARAM,      rangeNorm);
			core.set_param(TRIGGER_PARAM,    trigButton);

			auto setIn = [&](int id, InputId port, int numChans) {
				if (inputs[port].isConnected())
					core.set_input(id, inputs[port].getPolyVoltage(std::min(c, numChans - 1)));
				else
					core.mark_input_unpatched(id);
			};
			setIn(TRIGGERIN_INPUT,    TRIGGERIN_INPUT,    trigChans);
			setIn(PITCHCV_INPUT,      PITCHCV_INPUT,      pitchChans);
			setIn(DEPTHCV_INPUT,      DEPTHCV_INPUT,      depthChans);
			setIn(PITCHDECAYCV_INPUT, PITCHDECAYCV_INPUT, pitchDecChans);
			setIn(AMPDECAYCV_INPUT,   AMPDECAYCV_INPUT,   ampDecChans);
			setIn(SATURATIONCV_INPUT, SATURATIONCV_INPUT, satChans);

			core.mark_output_patched(0);
			core.update();
			outputs[AUDIO_OUTPUT].setVoltage(core.get_output(0), c);
		}

		lights[TRIGGER_LIGHT].setBrightness(cores[0].get_led_brightness(0));
	}
};

struct KickWidget : rack::app::ModuleWidget {
	KickWidget(KickVcv *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, KickInfo::svg_filename.data())));

		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(
			rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		VCVWidgetCreator<KickInfo> creator(this, module);
		for (auto &element : KickInfo::Elements)
			std::visit([&creator](auto &el) { creator.create(el); }, element);
	}
};

rack::Model *modelKick = rack::createModel<KickVcv, KickWidget>("Kick");
