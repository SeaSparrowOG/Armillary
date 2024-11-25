#pragma once

#include "utilities/utilities.h"

namespace Events
{
	namespace Death
	{
		class Listener : public Utilities::Singleton::EventClass<Listener, RE::TESDeathEvent> {
		private:
			RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent* a_event, RE::BSTEventSource<RE::TESDeathEvent>*) override;
		};
	}
}