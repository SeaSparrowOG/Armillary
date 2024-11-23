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
			static void HandleSkillIncrease(RE::PlayerCharacter* a_char, RE::ActorValue a_skill, float a_ammount, RE::TESForm* a_advanceObject,
				uint32_t a_advanceAction, bool a_useModifiers, bool a_hideNotification);
			static void HandleSkillIncrement(RE::PlayerCharacter* a_player, RE::ActorValue a_skill, unsigned int a_amount);

			inline static REL::Relocation<decltype(&VisitArmorRating)> _visitArmorRating;
			inline static REL::Relocation<decltype(&HandleSkillIncrease)> _handleSkillIncrease;
			inline static REL::Relocation<decltype(&HandleSkillIncrement)> _handleSkillIncrement;
		};
	}
}