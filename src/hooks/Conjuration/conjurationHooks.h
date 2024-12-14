#pragma once

#include "utilities/utilities.h"

namespace Hooks
{
	namespace Conjuration
	{
		void Install();

		class Manager : public Utilities::Singleton::ISingleton<Manager>
		{
		public:
			void Install();

		private:
			static void DrawWeaponMagicHands(RE::Character* a_this, bool a_draw);

			inline static REL::Relocation<decltype(DrawWeaponMagicHands)> _drawWeaponMagicHands;
		};
	}
}