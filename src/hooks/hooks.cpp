#include "Hooks/hooks.h"

#include "Alchemy/createdObjectMonitor.h"
#include "Common/combatHit.h"
#include "Common/spellCast.h"

namespace Hooks {
	void Install()
	{
		logger::info("Installing Alchemy hooks...");
		Hooks::Alchemy::Install();
		logger::info("Installing Spell Cast hooks...");
		Hooks::Spells::Install();
		logger::info("Installing Combat Hit hooks...");
		Hooks::CombatHit::Install();
	}
}