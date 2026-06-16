#include "plugin-vcv.hh"
#include "vcv/generic_module.hh"

#include "Accent.hh"
#include "Congabongo.hh"
#include "Duck.hh"
#include "Kick.hh"
#include "Maraca.hh"
#include "Rimshot.hh"
#include "Snare.hh"
#include "Tom.hh"

using namespace MetaModule;

rack::Plugin *pluginInstance;

extern rack::Model *modelClap;
extern rack::Model *modelClave;
extern rack::Model *modelCowbell;
extern rack::Model *modelHiHat;

void init(rack::Plugin *p) {
	pluginInstance = p;

	rack::Model *modelAccent = GenericModule<AccentInfo, Accent>::create();
	rack::Model *modelCongabongo = GenericModule<CongabongoInfo, Congabongo>::create();
	rack::Model *modelMaraca = GenericModule<MaracaInfo, Maraca>::create();
	rack::Model *modelRimshot = GenericModule<RimshotInfo, Rimshot>::create();
	rack::Model *modelKick = GenericModule<KickInfo, Kick>::create();
	rack::Model *modelTom = GenericModule<TomInfo, Tom>::create();
	rack::Model *modelDuck = GenericModule<DuckInfo, Duck>::create();
	rack::Model *modelSnare = GenericModule<SnareInfo, Snare>::create();

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
