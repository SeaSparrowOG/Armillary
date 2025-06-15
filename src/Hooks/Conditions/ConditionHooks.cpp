#include "ConditionHooks.h"

#include "ConditionManager/ConditionManager.h"
#include "RE/Offset.h"

namespace Hooks::Conditions
{
	bool Install() {
		logger::info("  >Installing condition-related hooks..."sv);
		return GetActorItemCountHook::Install();
	}

	bool GetActorItemCountHook::Install() {
		logger::info("    >Installing GetItemCount hook."sv);
		REL::Relocation<std::uintptr_t> target{ RE::Offset::TESObjectREFR::GetItemCount, 0xAE };
		if (!REL::make_pattern<"E8">().match(target.address())) {
			logger::critical("      >Failed to match expected pattern (E8)."sv);
			return false;
		}
		auto& trampoline = SKSE::GetTrampoline();
		_getItemCount = trampoline.write_call<5>(target.address(), GetItemCount);
		return true;
	}

	inline uint32_t GetActorItemCountHook::GetItemCount(RE::TESBoundObject* a_item, 
		RE::TESObjectREFR* a_containerRef) 
	{
		uint32_t response = 0;
		auto* manager = ConditionManager::ConditionManager::GetSingleton();
		if (manager && manager->SubstituteItemCount(a_item, a_containerRef, response)) {
			return response;
		}
		return _getItemCount(a_item, a_containerRef);
	}
}