#include "plugin-vcv.hh"
#include "vcv/generic_module.hh"

#include "core/Clave.hh"
#include "core/Cowbell.hh"

using namespace MetaModule;

rack::Plugin *pluginInstance;

__attribute__((__visibility__("default"))) void init(rack::Plugin *p) {
	pluginInstance = p;

	rack::Model *modelClave = GenericModule<ClaveInfo, Clave>::create();
	rack::Model *modelCowbell = GenericModule<CowbellInfo, Cowbell>::create();
	rack::Model *modelAccent = GenericModule<AccentInfo, Accent>::create();

	p->addModel(modelClave);
	p->addModel(modelCowbell);
	p->addModel(modelAccent);
}
