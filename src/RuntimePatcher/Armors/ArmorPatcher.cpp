#include "ArmorPatcher.h"

#include "Data/ModObjectManager.h"

namespace RuntimePatcher::Armors
{
	bool PatchArmorForms() {
		logger::info("  >Patching Armor forms..."sv);

		auto* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			logger::error("    Failed to fetch the game's Data Handler."sv);
			return false;
		}

		auto& armors = dataHandler->GetFormArray<RE::TESObjectARMO>();
		if (armors.empty()) {
			logger::warn("    Weapons array is empty. This is fatal, but I'll allow it."sv);
			return true;
		}

		auto* heavyCuirassKeyword = Data::ModObject<RE::BGSKeyword>(Data::HeavyArmor_HeavyCuirass);
		if (!heavyCuirassKeyword) {
			logger::error("    Failed to query {} from the mod's mod objects. This indicates a bad installation."sv, Data::HeavyArmor_HeavyCuirass);
			return false;
		}

		bool foundAll = true;
		auto* heavyArmor = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ArmorHeavy"sv);
		auto* cuirass = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ArmorCuirass"sv);
		if (!heavyArmor) {
			logger::error("    Failed to query the game's Heavy Armor keyword. This indicates a corrupted Skyrim.esm."sv);
			foundAll = false;
		}
		if (!cuirass) {
			logger::error("    Failed to query the game's Cuirass armor keyword.This indicates a corrupted Skyrim.esm"sv);
			foundAll = false;
		}
		if (!foundAll) {
			return false;
		}

		bool result = true;
		for (auto* armor : armors) {
			if (!armor) {
				continue;
			}

			if (armor->HasKeyword(heavyArmor)) {
				if (armor->HasKeyword(cuirass)) {
					armor->AddKeyword(heavyCuirassKeyword);
				}
			}
		}
		return result;
	}
}