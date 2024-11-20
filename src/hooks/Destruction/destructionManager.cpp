#include "destructionManager.h"

#include "data/modObjectManager.h"
#include "RE/Misc.h"

namespace
{
	void ProcessHit(RE::Projectile* a_this, RE::TESObjectREFR* a_hit) {
		const auto spell = a_this ? a_this->spell : nullptr;
		const auto hitActor = a_hit ? a_hit->As<RE::Actor>() : nullptr;
		const auto sourceActor = a_this && a_this->GetActorCause() ? a_this->GetActorCause()->actor.get().get() : nullptr;
		const auto magicCaster = sourceActor ? sourceActor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant) : nullptr;

		if (spell && hitActor && magicCaster) {
			const bool wasDualCast = a_this->flags.any(RE::Projectile::Flags::kIsDual);
			const auto inspiredPerk = Data::ModObject<RE::BGSPerk>("ARM_Destruction_PRK_040_Inspired");
			const auto inspiredProc = Data::ModObject<RE::SpellItem>("ARM_Destruction_SPL_InspireProc");
			assert(inspiredPerk && inspiredProc);

			bool fire = false;
			bool frost = false;
			bool shock = false;
			const auto fireKwd = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageFire");
			const auto frostKwd = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageFrost");
			const auto shockKwd = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicDamageShock");
			assert(fireKwd && frostKwd && shockKwd);

			if (fireKwd && frostKwd && shockKwd) {
				for (const auto effect : spell->effects) {
					if (!effect->baseEffect) {
						continue;
					}

					const auto baseEffect = effect->baseEffect;
					if (baseEffect->HasKeyword(fireKwd)) {
						fire = true;
					}
					if (baseEffect->HasKeyword(frostKwd)) {
						frost = true;
					}
					if (baseEffect->HasKeyword(shockKwd)) {
						shock = true;
					}
				}
			}

			const auto incapacitatePerk = Data::ModObject<RE::BGSPerk>("ARM_Destruction_PRK_050_Incapacitate");
			const auto incapacitateProc = Data::ModObject<RE::SpellItem>("ARM_Destruction_SPL_IncapacitateProc");
			const auto arcBurnPerk = Data::ModObject<RE::BGSPerk>("ARM_Destruction_PRK_070_ArcBurn");
			assert(arcBurnPerk && incapacitatePerk && incapacitateProc);

			if (shock && incapacitatePerk && incapacitateProc && sourceActor->HasPerk(incapacitatePerk)) {
				logger::debug("STRONG HIT");
				if (arcBurnPerk && sourceActor->HasPerk(arcBurnPerk)) {
					const auto damage = hitActor->GetActorValue(RE::ActorValue::kWardPower)
						+ hitActor->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, RE::ActorValue::kWardPower) + 1.0f;
					hitActor->ModActorValue(RE::ActorValue::kWardPower, -1.0f * damage);
				}
				magicCaster->CastSpellImmediate(incapacitateProc, false, hitActor, 1.0f, false, 0.0f, sourceActor);
			}

			if (wasDualCast && inspiredPerk && sourceActor->HasPerk(inspiredPerk) && !sourceActor->HasMagicEffect(inspiredProc->effects.front()->baseEffect)) {
				magicCaster->CastSpellImmediate(inspiredProc, false, nullptr, 1.0f, false, 0.0f, sourceActor);
			}

			const auto reignitePerk = Data::ModObject<RE::BGSPerk>("ARM_Destruction_PRK_050_ReIgnite");
			const auto reigniteProc = Data::ModObject<RE::SpellItem>("ARM_Destruction_SPL_ReIgniteProc");
			assert(reignitePerk && reigniteProc);

			if (fire && reignitePerk && sourceActor->HasPerk(reignitePerk) && reigniteProc) {
				magicCaster->CastSpellImmediate(reigniteProc, false, hitActor, 1.0f, false, 0.0f, sourceActor);
			}

			const auto witherPerk = Data::ModObject<RE::BGSPerk>("ARM_Destruction_PRK_050_Wither");
			const auto witherProc = Data::ModObject<RE::SpellItem>("ARM_Destruction_SPL_WitherProc");
			assert(witherPerk && witherProc);

			if (frost && witherPerk && sourceActor->HasPerk(witherPerk) && witherProc && !hitActor->HasMagicEffect(witherProc->effects.front()->baseEffect)) {
				magicCaster->CastSpellImmediate(witherProc, false, hitActor, 1.0f, false, 0.0f, sourceActor);
			}
		}
	}
}
namespace Hooks::Destruction
{
	void Install()
	{
		DestructionManager::GetSingleton()->Install();
	}

	void DestructionManager::Install()
	{
		//1.6.1170: 1407d1692
		REL::Relocation<std::uintptr_t> spellCollisionTarget{ REL::ID(44204), 0x21F };
		REL::Relocation<std::uintptr_t> spellCollisionBeamTarget{ REL::ID(44166), 0x4DE };
		if (!REL::make_pattern<"E8">().match(spellCollisionTarget.address()) ||
			!REL::make_pattern<"E8">().match(spellCollisionBeamTarget.address()) ) {
			util::report_and_fail("Destruction Explosion Manager: Failed to validate pattern, aborting launch.");
		}

		auto& trampoline = SKSE::GetTrampoline();
		_processSpellCollision = trampoline.write_call<5>(spellCollisionTarget.address(), &ProcessSpellCollision);
		_processSpellCollisionBeam = trampoline.write_call<5>(spellCollisionBeamTarget.address(), &ProcessSpellCollisionBeam);
	}

	bool DestructionManager::ProcessSpellCollision(RE::Projectile* a_this, RE::TESObjectREFR* a_hit, void* a3, void* a4, int a5, void* a6, void* a7)
	{
		ProcessHit(a_this, a_hit);
		return _processSpellCollision(a_this, a_hit, a3, a4, a5, a6, a7);
	}

	bool DestructionManager::ProcessSpellCollisionBeam(RE::Projectile* a_this, RE::TESObjectREFR* a_hit, void* a3, void* a4, int a5, void* a6, void* a7)
	{
		const auto thisSpell = a_this ? a_this->spell : nullptr;
		const auto isConc = thisSpell ? thisSpell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration : true;

		if (!isConc) {
			ProcessHit(a_this, a_hit);
		}
		return _processSpellCollisionBeam(a_this, a_hit, a3, a4, a5, a6, a7);
	}
}