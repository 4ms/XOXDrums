#include "Accent.hh"
#include "Clap.hh"
#include "Clave.hh"
#include "Congabongo.hh"
#include "CoreModules/register_module.hh"
#include "Cowbell.hh"
#include "Duck.hh"
#include "HiHat.hh"
#include "Kick.hh"
#include "Maraca.hh"
#include "Rimshot.hh"
#include "Snare.hh"
#include "Tom.hh"

extern "C" void init() {
	using namespace MetaModule;

	std::string_view brand = "4ms-XOXDrums";

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
