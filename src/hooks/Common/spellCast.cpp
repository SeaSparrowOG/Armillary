#include "spellCast.h"

#include "data/modObjectManager.h"
#include "RE/Misc.h"

namespace Hooks::Spells
{
	void ChargeTimeManager::Install()
	{
		//1.6.1170: 1405b1c0b
		REL::Relocation<std::uintptr_t> setCastingTimerForChargeTarget{ REL::ID(34146), 0x4B }; //Expected: e9 20 cc 00 00
		if (!REL::make_pattern<"E9">().match(setCastingTimerForChargeTarget.address())) {
			util::report_and_fail("Common Spell Cast: Failed to validate pattern, aborting launch.");
		}

		auto& trampoline = SKSE::GetTrampoline();
		_setCastingTimerForCharge = trampoline.write_branch<5>(setCastingTimerForChargeTarget.address(), &SetCastingTimerForCharge);
	}

	void ChargeTimeManager::SetCastingTimerForCharge(RE::MagicCaster* a_magicCaster)
	{
		_setCastingTimerForCharge(a_magicCaster);

		if (!a_magicCaster) {
			return;
		}

		const auto currentSpell = a_magicCaster->currentSpell;
		if (!currentSpell) {
			return;
		}

		float chargeTime = 1.25f;

		if (currentSpell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
			chargeTime -= 0.25;
		}

		const auto illusionQuickcharge = Data::ModObject<RE::BGSPerk>("ARM_Illusion_PRK_020_IllusionDualCasting");
		const auto conjurationQuickcharge = Data::ModObject<RE::BGSPerk>("ARM_Conjuration_PRK_020_ConjurationDualCasting");
		const auto destructionQuickcharge = Data::ModObject<RE::BGSPerk>("ARM_Destruction_PRK_020_DestructionDualCasting");
		const auto restorationQuickcharge = Data::ModObject<RE::BGSPerk>("ARM_Restoration_PRK_020_DualCasting");
		const auto alterationQuickcharge = Data::ModObject<RE::BGSPerk>("ARM_Alteration_PRK_020_AlterationDualCasting");
		assert(illusionQuickcharge && conjurationQuickcharge && destructionQuickcharge && restorationQuickcharge && alterationQuickcharge);

		const auto casterActor = a_magicCaster->GetCasterAsActor();
		if (!casterActor) {
			return;
		}

		switch (currentSpell->GetAssociatedSkill()) {
		case RE::ActorValue::kIllusion:
			chargeTime += 0.25f;
			if (illusionQuickcharge && casterActor->HasPerk(illusionQuickcharge)) {
				chargeTime -= 0.5f;
			}
			break;
		case RE::ActorValue::kConjuration:
			chargeTime += 0.25f;
			if (conjurationQuickcharge && casterActor->HasPerk(conjurationQuickcharge)) {
				chargeTime -= 0.5f;
			}
			break;
		case RE::ActorValue::kDestruction:
			if (destructionQuickcharge && casterActor->HasPerk(destructionQuickcharge)) {
				chargeTime -= 0.5f;
			}
			break;
		case RE::ActorValue::kRestoration:
			if (restorationQuickcharge && casterActor->HasPerk(restorationQuickcharge)) {
				chargeTime -= 0.5f;
			}
			break;
		case RE::ActorValue::kAlteration:
			chargeTime += 0.25f;
			if (alterationQuickcharge && casterActor->HasPerk(alterationQuickcharge)) {
				chargeTime -= 0.5f;
			}
			break;
		default:
			return;
		}

		if (a_magicCaster->GetIsDualCasting()) {
			chargeTime += 0.25f;
		}

		const auto quickchargeAV = RE::LookupActorValueByName("Quickcharge");
		auto quickcharge = casterActor->GetActorValue(quickchargeAV) / 100.0f;
		if (quickcharge < 0.0f) {
			quickcharge = 0.0f;
		}
		else if (quickcharge > 0.75f) {
			quickcharge = 0.75f;
		}
		chargeTime *= 1.0f - quickcharge;
		a_magicCaster->castingTimer = chargeTime;
	}

	void Spells::Install()
	{
		ChargeTimeManager::GetSingleton()->Install();
	}
}
