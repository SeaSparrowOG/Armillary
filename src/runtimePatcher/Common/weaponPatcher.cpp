#include "weaponPatcher.h"

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
		condData.object = RE::CONDITIONITEMOBJECT::kTarget;

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
	}

	void PatchBoundWeapon(RE::PlayerCharacter* a_player,
		RE::TESObjectWEAP* a_weapon,
		RE::BGSKeyword* a_boundWeaponKwd,
		RE::BGSPerk* a_voracityPerk,
		RE::BGSPerk* a_edgeOfOblivionPerk,
		RE::EffectSetting* a_edgeMR,
		RE::EffectSetting* a_edgeArmor,
		RE::EffectSetting* a_voracity,
		std::vector<RE::EnchantmentItem*>& a_patchedEnchantments) {

		if (!a_weapon->HasKeyword(a_boundWeaponKwd)) {
			a_weapon->AddKeyword(a_boundWeaponKwd);
		}

		auto* weaponEnchant = a_weapon->formEnchanting;
		if (weaponEnchant && (a_patchedEnchantments.empty() ||
			std::find(
				a_patchedEnchantments.begin(),
				a_patchedEnchantments.end(),
				weaponEnchant) == a_patchedEnchantments.end())) {
			a_patchedEnchantments.push_back(weaponEnchant);

			auto voracity = new RE::Effect();
			assert(voracity);
			if (!voracity) {
				logger::error("    >Failed to create Voracity effect for {}/{}.", a_boundWeaponKwd->GetName(), weaponEnchant->GetName());
				return;
			}

			auto* voracityCondition = new RE::TESConditionItem();
			assert(voracityCondition);
			if (!voracityCondition) {
				logger::error("    >Failed to create voracity condition for {}/{}.", a_boundWeaponKwd->GetName(), weaponEnchant->GetName());
				return;
			}

			AppendConditionToEffect(voracityCondition, voracity, a_voracityPerk, a_player);

			voracity->baseEffect = a_voracity;
			voracity->cost = 0.0f;
			voracity->effectItem.area = 0;
			voracity->effectItem.duration = 1;
			voracity->effectItem.magnitude = 25.0f;

			weaponEnchant->effects.push_back(voracity);

			auto edgeEffect = new RE::Effect();
			assert(edgeEffect);
			if (!edgeEffect) {
				logger::error("    >Failed to create Edge of Oblivion effect for {}/{}.", a_boundWeaponKwd->GetName(), weaponEnchant->GetName());
				return;
			}

			auto* edgeCondition = new RE::TESConditionItem();
			assert(edgeCondition);
			if (!edgeCondition) {
				logger::error("    >Failed to create Edge of Oblivion condition for {}/{}.", a_boundWeaponKwd->GetName(), weaponEnchant->GetName());
				return;
			}

			AppendConditionToEffect(edgeCondition, edgeEffect, a_edgeOfOblivionPerk, a_player);

			edgeEffect->baseEffect = a_edgeArmor;
			edgeEffect->cost = 0.0f;
			edgeEffect->effectItem.area = 0;
			edgeEffect->effectItem.duration = 10;
			edgeEffect->effectItem.magnitude = 150.0f;

			weaponEnchant->effects.push_back(edgeEffect);

			auto edgeEffectMR = new RE::Effect();
			assert(edgeEffectMR);
			if (!edgeEffectMR) {
				logger::error("    >Failed to create Edge of Oblivion effect for {}/{}.", a_boundWeaponKwd->GetName(), weaponEnchant->GetName());
				return;
			}

			auto* edgeConditionMR = new RE::TESConditionItem();
			assert(edgeConditionMR);
			if (!edgeConditionMR) {
				logger::error("    >Failed to create Edge of Oblivion condition for {}/{}.", a_boundWeaponKwd->GetName(), weaponEnchant->GetName());
				return;
			}

			AppendConditionToEffect(edgeConditionMR, edgeEffectMR, a_edgeOfOblivionPerk, a_player);

			edgeEffectMR->baseEffect = a_edgeMR;
			edgeEffectMR->cost = 0.0f;
			edgeEffectMR->effectItem.area = 0;
			edgeEffectMR->effectItem.duration = 10;
			edgeEffectMR->effectItem.magnitude = 25.0f;

			weaponEnchant->effects.push_back(edgeEffectMR);
		}
	}
}

namespace RuntimePatcher::Weapons
{
	void Patch()
	{
		logger::info("  Starting weapon patcher...");
		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		const auto player = RE::PlayerCharacter::GetSingleton();
		assert(dataHandler && player);
		if (!dataHandler || !player) {
			logger::error("    >Failed to get data handler or player!");
			return;
		}

		size_t patchedWeapons = 0;
		auto& weaponArray = dataHandler->GetFormArray<RE::TESObjectWEAP>();
		const auto boundWeaponKwd = Data::ModObject<RE::BGSKeyword>("ARM_Conjuration_KWD_BoundWeapon");

		const auto boundWeaponVoraciousBlade = Data::ModObject<RE::EffectSetting>("ARM_Conjuration_MGF_VoraciousBladeHealth");
		const auto boundWeaponEdgeOfOblivionArmor = Data::ModObject<RE::EffectSetting>("ARM_Conjuration_MGF_EdgeOfOblivionArmor");
		const auto boundWeaponEdgeOfOblivionMagicResist = Data::ModObject<RE::EffectSetting>("ARM_Conjuration_MGF_EdgeOfOblivionMagicResist");

		const auto boundWeaponVoracityPerk = Data::ModObject<RE::BGSPerk>("ARM_Conjuration_PRK_080_VoraciousEdge");
		const auto boundWeaponEdgeOfOblivionPerk = Data::ModObject<RE::BGSPerk>("ARM_Conjuration_PRK_080_EdgeOfOblivion");
		assert(boundWeaponKwd && 
			boundWeaponVoraciousBlade &&
			boundWeaponEdgeOfOblivionArmor && 
			boundWeaponEdgeOfOblivionMagicResist &&
			boundWeaponVoracityPerk &&
			boundWeaponEdgeOfOblivionPerk);

		if (!boundWeaponKwd) {
			logger::error("    >Failed to get bound weapon keyword!");
			return;
		}
		if (!boundWeaponVoracityPerk || !boundWeaponVoraciousBlade) {
			logger::error("    >Failed to get bound weapon voracity perk!");
			return;
		}
		if (!boundWeaponEdgeOfOblivionArmor || !boundWeaponEdgeOfOblivionMagicResist || !boundWeaponEdgeOfOblivionPerk) {
			logger::error("    >Failed to get bound weapon edge of oblivion perk!");
			return;
		}

		std::vector<RE::EnchantmentItem*> patchedEnchantments{};
		for (auto* weapon : weaponArray) {
			if (!weapon) {
				continue;
			}

			weapon->criticalData.prcntMult = 0.0f;
			weapon->weaponData.staggerValue = 0.0f;
			patchedWeapons++;

			if (weapon->IsBound()) {
				PatchBoundWeapon(player,
					weapon,
					boundWeaponKwd,
					boundWeaponVoracityPerk,
					boundWeaponEdgeOfOblivionPerk,
					boundWeaponEdgeOfOblivionMagicResist,
					boundWeaponEdgeOfOblivionArmor,
					boundWeaponVoraciousBlade,
					patchedEnchantments);
			}
		}
		logger::info("    >Finished patching {} weapons.", patchedWeapons);
	}
}
