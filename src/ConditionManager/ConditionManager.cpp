#include "ConditionManager.h"

#include "Data/ModObjectManager.h"

namespace ConditionManager
{
	bool Initialize() {
		SECTION_SEPARATOR;
		logger::info("Initializing the Condition Manager..."sv);
		auto* manager = ConditionManager::GetSingleton();
		if (!manager) {
			logger::info("  >Failed to get internal singleton."sv);
			return false;
		}
		return manager->Initialize();
	}

	bool ConditionManager::Initialize() {
		bool nominal = true;
		logger::info("  >Caching mod objects..."sv);

		conjuredCount = Data::ModObject<RE::TESBoundObject>(conjuredCountID);
		if (!conjuredCount) {
			logger::critical("    >Failed to cache {}", conjuredCountID);
			nominal = false;
		}
		totalCount = Data::ModObject<RE::TESBoundObject>(totalCountID);
		if (!totalCount) {
			logger::critical("    >Failed to cache {}", totalCountID);
			nominal = false;
		}
		fireCount = Data::ModObject<RE::TESBoundObject>(fireCountID);
		if (!fireCount) {
			logger::critical("    >Failed to cache {}", fireCountID);
			nominal = false;
		}
		frostCount = Data::ModObject<RE::TESBoundObject>(frostCountID);
		if (!frostCount) {
			logger::critical("    >Failed to cache {}", frostCountID);
			nominal = false;
		}
		shockCount = Data::ModObject<RE::TESBoundObject>(shockCountID);
		if (!shockCount) {
			logger::critical("    >Failed to cache {}", shockCountID);
			nominal = false;
		}
		untypedCount = Data::ModObject<RE::TESBoundObject>(untypedCountID);
		if (!untypedCount) {
			logger::critical("    >Failed to cache {}", untypedCountID);
			nominal = false;
		}
		conjuredUndeadCount = Data::ModObject<RE::TESBoundObject>(conjuredUndeadCountID);
		if (!conjuredUndeadCount) {
			logger::critical("    >Failed to cache {}", conjuredUndeadCountID);
			nominal = false;
		}
		reanimatedCount = Data::ModObject<RE::TESBoundObject>(reanimatedCountID);
		if (!reanimatedCount) {
			logger::critical("    >Failed to cache {}", reanimatedCountID);
			nominal = false;
		}

		if (!nominal) {
			return false;
		}

		logger::info("  >Caching game forms..."sv);
		actorTypeUndead = RE::TESForm::LookupByEditorID<RE::BGSKeyword>("ActorTypeUndead"sv);
		if (!actorTypeUndead) {
			logger::critical("    >Failed to cache the game's ActorTypeUndead keyword."sv);
			nominal = false;
		}

		if (!nominal) {
			return false;
		}

		logger::info("Initialized successfully."sv);
		return true;
	}

	bool ConditionManager::SubstituteItemCount(RE::TESBoundObject* a_obj,
		RE::TESObjectREFR* a_ref,
		int32_t& a_out)
	{
		auto* asActor = a_ref ? a_ref->As<RE::Actor>() : nullptr;
		auto* asMiddleHigh = asActor ? asActor->GetMiddleHighProcess() : nullptr;
		if (!asMiddleHigh || !a_obj) {
			return false;
		}

		auto& commandedActors = asMiddleHigh->commandedActors;
		if (commandedActors.empty()) {
			return false;
		}

		if (conjuredCount == a_obj) {
			for (const auto actorData : commandedActors) {
				auto* actor = actorData.commandedActor ? actorData.commandedActor.get().get() : nullptr;
				if (!actor) {
					continue;
				}
				a_out += actor->IsSummoned() ? 1 : 0;
			}
		}
		else if (totalCount == a_obj) {
			for (const auto actorData : commandedActors) {
				auto* actor = actorData.commandedActor ? actorData.commandedActor.get().get() : nullptr;
				if (!actor) {
					continue;
				}
				a_out += actor->IsSummoned() || actor->IsReanimated() ? 1 : 0;
			}
		}
		else if (fireCount == a_obj) {
			for (const auto actorData : commandedActors) {
				auto* actor = actorData.commandedActor ? actorData.commandedActor.get().get() : nullptr;
				auto* avOwner = actor ? actor->As<RE::ActorValueOwner>() : nullptr;
				if (!avOwner || !(actor->IsSummoned() || actor->IsReanimated())) {
					continue;
				}
				a_out += avOwner->GetActorValue(RE::ActorValue::kResistFire) >= 100.0f ? 1 : 0;
			}
		}
		else if (frostCount == a_obj) {
			for (const auto actorData : commandedActors) {
				auto* actor = actorData.commandedActor ? actorData.commandedActor.get().get() : nullptr;
				auto* avOwner = actor ? actor->As<RE::ActorValueOwner>() : nullptr;
				if (!avOwner || !(actor->IsSummoned() || actor->IsReanimated())) {
					continue;
				}
				a_out += avOwner->GetActorValue(RE::ActorValue::kResistFrost) >= 100.0f ? 1 : 0;
			}
		}
		else if (shockCount == a_obj) {
			for (const auto actorData : commandedActors) {
				auto* actor = actorData.commandedActor ? actorData.commandedActor.get().get() : nullptr;
				auto* avOwner = actor ? actor->As<RE::ActorValueOwner>() : nullptr;
				if (!avOwner || !(actor->IsSummoned() || actor->IsReanimated())) {
					continue;
				}
				a_out += avOwner->GetActorValue(RE::ActorValue::kResistShock) >= 100.0f ? 1 : 0;
			}
		}
		else if (untypedCount == a_obj) {
			for (const auto actorData : commandedActors) {
				auto* actor = actorData.commandedActor ? actorData.commandedActor.get().get() : nullptr;
				auto* avOwner = actor ? actor->As<RE::ActorValueOwner>() : nullptr;
				if (!avOwner || !actor->IsSummoned()) {
					continue;
				}
				a_out += avOwner->GetActorValue(RE::ActorValue::kResistFire) < 100.0f &&
					avOwner->GetActorValue(RE::ActorValue::kResistFrost) < 100.0f &&
					avOwner->GetActorValue(RE::ActorValue::kResistShock) < 100.0f
					? 1u : 0u;
			}
		}
		else if (conjuredUndeadCount == a_obj) {
			for (const auto actorData : commandedActors) {
				auto* actor = actorData.commandedActor ? actorData.commandedActor.get().get() : nullptr;
				auto* base = actor ? actor->GetActorBase() : nullptr;
				bool isUndead = base ? base->HasKeyword(actorTypeUndead) : false;
				isUndead = !isUndead && base && base->GetRace() ? base->GetRace()->HasKeyword(actorTypeUndead) : isUndead;
				if (!isUndead || !actor->IsSummoned()) {
					continue;
				}
				a_out += 1u;
			}
		}
		else if (reanimatedCount == a_obj) {
			for (const auto actorData : commandedActors) {
				auto* actor = actorData.commandedActor ? actorData.commandedActor.get().get() : nullptr;
				if (!actor) {
					continue;
				}
				a_out += actor->IsReanimated() ? 1 : 0;
			}
		}
		else {
			return false;
		}
		return true;
	}
}