#pragma once

#include "utilities/utilities.h"

namespace Conditions
{
	void Install();
	namespace IsCurrentSpell
	{
		class Manager : public Utilities::Singleton::ISingleton<Manager> {
		public:
			void Install();

		private:
			static unsigned long long IsCurrentSpell(RE::Actor* a_target, RE::MagicItem* a_spell);

			inline static REL::Relocation<decltype(&IsCurrentSpell)> _isCurrentSpell;
		};
	}
}