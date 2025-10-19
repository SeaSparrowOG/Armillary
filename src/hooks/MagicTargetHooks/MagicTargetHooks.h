#pragma once

namespace Hooks
{
	namespace MagicTargetHooks
	{
		bool Install();

		struct MagicTarget_CheckAbsorbHook
		{
			inline static bool InstallCheckAbsorbHook();

			inline static bool Thunk(RE::MagicTarget* a_this, 
				RE::Actor* a_actor, 
				RE::MagicItem* a_spell, 
				const RE::Effect* a_effect);

			inline static REL::Relocation<decltype(Thunk)> _func;
		};
	}
}