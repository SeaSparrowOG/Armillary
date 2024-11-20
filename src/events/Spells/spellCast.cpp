#include "spellCast.h"

namespace Events::SpellCast
{

	RE::BSEventNotifyControl Listener::ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*)
	{
		using control = RE::BSEventNotifyControl;
		if (!a_event) {
			return control::kContinue;
		}

		const auto effect = RE::TESForm::LookupByID<RE::Effect>(a_event->magicEffect);
		const auto casterRef = a_event->caster.get();
		const auto eventCaster = casterRef ? casterRef->As<RE::Actor>() : nullptr;
		if (!effect || !eventCaster) {
			return control::kContinue;
		}

		return control::kContinue;
	}
}