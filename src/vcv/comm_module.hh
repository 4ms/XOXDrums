#pragma once
#include "CoreModules/CoreProcessor.hh"
#include "comm_jack.hh"

// CommModule wraps CoreProcessors for use in VCV Rack

class CommModule : public rack::Module {
public:
	std::unique_ptr<CoreProcessor> core;
	std::vector<CommInputJack> inJacks;
	std::vector<CommOutputJack> outJacks;

private:
	bool sampleRateChanged = true;

protected:
	CommModule() = default;
	~CommModule() = default;

	void configComm(unsigned NUM_PARAMS, unsigned NUM_INPUTS, unsigned NUM_OUTPUTS, unsigned NUM_LIGHTS);
	void process(const ProcessArgs &args) override;

	void processBypass(const ProcessArgs &) override {
		for (auto &out : outJacks) {
			out.setValue(0.f);
		}
		for (auto &route : bypassRoutes) {
			if (route.inputId >= 0 && route.outputId >= 0)
				outputs[route.outputId].setVoltage(inputs[route.inputId].getVoltage());
		}
	}

	void onSampleRateChange() override;

	json_t *dataToJson() override;
	void dataFromJson(json_t *rootJ) override;
};
