#include "conjurationHooks.h"

#include "data/modObjectManager.h"

namespace Hooks::Conjuration
{
	void Install()
	{
		Manager::GetSingleton()->Install();
	}

	void Manager::Install()
	{
		REL::Relocation<std::uintptr_t> reanimateVTBL{ RE::ReanimateEffect::VTABLE[0] };
		REL::Relocation<std::uintptr_t> summonVTBL{ RE::SummonCreatureEffect::VTABLE[0] };

		_onUpdate = reanimateVTBL.write_vfunc(0x4, OnUpdate);
		_handleEvent = summonVTBL.write_vfunc(0xD, HandleEvent);

		REL::Relocation<std::uintptr_t> getBaseObjectTarget{ REL::ID(34229), 0x48 };

		if (!REL::make_pattern<"E8">().match(getBaseObjectTarget.address())) {
			util::report_and_fail("Common Combat Hooks: Failed to validate pattern, aborting launch.");
		}

		auto& trampoline = SKSE::GetTrampoline();
		_getBaseObject = trampoline.write_call<5>(getBaseObjectTarget.address(), &GetBaseObject);
	}

	void Manager::OnUpdate(RE::ReanimateEffect* a_effect, float a_delta)
	{
		if (a_effect) {
			const auto remainder = a_effect->duration - a_delta - a_effect->elapsedSeconds;
			if (const auto zombie = a_effect->commandedActor.get().get(); zombie && zombie->GetCommandingActor() && remainder <= 0.0f) {
				const auto eternalServitudePerk = Data::ModObject<RE::BGSPerk>("ARM_Conjuration_PRK_075_EternalServitude");
				const auto eternalServitudeSpell = Data::ModObject<RE::SpellItem>("ARM_Conjuration_SPL_EternalServitudeDamage");
				//assert(eternalServitudePerk);

				const auto commander = zombie->GetCommandingActor();
				bool isHostileToCommander = commander->IsHostileToActor(zombie);
				if (!isHostileToCommander
					&& eternalServitudePerk 
					&& eternalServitudeSpell
					&& commander->HasPerk(eternalServitudePerk)) {
					zombie->AddSpell(eternalServitudeSpell);
					a_effect->elapsedSeconds = 0.0f;
				}
			}
		}
		_onUpdate(a_effect, a_delta);
	}

	void Manager::HandleEvent(RE::SummonCreatureEffect* a_effect, const RE::BSFixedString& a_eventName)
	{
		_handleEvent(a_effect, a_eventName);
		if (a_eventName == "PlaceActor") {
			const auto summonSpell = a_effect ? a_effect->spell : nullptr;
			const auto summoner = a_effect ? a_effect->caster.get().get() : nullptr;
			const auto summon = a_effect ? a_effect->commandedActor.get().get() : nullptr;
			const auto summonRace = summon ? summon->GetRace() : nullptr;
			const auto summonerCaster = summon ? summon->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant) : nullptr;
			const auto summonAtronach = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ActorTypeDaedra");
			const auto summonFire = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicSummonFire");
			const auto summonFrost = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicSummonFrost");
			const auto summonShock = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("MagicSummonShock");
			bool fire = false;
			bool frost = false;
			bool shock = false;

			if (summonSpell && !summonSpell->effects.empty()) {
				for (const auto effect : summonSpell->effects) {
					if (!effect->baseEffect) {
						continue;
					}

					const auto baseEffect = effect->baseEffect;
					if (baseEffect->HasKeyword(summonFire)) {
						fire = true;
					}
					if (baseEffect->HasKeyword(summonFrost)) {
						frost = true;
					}
					if (baseEffect->HasKeyword(summonShock)) {
						shock = true;
					}
				}
			}

			if (fire || frost || shock) {
				if (summoner && summonerCaster && summon && summonAtronach && summonFire && summonFrost && summonShock && summonRace && summonRace->HasKeyword(summonAtronach)) {
					const auto gatekeeperPerk = Data::ModObject<RE::BGSPerk>("ARM_Conjuration_PRK_050_Gatekeeper");
					const auto gatekeeperFire = Data::ModObject<RE::SpellItem>("ARM_Conjuration_SPL_GatekeeperFire");
					const auto gatekeeperFrost = Data::ModObject<RE::SpellItem>("ARM_Conjuration_SPL_GatekeeperFrost");
					const auto gatekeeperShock = Data::ModObject<RE::SpellItem>("ARM_Conjuration_SPL_GatekeeperShock");
					assert(gatekeeperFire && gatekeeperFrost && gatekeeperShock && gatekeeperPerk);

					if (gatekeeperFire && gatekeeperFrost && gatekeeperShock && gatekeeperPerk && summoner->HasPerk(gatekeeperPerk)) {
						if (fire) {
							summonerCaster->CastSpellImmediate(gatekeeperFire, false, summon, 1.0f, false, 0.0f, summon);
						}
						if (frost) {
							summonerCaster->CastSpellImmediate(gatekeeperFrost, false, summon, 1.0f, false, 0.0f, summon);
						}
						if (shock) {
							summonerCaster->CastSpellImmediate(gatekeeperShock, false, summon, 1.0f, false, 0.0f, summon);
						}
					}
				}
			}
		}
	}

	RE::EffectSetting* Manager::GetBaseObject(RE::ActiveEffect* a_effect)
	{
		if (a_effect && 
			(a_effect->flags.any(RE::ActiveEffect::Flag::kDual)
				|| a_effect->castingSource == RE::MagicSystem::CastingSource::kInstant)) {
			a_effect->castingSource = RE::MagicSystem::CastingSource::kRightHand;
		}
		return _getBaseObject(a_effect);
	}
}