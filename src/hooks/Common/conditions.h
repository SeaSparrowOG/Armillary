#pragma once

#include "utilities/utilities.h"

namespace Conditions
{
	void Install();

	class Manager : public Utilities::Singleton::ISingleton<Manager> {
	public:
		void Install();

		bool IsActorVulnerable(RE::Actor* a_target);

	private:
		static unsigned long long HasSpell(RE::Actor* a_target, RE::MagicItem* a_spell);

		inline static REL::Relocation<decltype(&HasSpell)> _hasSpell;
	};
}