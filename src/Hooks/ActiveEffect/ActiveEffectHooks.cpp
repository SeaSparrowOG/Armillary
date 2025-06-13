#include "ActiveEffectHooks.h"

#include "BoundEffectManager/BoundEffectManager.h"

namespace Hooks::ActiveEffect
{
	bool Install() {
		logger::info("  >Installing Active Effect Hooks..."sv);
		SummonCreatureUpdateHook::Install();
		return true;
	}

	void SummonCreatureUpdateHook::Install() {
		REL::Relocation<std::uintptr_t> ActorMagicCasterVTBL{ RE::SummonCreatureEffect::VTABLE[0] };
		_update = ActorMagicCasterVTBL.write_vfunc(0x4, &Update);
	}

	void SummonCreatureUpdateHook::Update(RE::SummonCreatureEffect* a_this, float a_delta) {
		_update(a_this, a_delta);
		auto* manager = BoundEffectManager::BoundEffectManager::GetSingleton();
		if (manager && manager->IsBoundEffect(a_this)) {
			a_this->elapsedSeconds -= a_delta;
		}
	}
}