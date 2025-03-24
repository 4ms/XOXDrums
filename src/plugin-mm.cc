#include "CoreModules/register_module.hh"
#include "core/Clave.hh"
#include "core/Cowbell.hh"
#include "core/Accent.hh"
#include "core/Clap.hh"

extern "C" __attribute__((__visibility__("default"))) void init() {
	using namespace MetaModule;

	std::string_view brand = "4msDrums";

	register_module<Clave, ClaveInfo>(brand);
	register_module<Cowbell, CowbellInfo>(brand);
	register_module<Accent, AccentInfo>(brand);
	register_module<Clap, ClapInfo>(brand);
}
