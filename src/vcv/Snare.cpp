#include "plugin-vcv.hh"
#include "CoreModules/elements/element_counter.hh"
#include "modules/Snare.hh"
#include "modules/info/Snare_info.hh"
#include "widgets/vcv_module_creator.hh"
#include "widgets/vcv_widget_creator.hh"

using namespace rack;
using namespace MetaModule;

struct SnareVcv : rack::Module {
	Snare cores[PORT_MAX_CHANNELS];

	enum ParamId {
		PITCH_PARAM,
		PITCHDECAY_PARAM,
		PITCHENVAMOUNT_PARAM,
		NOISECOLOR_PARAM,
		NOISEDECAY_PARAM,
		BODYDECAY_PARAM,
		BODYNOISE_PARAM,
		RANGE_PARAM,
		SATURATION_PARAM,
		TRIGGER_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		TRIGGERIN_INPUT,
		PITCHCV_INPUT,
		PITCHDECAYCV_INPUT,
		PITCHAMOUNTCV_INPUT,
		NOISECOLORCV_INPUT,
		NOISEDECAYCV_INPUT,
		BODYDECAYCV_INPUT,
		BODYNOISECV_INPUT,
		SATURATIONCV_INPUT,
		INPUTS_LEN
	};
	enum OutputId { AUDIO_OUTPUT, OUTPUTS_LEN };
	enum LightId { TRIGGER_LIGHT, LIGHTS_LEN };

	SnareVcv() {
		auto cnt = ElementCount::count<SnareInfo>();
		config(cnt.num_params, cnt.num_inputs, cnt.num_outputs, cnt.num_lights);

		VCVModuleParamCreator<SnareInfo> creator{this};
		for (auto &element : SnareInfo::Elements)
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
			inputs[PITCHDECAYCV_INPUT].getChannels(),
			inputs[PITCHAMOUNTCV_INPUT].getChannels(),
			inputs[NOISECOLORCV_INPUT].getChannels(),
			inputs[NOISEDECAYCV_INPUT].getChannels(),
			inputs[BODYDECAYCV_INPUT].getChannels(),
			inputs[BODYNOISECV_INPUT].getChannels(),
			inputs[SATURATIONCV_INPUT].getChannels(),
		});
		outputs[AUDIO_OUTPUT].setChannels(n);

		const float pitchNorm      = getParamQuantity(PITCH_PARAM)->toScaled(params[PITCH_PARAM].getValue());
		const float pitchDecNorm   = getParamQuantity(PITCHDECAY_PARAM)->toScaled(params[PITCHDECAY_PARAM].getValue());
		const float pitchAmtNorm   = getParamQuantity(PITCHENVAMOUNT_PARAM)->toScaled(params[PITCHENVAMOUNT_PARAM].getValue());
		const float noiseColNorm   = getParamQuantity(NOISECOLOR_PARAM)->toScaled(params[NOISECOLOR_PARAM].getValue());
		const float noiseDecNorm   = getParamQuantity(NOISEDECAY_PARAM)->toScaled(params[NOISEDECAY_PARAM].getValue());
		const float bodyDecNorm    = getParamQuantity(BODYDECAY_PARAM)->toScaled(params[BODYDECAY_PARAM].getValue());
		const float bodyNoiseNorm  = getParamQuantity(BODYNOISE_PARAM)->toScaled(params[BODYNOISE_PARAM].getValue());
		const float rangeNorm      = getParamQuantity(RANGE_PARAM)->toScaled(params[RANGE_PARAM].getValue());
		const float satNorm        = getParamQuantity(SATURATION_PARAM)->toScaled(params[SATURATION_PARAM].getValue());
		const float trigButton     = params[TRIGGER_PARAM].getValue();

		const int trigChans      = inputs[TRIGGERIN_INPUT].getChannels();
		const int pitchChans     = inputs[PITCHCV_INPUT].getChannels();
		const int pitchDecChans  = inputs[PITCHDECAYCV_INPUT].getChannels();
		const int pitchAmtChans  = inputs[PITCHAMOUNTCV_INPUT].getChannels();
		const int noiseColChans  = inputs[NOISECOLORCV_INPUT].getChannels();
		const int noiseDecChans  = inputs[NOISEDECAYCV_INPUT].getChannels();
		const int bodyDecChans   = inputs[BODYDECAYCV_INPUT].getChannels();
		const int bodyNoiseChans = inputs[BODYNOISECV_INPUT].getChannels();
		const int satChans       = inputs[SATURATIONCV_INPUT].getChannels();

		for (int c = 0; c < n; c++) {
			auto &core = cores[c];

			core.set_param(PITCH_PARAM,           pitchNorm);
			core.set_param(PITCHDECAY_PARAM,      pitchDecNorm);
			core.set_param(PITCHENVAMOUNT_PARAM,  pitchAmtNorm);
			core.set_param(NOISECOLOR_PARAM,      noiseColNorm);
			core.set_param(NOISEDECAY_PARAM,      noiseDecNorm);
			core.set_param(BODYDECAY_PARAM,       bodyDecNorm);
			core.set_param(BODYNOISE_PARAM,       bodyNoiseNorm);
			core.set_param(RANGE_PARAM,           rangeNorm);
			core.set_param(SATURATION_PARAM,      satNorm);
			core.set_param(TRIGGER_PARAM,         trigButton);

			auto setIn = [&](int id, InputId port, int numChans) {
				if (inputs[port].isConnected())
					core.set_input(id, inputs[port].getPolyVoltage(std::min(c, numChans - 1)));
				else
					core.mark_input_unpatched(id);
			};
			setIn(TRIGGERIN_INPUT,    TRIGGERIN_INPUT,    trigChans);
			setIn(PITCHCV_INPUT,      PITCHCV_INPUT,      pitchChans);
			setIn(PITCHDECAYCV_INPUT, PITCHDECAYCV_INPUT, pitchDecChans);
			setIn(PITCHAMOUNTCV_INPUT,PITCHAMOUNTCV_INPUT,pitchAmtChans);
			setIn(NOISECOLORCV_INPUT, NOISECOLORCV_INPUT, noiseColChans);
			setIn(NOISEDECAYCV_INPUT, NOISEDECAYCV_INPUT, noiseDecChans);
			setIn(BODYDECAYCV_INPUT,  BODYDECAYCV_INPUT,  bodyDecChans);
			setIn(BODYNOISECV_INPUT,  BODYNOISECV_INPUT,  bodyNoiseChans);
			setIn(SATURATIONCV_INPUT, SATURATIONCV_INPUT, satChans);

			core.mark_output_patched(0);
			core.update();
			outputs[AUDIO_OUTPUT].setVoltage(core.get_output(0), c);
		}

		lights[TRIGGER_LIGHT].setBrightness(cores[0].get_led_brightness(0));
	}
};

struct SnareWidget : rack::app::ModuleWidget {
	SnareWidget(SnareVcv *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, SnareInfo::svg_filename.data())));

		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(
			rack::math::Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		VCVWidgetCreator<SnareInfo> creator(this, module);
		for (auto &element : SnareInfo::Elements)
			std::visit([&creator](auto &el) { creator.create(el); }, element);
	}
};

rack::Model *modelSnare = rack::createModel<SnareVcv, SnareWidget>("Snare");
