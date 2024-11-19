#include "destructionManager.h"

#include "data/modObjectManager.h"

namespace Hooks::Destruction
{
	void Install()
	{
		DestructionManager::GetSingleton()->Install();
	}

	void DestructionManager::Install()
	{
		//1.6.1170: 1407d1692
		REL::Relocation<std::uintptr_t> explosionProcessTargetTarget{ REL::ID(44204), 0x21F };
		if (!REL::make_pattern<"E8">().match(explosionProcessTargetTarget.address())) {
			util::report_and_fail("Destruction Explosion Manager: Failed to validate pattern, aborting launch.");
		}

		auto& trampoline = SKSE::GetTrampoline();
		_explosionProcessTargets = trampoline.write_call<5>(explosionProcessTargetTarget.address(), &Explosion__ProcessTargets);
	}
	bool DestructionManager::Explosion__ProcessTargets(RE::Projectile* a_this, RE::TESObjectREFR* a_hit, void* a3, void* a4, int a5, void* a6, void* a7)
	{
		const auto spell = a_this ? a_this->spell : nullptr;
		const auto hitActor = a_hit ? a_hit->As<RE::Actor>() : nullptr;
		const auto sourceActor = a_this && a_this->GetActorCause() ? a_this->GetActorCause()->actor.get().get() : nullptr;
		const auto magicCaster = sourceActor ? sourceActor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant) : nullptr;

		if (spell && hitActor && magicCaster) {
			const auto reduceFire = Data::ModObject<RE::SpellItem>("ARM_Destruction_SPL_ReduceResistanceFire");
			const auto reduceFrost = Data::ModObject<RE::SpellItem>("ARM_Destruction_SPL_ReduceResistanceFrost");
			const auto reduceShock = Data::ModObject<RE::SpellItem>("ARM_Destruction_SPL_ReduceResistanceShock");
			assert(reduceFire && reduceFrost && reduceShock);

			const auto elementalStrike = Data::ModObject<RE::BGSPerk>("ARM_Destruction_PRK_040_ElementalStrike");
			assert(elementalStrike);

			if (elementalStrike && reduceFire && reduceFrost && reduceShock) {
				if (elementalStrike && sourceActor->HasPerk(elementalStrike)) {
					bool fire = false;
					bool frost = false;
					bool shock = false;

					for (const auto effect : spell->effects) {
						if (!effect->baseEffect) {
							continue;
						}

						const auto baseEffect = effect->baseEffect;
						if (!fire && baseEffect->HasKeywordString("MagicDamageFire")) {
							fire = true;
						}
						if (!frost && baseEffect->HasKeywordString("MagicDamageFrost")) {
							frost = true;
						}
						if (!shock && baseEffect->HasKeywordString("MagicDamageShock")) {
							shock = true;
						}
					}

					logger::debug("Front: {}", Utilities::EDID::GetEditorID(reduceFire->effects.back()->baseEffect));
					if (fire && !hitActor->HasMagicEffect(reduceFire->effects.front()->baseEffect)) {
						magicCaster->CastSpellImmediate(reduceFire, false, hitActor, 1.0f, false, 0.0f, hitActor);
					}

					if (frost && !hitActor->HasMagicEffect(reduceFrost->effects.front()->baseEffect)) {
						magicCaster->CastSpellImmediate(reduceFrost, false, hitActor, 1.0f, false, 0.0f, hitActor);
					}

					if (shock && !hitActor->HasMagicEffect(reduceShock->effects.front()->baseEffect)) {
						magicCaster->CastSpellImmediate(reduceShock, false, hitActor, 1.0f, false, 0.0f, hitActor);
					}
				}
			}
		}
		return _explosionProcessTargets(a_this, a_hit, a3, a4, a5, a6, a7);
	}
}