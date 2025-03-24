#include "plugin-vcv.hh"
#include "vcv/generic_module.hh"

#include "core/Clave.hh"
#include "core/Cowbell.hh"
#include "core/Accent.hh"
#include "core/Clap.hh"
#include "core/Congabongo.hh"
#include "core/HiHat.hh"
#include "core/Maraca.hh"
#include "core/Rimshot.hh"
#include "core/Kick.hh"
#include "core/Snare.hh"
#include "core/Tom.hh"
#include "core/Duck.hh"

using namespace MetaModule;

rack::Plugin *pluginInstance;

__attribute__((__visibility__("default"))) void init(rack::Plugin *p) {
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
