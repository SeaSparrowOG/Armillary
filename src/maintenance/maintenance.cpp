#include "maintenance.h"

#include "data/modObjectManager.h"

namespace Maintenance
{
	void Manager::Initialize()
	{
		const auto avPerk = Data::ModObject<RE::BGSPerk>("ARM_Common_avManager");
		assert(avPerk);
		if (avPerk) {
			perksToRefresh.push_back(avPerk);
		}
	}
	void Manager::LoadMaintenance()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		assert(player);
		if (!player) {
			logger::critical("I mean, you crashed, right? Like how does that even happen?");
			return;
		}

		for (const auto perk : perksToRefresh) {
			if (!perk) {
				continue; //not possible to happen
			}

			if (player->HasPerk(perk)) {
				player->RemovePerk(perk);
			}
			player->AddPerk(perk);
		}
	}
}