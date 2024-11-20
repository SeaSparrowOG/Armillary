#include "conditions.h"

#include "data/modObjectManager.h"

void Conditions::Install()
{
	IsCurrentSpell::Manager::GetSingleton()->Install();
}

namespace Conditions::IsCurrentSpell
{
	void Manager::Install()
	{
		REL::Relocation<std::uintptr_t> IsCurrentSpellTarget{ REL::ID(21765), 0x45 };

		if (!REL::make_pattern<"E8">().match(IsCurrentSpellTarget.address())) {
			util::report_and_fail("Common Condition (IsCurrentSpell) Hooks: Failed to validate pattern, aborting launch.");
		}

		auto& trampoline = SKSE::GetTrampoline();
		_isCurrentSpell = trampoline.write_call<5>(IsCurrentSpellTarget.address(), &IsCurrentSpell);
	}

	unsigned long long Manager::IsCurrentSpell(RE::Actor* a_target, RE::MagicItem* a_spell)
	{
		const auto response = _isCurrentSpell(a_target, a_spell);

		const auto elementsArise = Data::ModObject<RE::BGSPerk>("ARM_Destruction_PRK_080_ElementsArise");
		assert(elementsArise);

		if (a_target && elementsArise && a_target->HasPerk(elementsArise)) {

			const auto dummyFire = Data::ModObject<RE::SpellItem>("ARM_Destruction_SPL_DummyFireCastDetector");
			const auto dummyFrost = Data::ModObject<RE::SpellItem>("ARM_Destruction_SPL_DummyFrostCastDetector");
			const auto dummyShock = Data::ModObject<RE::SpellItem>("ARM_Destruction_SPL_DummyShockCastDetector");
			assert(dummyFire && dummyFrost && dummyShock);

			if (dummyFire && dummyFrost && dummyShock) {
				const auto magicCasterLeft = a_target->GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
				const auto magicCasterRight = a_target->GetMagicCaster(RE::MagicSystem::CastingSource::kRightHand);
				const auto leftSpell = magicCasterLeft ? magicCasterLeft->currentSpell : nullptr;
				const auto rightSpell = magicCasterRight ? magicCasterRight->currentSpell : nullptr;

				if (a_spell == dummyFire) {
					if (leftSpell && a_target->IsCasting(leftSpell)) {
						for (const auto effect : leftSpell->effects) {
							if (!effect->baseEffect) {
								continue;
							}

							if (effect->baseEffect->HasKeywordString("MagicDamageFire")) {
								return 1;
							}
						}
					}

					if (rightSpell) {
						for (const auto effect : rightSpell->effects) {
							if (!effect->baseEffect) {
								continue;
							}

							if (effect->baseEffect->HasKeywordString("MagicDamageFire")) {
								return 1;
							}
						}
					}
				}
				//Frost
				else if (a_spell == dummyFrost) {
					if (leftSpell) {
						for (const auto effect : leftSpell->effects) {
							if (!effect->baseEffect) {
								continue;
							}

							if (effect->baseEffect->HasKeywordString("MagicDamageFrost")) {
								return 1;
							}
						}
					}

					if (rightSpell) {
						for (const auto effect : rightSpell->effects) {
							if (!effect->baseEffect) {
								continue;
							}

							if (effect->baseEffect->HasKeywordString("MagicDamageFrost")) {
								return 1;
							}
						}
					}
				}
				//Shock
				else if (a_spell == dummyShock) {
					if (leftSpell) {
						for (const auto effect : leftSpell->effects) {
							if (!effect->baseEffect) {
								continue;
							}

							if (effect->baseEffect->HasKeywordString("MagicDamageShock")) {
								return 1;
							}
						}
					}

					if (rightSpell) {
						for (const auto effect : rightSpell->effects) {
							if (!effect->baseEffect) {
								continue;
							}

							if (effect->baseEffect->HasKeywordString("MagicDamageShock")) {
								return 1;
							}
						}
					}
				}
			}
		}

		return response;
	}
}