#include "deathEvent.h"

#include "data/modObjectManager.h"

namespace Events::Death
{
	RE::BSEventNotifyControl Listener::ProcessEvent(const RE::TESDeathEvent* a_event, RE::BSTEventSource<RE::TESDeathEvent>*)
	{
		using control = RE::BSEventNotifyControl;

		if (!a_event || !a_event->dead) {
			return control::kContinue;
		}

		const auto dyingReference = a_event->actorDying.get();
		const auto dyingActor = dyingReference ? dyingReference->As<RE::Actor>() : nullptr;
		const auto commandingActor = dyingActor ? dyingActor->GetCommandingActor().get() : nullptr;
		if (!commandingActor) {
			return control::kContinue;
		}

		const auto partingGiftPerk = Data::ModObject<RE::BGSPerk>("ARM_Conjuration_PRK_060_PartingGift");
		const auto partingGiftSpell = Data::ModObject<RE::SpellItem>("ARM_Conjuration_SPL_PartingGiftProc");
		assert(partingGiftPerk && partingGiftSpell);

		if (partingGiftSpell && partingGiftPerk && commandingActor->HasPerk(partingGiftPerk)) {
			const auto commanderCaster = commandingActor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
			if (commanderCaster) {
				commanderCaster->CastSpellImmediate(partingGiftSpell, false, commandingActor, 1.0f, false, 0.0f, commandingActor);
			}
		}
		return control::kContinue;
	}
}