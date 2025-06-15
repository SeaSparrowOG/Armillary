#pragma once

namespace Hooks
{
	namespace ActiveEffect
	{
		bool Install();

		// Allows changing the elapsed time in Active Effects
		struct ActiveEffectUpdateHook
		{
			static void Install();

			inline static void UpdateDualValueModifier(RE::DualValueModifierEffect* a_this, float a_delta);
			inline static REL::Relocation<decltype(UpdateDualValueModifier)> _updateDualValueModifier;

			inline static void UpdateValueModifier(RE::ValueModifierEffect* a_this, float a_delta);
			inline static REL::Relocation<decltype(UpdateValueModifier)> _updateValueModifier;

			inline static void UpdateBoundWeapon(RE::BoundItemEffect* a_this, float a_delta);
			inline static REL::Relocation<decltype(UpdateBoundWeapon)> _updateBoundWeapon;

			inline static void UpdateReanimated(RE::ReanimateEffect* a_this, float a_delta);
			inline static REL::Relocation<decltype(UpdateReanimated)> _updateReanimated;

			inline static void UpdateSummoned(RE::SummonCreatureEffect* a_this, float a_delta);
			inline static REL::Relocation<decltype(UpdateSummoned)> _updateSummoned;

			inline static void UpdateScripted(RE::ScriptEffect* a_this, float a_delta);
			inline static REL::Relocation<decltype(UpdateScripted)> _updateScripted;
		};
	}
}