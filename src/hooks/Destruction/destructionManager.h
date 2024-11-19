#pragma once

#include "utilities/utilities.h"

namespace Hooks 
{
	namespace Destruction
	{
		void Install();

		class DestructionManager : public Utilities::Singleton::ISingleton<DestructionManager> {
		public:
			void Install();

		private:
			static bool Explosion__ProcessTargets(RE::Projectile* a_this, RE::TESObjectREFR* a_hit, void* a3, void* a4, int a5, void* a6, void* a7);

			inline static REL::Relocation<decltype(&Explosion__ProcessTargets)> _explosionProcessTargets;
		};
	}
}