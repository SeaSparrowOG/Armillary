#include "Hooks/hooks.h"

#include "ActiveEffect/ActiveEffectHooks.h"
#include "Conditions/ConditionHooks.h"
#include "MagicCaster/MagicCasterHooks.h"
#include "MagicTarget/MagicTargetHooks.h"
#include "PlayerCharacter/PlayerCharacterHooks.h"

namespace Hooks {
	bool Install() {
		SECTION_SEPARATOR;
		logger::info("Installing hooks..."sv);

		constexpr size_t trampolineSize = 14;
		SKSE::AllocTrampoline(trampolineSize);
		logger::info("Allocated {} bytes to the trampoline."sv, trampolineSize);

		bool result = true;

		if (!ActiveEffect::Install()) {
			result = false;
		}
		if (!Conditions::Install()) {
			result = false;
		}
		if (!MagicCaster::Install()) {
			result = false;
		}
		if (!MagicTarget::Install()) {
			result = false;
		}
		if (!PlayerCharacter::Install()) {
			result = false;
		}

		return result;
	}
}