#include "armorPatcher.h"

#include "data/modObjectManager.h"

void RuntimePatcher::Armor::Patch()
{
	logger::info("  Starting armor patcher...");
	const auto dataHandler = RE::TESDataHandler::GetSingleton();
	if (!dataHandler) {
		logger::error("    >Failed to get data handler!");
		return;
	}

	const auto heavyCuirass = Data::ModObject<RE::BGSKeyword>("ARM_Armor_KWD_HeavyCuirass");
	const auto lightCuirass = Data::ModObject<RE::BGSKeyword>("ARM_Armor_KWD_LightCuirass");
	const auto clothCuirass = Data::ModObject<RE::BGSKeyword>("ARM_Armor_KWD_ClothingCuirass");
	assert(heavyCuirass && lightCuirass && clothCuirass);
	if (!heavyCuirass || !lightCuirass || !clothCuirass) {
		logger::error("    >Failed to get default objects!");
		return;
	}

	const auto cuirass = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ArmorCuirass");
	const auto heavy = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ArmorHeavy");
	const auto light = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ArmorLight");
	const auto cloth = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ArmorClothing");
	assert(cuirass && heavy && light && cloth);
	if (!cuirass || !heavy || !light || !cloth) {
		logger::error("    >Failed to get base game objects!");
		return;
	}

	size_t patchedWeapons = 0;
	auto& armorArray = dataHandler->GetFormArray<RE::TESObjectARMO>();
	for (auto* armor : armorArray) {
		if (!armor) {
			continue;
		}

		if (armor->HasKeyword(heavy) && armor->HasKeyword(cuirass)) {
			armor->AddKeyword(heavyCuirass);
		}  
		if (armor->HasKeyword(light) && armor->HasKeyword(cuirass)) {
			armor->AddKeyword(lightCuirass);
		}
		if (armor->HasKeyword(cloth) && armor->HasKeyword(cuirass)) {
			armor->AddKeyword(clothCuirass);
		}
		patchedWeapons++;
	}
	logger::info("    >Finished patching {} armors.", patchedWeapons);
}
