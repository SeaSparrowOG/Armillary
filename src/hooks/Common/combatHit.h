#pragma once

#include "utilities/utilities.h"

namespace Hooks
{
	namespace CombatHit
	{
		void Install();

		class CombatHitManager : public Utilities::Singleton::ISingleton<CombatHitManager> {
		public:
			void Install();

		private:
			static void CombatHit(RE::Actor* a_target, RE::HitData* a_hitData);

			inline static REL::Relocation<decltype(&CombatHit)> _combatHit;
		};
	}
}