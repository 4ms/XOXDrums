#include "plugin-vcv.hh"
#include "vcv/generic_module.hh"

#include "Accent.hh"
#include "Duck.hh"

using namespace MetaModule;

rack::Plugin *pluginInstance;

extern rack::Model *modelClap;
extern rack::Model *modelClave;
extern rack::Model *modelCowbell;
extern rack::Model *modelHiHat;
extern rack::Model *modelKick;
extern rack::Model *modelMaraca;
extern rack::Model *modelRimshot;
extern rack::Model *modelSnare;
extern rack::Model *modelTom;
extern rack::Model *modelCongabongo;

void init(rack::Plugin *p) {
	pluginInstance = p;

	rack::Model *modelAccent = GenericModule<AccentInfo, Accent>::create();
	rack::Model *modelDuck = GenericModule<DuckInfo, Duck>::create();

	p->addModel(modelClave);
	p->addModel(modelCowbell);
	p->addModel(modelAccent);
	p->addModel(modelClap);
	p->addModel(modelCongabongo);
	p->addModel(modelHiHat);
	p->addModel(modelMaraca);
	p->addModel(modelRimshot);
	p->addModel(modelKick);
	p->addModel(modelSnare);
	p->addModel(modelTom);
	p->addModel(modelDuck);
}
