#include "MagicTargetHooks.h"

#include "Data/Lookup.h"
#include "Data/ModObjectManager.h"

namespace Hooks::MagicTargetHooks::HeavyArmor
{
	inline static bool AbsorbedByIndomitable(RE::MagicTarget* a_this, const RE::Effect* a_effect, RE::Actor* a_shooter)
	{
		if (!a_this || !a_effect || !a_effect->baseEffect) {
			return false;
		}

		auto* baseEffect = a_effect->baseEffect;
		bool validType = baseEffect->GetArchetype() == RE::EffectSetting::Archetype::kParalysis;
		validType |= baseEffect->GetArchetype() == RE::EffectSetting::Archetype::kDisarm;
		if (!validType) {
			return false;
		}

		auto* indomitable = Data::ModObject<RE::BGSPerk>(Data::HeavyArmor_Indomitable);
		auto* heavyCuirass = Data::ModObject<RE::BGSKeyword>(Data::HeavyArmor_HeavyCuirass);
		auto* target = a_this->GetTargetAsActor();
		if (!indomitable || 
			!target || 
			!heavyCuirass || 
			!target->HasPerk(indomitable)) 
		{
			return false;
		}

		auto* equipped = target->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody);
		if (!equipped || !equipped->HasKeyword(heavyCuirass)) {
			return false;
		}

		auto* dobj = RE::BGSDefaultObjectManager::GetSingleton();
		if (!dobj) {
			logger::warn("Failed to get DOBJ manager from the game. This may be fatal."sv);
			return false;
		}

		auto* absorbEffect = dobj->GetObject<RE::BGSArtObject>(
			RE::DEFAULT_OBJECT::kArtObjectAbsorbEffect);
		if (!absorbEffect) {
			logger::warn("Default Absorb Effect art object is NULL or undefined - this may cause a crash."sv);
			return false;
		}

		auto absorbVisualTimer = "fMagicAbsorbVisualTimer"_gs.value_or(1.0f);
		float timer = std::clamp(absorbVisualTimer, 0.0f, 2.0f);
		target->ApplyArtObject(absorbEffect,
			timer,
			a_shooter,
			false,
			true,
			nullptr,
			false);
		return true;
	}
}
namespace Hooks::MagicTargetHooks
{
	bool Install()
	{
		logger::info("  >Installing Magic Target related hooks..."sv);
		bool result = true;
		result &= MagicTarget_CheckAbsorbHook_PC::InstallCheckAbsorbHook_PC();
		result &= MagicTarget_CheckAbsorbHook_NPC::InstallCheckAbsorbHook_NPC();
		return result;
	}

	inline bool MagicTarget_CheckAbsorbHook_PC::InstallCheckAbsorbHook_PC() {
		REL::Relocation<std::uintptr_t> vtbl{ RE::PlayerCharacter::VTABLE[4] };
		_func_CheckAbsorb_PC = vtbl.write_vfunc(0xB, Thunk_CheckAbsorb_PC);
		logger::info("    Installed Check Absorb (Player) hook."sv);
		return true;
	}

	inline bool MagicTarget_CheckAbsorbHook_PC::Thunk_CheckAbsorb_PC(RE::MagicTarget* a_this, 
		RE::Actor* a_actor, 
		RE::MagicItem* a_spell, 
		const RE::Effect* a_effect)
	{
		bool wouldAbsorb = _func_CheckAbsorb_PC(a_this, a_actor, a_spell, a_effect);
		wouldAbsorb |= HeavyArmor::AbsorbedByIndomitable(a_this, a_effect, a_actor);

		return wouldAbsorb;
	}

	inline bool MagicTarget_CheckAbsorbHook_NPC::InstallCheckAbsorbHook_NPC() {
		REL::Relocation<std::uintptr_t> vtbl{ RE::Character::VTABLE[4] };
		_func_CheckAbsorb_NPC = vtbl.write_vfunc(0xB, Thunk_CheckAbsorb_NPC);
		logger::info("    Installed Check Absorb (NPC) hook."sv);
		return true;
	}

	inline bool MagicTarget_CheckAbsorbHook_NPC::Thunk_CheckAbsorb_NPC(RE::MagicTarget* a_this,
		RE::Actor* a_actor,
		RE::MagicItem* a_spell,
		const RE::Effect* a_effect)
	{
		bool wouldAbsorb = _func_CheckAbsorb_NPC(a_this, a_actor, a_spell, a_effect);
		wouldAbsorb |= HeavyArmor::AbsorbedByIndomitable(a_this, a_effect, a_actor);

		return wouldAbsorb;
	}
}