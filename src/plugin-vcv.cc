#include "plugin-vcv.hh"
#include "vcv/generic_module.hh"

#include "Accent.hh"
#include "Clap.hh"
#include "Clave.hh"
#include "Congabongo.hh"
#include "Cowbell.hh"
#include "Duck.hh"
#include "HiHat.hh"
#include "Kick.hh"
#include "Maraca.hh"
#include "Rimshot.hh"
#include "Snare.hh"
#include "Tom.hh"

using namespace MetaModule;

rack::Plugin *pluginInstance;

void init(rack::Plugin *p) {
	pluginInstance = p;

	rack::Model *modelClave = GenericModule<ClaveInfo, Clave>::create();
	rack::Model *modelCowbell = GenericModule<CowbellInfo, Cowbell>::create();
	rack::Model *modelAccent = GenericModule<AccentInfo, Accent>::create();
	rack::Model *modelClap = GenericModule<ClapInfo, Clap>::create();
	rack::Model *modelCongabongo = GenericModule<CongabongoInfo, Congabongo>::create();
	rack::Model *modelHiHat = GenericModule<HiHatInfo, HiHat>::create();
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
