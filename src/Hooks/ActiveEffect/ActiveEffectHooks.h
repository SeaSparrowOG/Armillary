#pragma once

namespace Hooks
{
	namespace ActiveEffect
	{
		bool Install();

		// Allows changing the elapsed time in Active Effects
		struct SummonCreatureUpdateHook
		{
			static void Install();
			inline static void Update(RE::SummonCreatureEffect* a_this, float a_delta);
			inline static REL::Relocation<decltype(Update)> _update;
		};
	}
}