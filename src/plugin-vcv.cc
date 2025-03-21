#include "plugin-vcv.hh"
#include "vcv/generic_module.hh"

#include "core/Clave.hh"

using namespace MetaModule;

rack::Plugin *pluginInstance;

__attribute__((__visibility__("default"))) void init(rack::Plugin *p) {
	pluginInstance = p;

	rack::Model *modelClave = GenericModule<ClaveInfo, Clave>::create();

	p->addModel(modelClave);
}
