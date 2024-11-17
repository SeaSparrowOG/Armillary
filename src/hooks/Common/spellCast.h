#pragma once

#include "utilities/utilities.h"

namespace Hooks
{
	namespace Spells
	{
		void Install();

		class ChargeTimeManager : public Utilities::Singleton::ISingleton<ChargeTimeManager> {
		public:
			void Install();

		private:
			static void SetCastingTimerForCharge(RE::MagicCaster* a_magicCaster);

			inline static REL::Relocation<decltype(&SetCastingTimerForCharge)> _setCastingTimerForCharge;
		};
	}
}