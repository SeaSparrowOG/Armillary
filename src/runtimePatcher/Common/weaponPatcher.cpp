#include "weaponPatcher.h"

namespace RuntimePatcher::Weapons
{
	void Patch()
	{
		logger::info("  Starting weapon patcher...");
		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			logger::error("    >Failed to get data handler!");
			return;
		}

		size_t patchedWeapons = 0;
		auto& weaponArray = dataHandler->GetFormArray<RE::TESObjectWEAP>();
		for (auto* weapon : weaponArray) {
			if (!weapon) {
				continue;
			}

			weapon->criticalData.prcntMult = 0.0f;
			weapon->weaponData.staggerValue = 0.0f;
			patchedWeapons++;
		}
		logger::info("    >Finished patching {} weapons.", patchedWeapons);
	}
}
