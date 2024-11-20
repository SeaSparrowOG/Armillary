#include "combatHit.h"

#include "data/modObjectManager.h"

namespace Hooks::CombatHit
{
	void Install()
	{
		CombatHitManager::GetSingleton()->Install();
	}

	void CombatHitManager::Install()
	{
		REL::Relocation<std::uintptr_t> selectItemTarget{ REL::ID(38627), 0x4A8 };

		if (!REL::make_pattern<"E8">().match(selectItemTarget.address())) {
			util::report_and_fail("Common Combat Hooks: Failed to validate pattern, aborting launch.");
		}

		auto& trampoline = SKSE::GetTrampoline();
		_combatHit = trampoline.write_call<5>(selectItemTarget.address(), &CombatHit);
	}

	void CombatHitManager::CombatHit(RE::Actor* a_target, RE::HitData* a_hitData)
	{
		const auto staggerSpellCooldown = Data::ModObject<RE::EffectSetting>("ARM_Common_MGF_StaggerCooldownFFTActor");
		const auto staggerSpell = Data::ModObject<RE::SpellItem>("ARM_Common_SPL_StaggerCooldownSpell");
		assert(staggerSpell && staggerSpellCooldown);
		
		const auto attacker = a_hitData ? a_hitData->aggressor.get().get() : nullptr;
		if (!attacker || !a_target) {
			_combatHit(a_target, a_hitData);
			return;
		}
		const auto magicCaster = attacker->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		if (!magicCaster) {
			_combatHit(a_target, a_hitData);
			return;
		}

		const auto attackData = a_hitData ? a_hitData->attackData.get() : nullptr;
		const auto isPowerAttack = attackData ? attackData->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack) : false;
		const auto wasBlocked = a_target ? a_target->IsBlocking() : false;

		if (wasBlocked) {
			// TODO: Gatecrasher
		}
		else {
			if (isPowerAttack && !a_target->HasMagicEffect(staggerSpellCooldown)) {
				magicCaster->CastSpellImmediate(staggerSpell, true, a_target, 1.0f, false, 0.0f, attacker);
			}
		}
		_combatHit(a_target, a_hitData);
	}
}