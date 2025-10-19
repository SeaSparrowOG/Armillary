#include "Hooks/hooks.h"

#include "MagicTargetHooks/MagicTargetHooks.h"

namespace Hooks {
	bool Install() {
		SECTION_SEPARATOR;
		logger::info("Installing hooks..."sv);

		bool result = true;
		result &= MagicTargetHooks::Install();
		return result;
	}
}