#pragma once

#include "utilities/utilities.h"

namespace Events
{
	namespace SpellCast
	{
		class Listener : public Utilities::Singleton::EventClass<Listener, RE::TESMagicEffectApplyEvent> {
		private:
			RE::BSEventNotifyControl ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*) override;
		};
	}
}