#pragma once

#include "utilities/utilities.h"

namespace Hooks
{
	namespace Conjuration
	{
		void Install();

		class Manager : public Utilities::Singleton::ISingleton<Manager> {
		public:
			void Install();

		private:
			static void OnUpdate(RE::ReanimateEffect* a_effect, float a_delta);
			static void HandleEvent(RE::SummonCreatureEffect* a_effect, const RE::BSFixedString& a_eventName);
			static RE::EffectSetting* GetBaseObject(RE::ActiveEffect* a_effect);

			inline static REL::Relocation<decltype(&OnUpdate)> _onUpdate;
			inline static REL::Relocation<decltype(&HandleEvent)> _handleEvent;
			inline static REL::Relocation<decltype(&GetBaseObject)> _getBaseObject;
		};
	}
}