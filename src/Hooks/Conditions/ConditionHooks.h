#pragma once

namespace Hooks
{
	namespace Conditions
	{
		bool Install();

		struct GetActorItemCountHook
		{
			inline static bool Install();
			inline static uint32_t GetItemCount(RE::TESBoundObject* a_item, RE::TESObjectREFR* a_containerRef);
			inline static REL::Relocation<decltype(GetItemCount)> _getItemCount;
		};
	}
}