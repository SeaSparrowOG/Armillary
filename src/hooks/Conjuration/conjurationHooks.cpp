#include "conjurationHooks.h"

#include "data/modObjectManager.h"

namespace Hooks::Conjuration
{
	void Install()
	{
		Manager::GetSingleton()->Install();
	}

	void Manager::Install()
	{
		REL::Relocation<std::uintptr_t> vtbl{ RE::Character::VTABLE[0] };
		_drawWeaponMagicHands = vtbl.write_vfunc(0xA6, DrawWeaponMagicHands);
	}

	void Manager::DrawWeaponMagicHands(RE::Character* a_this, bool a_draw)
	{
		if (a_this && !a_draw && a_this->IsCommandedActor() && a_this->GetCommandingActor().get()) {
			if ((a_this->GetActorBase() == Data::ModObject<RE::TESNPC>("ARM_Conjuration_NPC_InvisibleNPC") ||
				a_this->GetActorBase() == Data::ModObject<RE::TESNPC>("ARM_Conjuration_NPC_InvisibleNPCEdgeOfOblivion"))) {
				if (const auto weaponState = a_this->GetCommandingActor()->GetWeaponState();
					weaponState == RE::WEAPON_STATE::kDrawing
					|| weaponState == RE::WEAPON_STATE::kWantToDraw
					|| weaponState == RE::WEAPON_STATE::kDrawn) {
					return;
				}
				else {
					a_this->KillImmediate();
				}
			}
		}
		_drawWeaponMagicHands(a_this, a_draw);
	}
}