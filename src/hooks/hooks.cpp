#include "Hooks/hooks.h"

#include "Alchemy/createdObjectMonitor.h"
#include "Armor/armorHooks.h"
#include "Common/combatHit.h"
#include "Common/conditions.h"
#include "Common/spellCast.h"
#include "Conjuration/conjurationHooks.h"
#include "Destruction/destructionManager.h"

namespace Hooks {
	void Install()
	{
		logger::info("Installing Alchemy hooks...");
		Hooks::Alchemy::Install();
		logger::info("Installing Armor hooks...");
		Hooks::Armor::Install();
		logger::info("Installing Spell Cast hooks...");
		Hooks::Spells::Install();
		logger::info("Installing Condition hooks...");
		Conditions::Install();
		logger::info("Installing Combat Hit hooks...");
		Hooks::CombatHit::Install();
		logger::info("Installing Conjuration hooks...");
		Hooks::Conjuration::Install();
		logger::info("Installing Destruction hooks...");
		Hooks::Destruction::Install();
	}
}