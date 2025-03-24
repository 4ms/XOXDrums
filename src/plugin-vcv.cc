#include "plugin-vcv.hh"
#include "vcv/generic_module.hh"

#include "core/Clave.hh"
#include "core/Cowbell.hh"
#include "core/Accent.hh"
#include "core/Clap.hh"
#include "core/Congabongo.hh"
#include "core/HiHat.hh"

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

	p->addModel(modelClave);
	p->addModel(modelCowbell);
	p->addModel(modelAccent);
	p->addModel(modelClap);
	p->addModel(modelCongabongo);
	p->addModel(modelHiHat);
}
