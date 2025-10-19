#include "MagicTargetHooks.h"

#include "Data/ModObjectManager.h"

namespace Hooks::MagicTargetHooks
{
	bool Install()
	{
		logger::info("  >Installing Magic Target related hooks..."sv);
		bool result = true;
		result &= MagicTarget_CheckAbsorbHook::InstallCheckAbsorbHook();
		return result;
	}

	inline bool MagicTarget_CheckAbsorbHook::InstallCheckAbsorbHook() {
		
		return false;
	}

	inline bool MagicTarget_CheckAbsorbHook::Thunk(RE::MagicTarget* a_this, 
		RE::Actor* a_actor, 
		RE::MagicItem* a_spell, 
		const RE::Effect* a_effect)
	{
		bool wouldAbsorb = _func(a_this, a_actor, a_spell, a_effect);
		if (wouldAbsorb || !a_this || !a_effect || !a_effect->baseEffect) {
			return wouldAbsorb;
		}

		auto* baseEffect = a_effect->baseEffect;
		bool validType = baseEffect->GetArchetype() == RE::EffectSetting::Archetype::kParalysis;
		validType |= baseEffect->GetArchetype() == RE::EffectSetting::Archetype::kDisarm;

		if (!validType) {
			return wouldAbsorb;
		}

		auto* indomitable = Data::ModObject<RE::BGSPerk>(Data::HeavyArmor_Indomitable);
		auto* target = a_this->GetTargetAsActor();
		if (!indomitable || !target || !target->HasPerk(indomitable)) {
			return wouldAbsorb;
		}

		return true;
	}
}