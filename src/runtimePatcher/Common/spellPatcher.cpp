#include "spellPatcher.h"

#include "data/modObjectManager.h"

namespace
{
	union VOID_PARAM
	{
		char* c;
		std::int32_t i;
		float        f;
		RE::TESForm* ptr;
	};

	void AppendConditionToEffect(RE::TESConditionItem* a_condition, RE::Effect* a_effect, RE::BGSPerk* a_perk, RE::PlayerCharacter* a_player) {
		using PARAM_TYPE = RE::SCRIPT_PARAM_TYPE;
		using PARAMS = std::pair<std::optional<PARAM_TYPE>, std::optional<PARAM_TYPE>>;
		RE::CONDITION_ITEM_DATA condData{};

		const auto ref = a_player->AsReference();
		condData.runOnRef = ref->CreateRefHandle();
		condData.object = RE::CONDITIONITEMOBJECT::kSelf;

		PARAMS paramPair = { PARAM_TYPE::kPerk, PARAM_TYPE::kInt };
		auto& [param1Type, param2Type] = paramPair;
		if (param2Type) {
			VOID_PARAM param{};
			param.i = (int32_t)1;
			condData.functionData.params[1] = std::bit_cast<void*>(param);
		}
		if (param1Type) {
			VOID_PARAM param{};
			param.ptr = a_perk;
			condData.functionData.params[0] = std::bit_cast<void*>(param);
		}

		condData.flags.opCode = RE::CONDITION_ITEM_DATA::OpCode::kEqualTo;
		condData.functionData.function = RE::FUNCTION_DATA::FunctionID::kHasPerk;
		condData.comparisonValue.f = 1.0f;
		condData.flags.isOR = false;

		a_condition->data = condData;
		a_condition->next = nullptr;

		auto* tail = a_effect->conditions.head;
		while (tail && tail->next) {
			tail = tail->next;
		}

		if (tail) {
			tail->next = a_condition;
		}
		else {
			a_effect->conditions.head = a_condition;
		}
		a_condition->next = nullptr;
	}

	void PatchBoundWeaponSpell(RE::SpellItem* a_spell, 
		RE::PlayerCharacter* a_player,
		RE::EffectSetting* a_animate,
		RE::EffectSetting* a_animateEdgeOfOblivion) {

		auto summonAdvanced = new RE::Effect();
		assert(summonAdvanced);

		auto summonBasic = new RE::Effect();
		assert(summonBasic);
		if (!summonBasic || !summonAdvanced) {
			logger::error("    >Failed to create animate effect for {}.", a_spell->GetName());
			return;
		}

		summonBasic->baseEffect = a_animate;
		summonBasic->cost = 0.0f;
		summonBasic->effectItem.area = 0;
		summonBasic->effectItem.duration = 60;
		summonBasic->effectItem.magnitude = 0.0f;

		summonAdvanced->baseEffect = a_animateEdgeOfOblivion;
		summonAdvanced->cost = 0.0f;
		summonAdvanced->effectItem.area = 0;
		summonAdvanced->effectItem.duration = 60;
		summonAdvanced->effectItem.magnitude = 0.0f;

		a_spell->effects.push_back(summonBasic);
		a_spell->effects.push_back(summonAdvanced);
	}
}

namespace RuntimePatcher::Spells
{
	void Patch()
	{
		logger::info("  Starting spell patcher...");
		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		const auto player = RE::PlayerCharacter::GetSingleton();
		assert(dataHandler && player);
		if (!dataHandler || !player) {
			logger::error("    >Failed to get data handler or player!");
			return;
		}

		auto& spellArray = dataHandler->GetFormArray<RE::SpellItem>();

		const auto boundWeaponAnimatedEdgeOfOblivion = Data::ModObject<RE::EffectSetting>("ARM_Conjuration_MGF_ConjureBoundWeaponEdgeOfOblivion");
		const auto boundWeaponAnimated = Data::ModObject<RE::EffectSetting>("ARM_Conjuration_MGF_ConjureBoundWeapon");
		assert(boundWeaponAnimated && boundWeaponAnimatedEdgeOfOblivion);
		if (!boundWeaponAnimated || !boundWeaponAnimatedEdgeOfOblivion) {
			logger::error("    >Failed to get animated bound weapon effects.");
			return;
		}

		for (auto* spell : spellArray) {
			if (!spell || spell->effects.empty()) {
				continue;
			}

			bool isBoundWeaponSpell = false;
			for (const auto effect : spell->effects) {
				if (!effect || !effect->baseEffect) {
					continue;
				}

				if (effect->baseEffect->GetArchetype() == RE::EffectSetting::Archetype::kBoundWeapon) {
					isBoundWeaponSpell = true;
				}
			}

			if (isBoundWeaponSpell) {
				PatchBoundWeaponSpell(spell, player, boundWeaponAnimated, boundWeaponAnimatedEdgeOfOblivion);
			}
		}
	}
}