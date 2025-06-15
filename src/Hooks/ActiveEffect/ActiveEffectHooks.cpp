#include "ActiveEffectHooks.h"

#include "BoundEffectManager/BoundEffectManager.h"

namespace Hooks::ActiveEffect
{
	bool Install() {
		logger::info("  >Installing Active Effect Hooks..."sv);
		ActiveEffectUpdateHook::Install();
		return true;
	}

	void ActiveEffectUpdateHook::Install() {
		logger::info("    >Installing script effect update..."sv);
		REL::Relocation<std::uintptr_t> ScriptEffectVTBL{ RE::ScriptEffect::VTABLE[0] };
		_updateScripted = ScriptEffectVTBL.write_vfunc(0x4, &UpdateScripted);

		logger::info("    >Installing dual value modifier effect update..."sv);
		REL::Relocation<std::uintptr_t> DualValueModifierVTBL{ RE::DualValueModifierEffect::VTABLE[0] };
		_updateDualValueModifier = DualValueModifierVTBL.write_vfunc(0x4, &UpdateDualValueModifier);

		logger::info("    >Installing value modifier effect update..."sv);
		REL::Relocation<std::uintptr_t> ValueModifierVTBL{ RE::ValueModifierEffect::VTABLE[0] };
		_updateValueModifier = ValueModifierVTBL.write_vfunc(0x4, &UpdateValueModifier);

		logger::info("    >Installing bound item effect update..."sv);
		REL::Relocation<std::uintptr_t> BoundItemEffectVTBL{ RE::BoundItemEffect::VTABLE[0] };
		_updateBoundWeapon = BoundItemEffectVTBL.write_vfunc(0x4, &UpdateBoundWeapon);

		logger::info("    >Installing reanimation effect update..."sv);
		REL::Relocation<std::uintptr_t> ReanimateEffectVTBL{ RE::ReanimateEffect::VTABLE[0] };
		_updateReanimated = ReanimateEffectVTBL.write_vfunc(0x4, &UpdateReanimated);

		logger::info("    >Installing script summon update..."sv);
		REL::Relocation<std::uintptr_t> SummonCreatureEffectVTBL{ RE::SummonCreatureEffect::VTABLE[0] };
		_updateSummoned = SummonCreatureEffectVTBL.write_vfunc(0x4, &UpdateSummoned);
	}

	inline void ActiveEffectUpdateHook::UpdateScripted(RE::ScriptEffect* a_this, float a_delta) {
		_updateScripted(a_this, a_delta);
		auto* manager = BoundEffectManager::BoundEffectManager::GetSingleton();
		if (manager && manager->IsBoundEffect(a_this)) {
			a_this->elapsedSeconds -= a_delta;
		}
	}

	inline void ActiveEffectUpdateHook::UpdateDualValueModifier(RE::DualValueModifierEffect* a_this, float a_delta) {
		_updateDualValueModifier(a_this, a_delta);
		auto* manager = BoundEffectManager::BoundEffectManager::GetSingleton();
		if (manager && manager->IsBoundEffect(a_this)) {
			a_this->elapsedSeconds -= a_delta;
		}
	}

	inline void ActiveEffectUpdateHook::UpdateValueModifier(RE::ValueModifierEffect* a_this, float a_delta) {
		_updateValueModifier(a_this, a_delta);
		auto* manager = BoundEffectManager::BoundEffectManager::GetSingleton();
		if (manager && manager->IsBoundEffect(a_this)) {
			a_this->elapsedSeconds -= a_delta;
		}
	}

	inline void ActiveEffectUpdateHook::UpdateBoundWeapon(RE::BoundItemEffect* a_this, float a_delta) {
		_updateBoundWeapon(a_this, a_delta);
		auto* manager = BoundEffectManager::BoundEffectManager::GetSingleton();
		if (manager && manager->IsBoundEffect(a_this)) {
			a_this->elapsedSeconds -= a_delta;
		}
	}

	inline void ActiveEffectUpdateHook::UpdateReanimated(RE::ReanimateEffect* a_this, float a_delta) {
		_updateReanimated(a_this, a_delta);
		auto* manager = BoundEffectManager::BoundEffectManager::GetSingleton();
		if (manager && manager->IsBoundEffect(a_this)) {
			a_this->elapsedSeconds -= a_delta;
		}
	}

	void ActiveEffectUpdateHook::UpdateSummoned(RE::SummonCreatureEffect* a_this, float a_delta) {
		_updateSummoned(a_this, a_delta);
		auto* manager = BoundEffectManager::BoundEffectManager::GetSingleton();
		if (manager && manager->IsBoundEffect(a_this)) {
			a_this->elapsedSeconds -= a_delta;
		}
	}
}