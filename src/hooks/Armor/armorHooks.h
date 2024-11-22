#pragma once

#include "utilities/utilities.h"

namespace Hooks
{
	namespace Armor
	{
		void Install();

		class Manager : public Utilities::Singleton::ISingleton<Manager>
		{
		public:
			void Install();

		private:
			static float VisitArmorRating(RE::Character* a_target);

			inline static REL::Relocation<decltype(&VisitArmorRating)> _visitArmorRating;
		};
	}
}