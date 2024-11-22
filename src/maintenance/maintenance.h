#pragma once

#include "utilities/utilities.h"

namespace Maintenance
{
	class Manager : public Utilities::Singleton::ISingleton<Manager>
	{
	public:
		void Initialize();
		void LoadMaintenance();

	private:
		std::vector<RE::BGSPerk*> perksToRefresh;
		std::vector<RE::SpellItem*> spellsToRefresh;
	};
}