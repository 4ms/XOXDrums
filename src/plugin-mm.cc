#include "CoreModules/register_module.hh"
#include "core/Clave.hh"

extern "C" __attribute__((__visibility__("default"))) void init() {
	using namespace MetaModule;

	std::string_view brand = "4msDrums";

	register_module<Clave, ClaveInfo>(brand);
}
