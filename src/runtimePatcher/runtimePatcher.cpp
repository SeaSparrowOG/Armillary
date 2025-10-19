#include "RuntimePatcher.h"

#include "RuntimePatcher/Armors/ArmorPatcher.h"

namespace RuntimePatcher
{
	bool PatchGameForms() {
		logger::info("Beginning runtime patching..."sv);
		bool result = true;
		result &= Armors::PatchArmorForms();
		return result;
	}
}