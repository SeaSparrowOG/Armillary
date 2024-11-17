#include "runtimePatcher/runtimePatcher.h"

#include "runtimePatcher/Common/weaponPatcher.h"

namespace RuntimePatcher
{
	void PatchAllForms() {
		logger::info("Starting runtime patcher:");
		Weapons::Patch();
	}
}