#pragma once

#include "utilities/utilities.h"

namespace Hooks
{
	namespace Alchemy
	{
		void Install();

		class CreatedObjectMonitor : public Utilities::Singleton::ISingleton<CreatedObjectMonitor> {
		public:
			void Install();

		private:
			float currentBoostPct{ 1.0f };

			static void CreateItem(RE::TESDataHandler* a_dataHandler, RE::AlchemyItem* a_alchemyItem);
			static bool SelectItem(const RE::CraftingSubMenus::AlchemyMenu& a_menu, const uint64_t& a_param2);

			inline static REL::Relocation<decltype(&SelectItem)> _selectItem;
			inline static REL::Relocation<decltype(&CreateItem)> _createItem;
		};
	}
}