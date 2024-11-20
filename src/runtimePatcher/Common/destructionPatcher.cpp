#include "destructionPatcher.h"

#include "data/modObjectManager.h"

namespace RuntimePatcher::CloakSpells
{
	void Patch() {
		logger::info("  Starting Cloak Spell patcher...");

		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			logger::error("    >Failed to get data handler!");
			return;
		}

		const auto magicCloakFire = Data::ModObject<RE::BGSKeyword>("ARM_Destruction_KWD_MagicCloakFire");
		const auto magicCloakFrost = Data::ModObject<RE::BGSKeyword>("ARM_Destruction_KWD_MagicCloakFrost");
		const auto magicCloakShock = Data::ModObject<RE::BGSKeyword>("ARM_Destruction_KWD_MagicCloakShock");

		size_t patchedSpells = 0;
		auto& spellArray = dataHandler->GetFormArray<RE::SpellItem>();
		for (auto* spell : spellArray) {
			if (!spell) {
				continue;
			}

			if (spell->effects.empty() || !spell->effects.front()->baseEffect) {
				continue;
			}

			bool patched = false;
			for (const auto effect : spell->effects) {
				if (!effect->baseEffect) {
					continue;
				}

				if (!effect->baseEffect->HasArchetype(RE::EffectSetting::Archetype::kCloak)) {
					continue;
				}

				const auto originalBase = effect->baseEffect;
				if (const auto associatedForm = originalBase->data.associatedForm; associatedForm && associatedForm->As<RE::SpellItem>()) {
					const auto associatedSpell = associatedForm->As<RE::SpellItem>();
					for (const auto secondary : associatedSpell->effects) {
						if (!secondary->baseEffect) {
							continue;
						}

						if (secondary->baseEffect->HasKeywordString("MagicDamageFire")) {
							patched = true;
							if (!originalBase->HasKeyword(magicCloakFire)) {
								originalBase->AddKeyword(magicCloakFire);
							}
						}
						if (secondary->baseEffect->HasKeywordString("MagicDamageFrost")) {
							patched = true;
							if (!originalBase->HasKeyword(magicCloakFrost)) {
								originalBase->AddKeyword(magicCloakFrost);
							}
						}
						if (secondary->baseEffect->HasKeywordString("MagicDamageShock")) {
							patched = true;
							if (!originalBase->HasKeyword(magicCloakShock)) {
								originalBase->AddKeyword(magicCloakShock);
							}
						}
					}
				}
			}
			if (patched) {
				logger::debug("Patched {}", spell->GetName());
			}
			patchedSpells = patched ? patchedSpells + 1 : patchedSpells;
		}
		logger::info("    >Finished patching {} cloak spells.", patchedSpells);
	}
}