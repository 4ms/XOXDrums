#include "CoreModules/register_module.hh"
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

extern "C" __attribute__((__visibility__("default"))) void init() {
	using namespace MetaModule;

	std::string_view brand = "4msDrums";

	register_module<Clave, ClaveInfo>(brand);
	register_module<Cowbell, CowbellInfo>(brand);
	register_module<Accent, AccentInfo>(brand);
	register_module<Clap, ClapInfo>(brand);
	register_module<Congabongo, CongabongoInfo>(brand);
	register_module<HiHat, HiHatInfo>(brand);
	register_module<Maraca, MaracaInfo>(brand);
	register_module<Rimshot, RimshotInfo>(brand);
	register_module<Kick, KickInfo>(brand);
	register_module<Snare, SnareInfo>(brand);
	register_module<Tom, TomInfo>(brand);
	register_module<Duck, DuckInfo>(brand);
}
