#pragma once

namespace Hooks
{
	namespace MagicTargetHooks
	{
		bool Install();

		struct MagicTarget_CheckAbsorbHook_PC
		{
			inline static bool InstallCheckAbsorbHook_PC();

			inline static bool Thunk_CheckAbsorb_PC(RE::MagicTarget* a_this,
				RE::Actor* a_actor,
				RE::MagicItem* a_spell,
				const RE::Effect* a_effect);

			inline static REL::Relocation<decltype(Thunk_CheckAbsorb_PC)> _func_CheckAbsorb_PC;
		};

		struct MagicTarget_CheckAbsorbHook_NPC
		{
			inline static bool InstallCheckAbsorbHook_NPC();

			inline static bool Thunk_CheckAbsorb_NPC(RE::MagicTarget* a_this,
				RE::Actor* a_actor,
				RE::MagicItem* a_spell,
				const RE::Effect* a_effect);

			inline static REL::Relocation<decltype(Thunk_CheckAbsorb_NPC)> _func_CheckAbsorb_NPC;
		};
	}
}