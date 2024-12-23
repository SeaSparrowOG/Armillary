#include "runtimePatcher/runtimePatcher.h"

#include "runtimePatcher/Common/destructionPatcher.h"
#include "runtimePatcher/Common/weaponPatcher.h"
#include "runtimePatcher/Common/armorPatcher.h"

namespace RuntimePatcher
{
	void PatchAllForms() {
		logger::info("Starting runtime patcher:");
		Armor::Patch();
		Weapons::Patch();
		CloakSpells::Patch();
	}
}