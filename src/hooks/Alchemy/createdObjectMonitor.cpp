#include "createdObjectMonitor.h"

#include "data/modObjectManager.h"

namespace Hooks::Alchemy
{
	void Install()
	{
		CreatedObjectMonitor::GetSingleton()->Install();
	}

	void CreatedObjectMonitor::Install()
	{
		//1.6.1170: 14062d7ef
		REL::Relocation<std::uintptr_t> createItemTarget{ REL::ID(36179), 0x16F }; //Expected: e8 cc 3c b8 ff
		//1.6.1170: 1409090fa
		REL::Relocation<std::uintptr_t> selectItemTarget{ REL::ID(51343), 0x2A }; //Expected: e8 31 63 00 00

		if (!REL::make_pattern<"E8">().match(createItemTarget.address()) ||
			!REL::make_pattern<"E8">().match(selectItemTarget.address())) {

			util::report_and_fail("Alchemy: Failed to validate pattern, aborting launch.");
		}


		auto& trampoline = SKSE::GetTrampoline();
		_createItem = trampoline.write_call<5>(createItemTarget.address(), &CreateItem);
		_selectItem = trampoline.write_call<5>(selectItemTarget.address(), &SelectItem);
	}

	void CreatedObjectMonitor::CreateItem(RE::TESDataHandler* a_dataHandler, RE::AlchemyItem* a_alchemyItem)
	{
		const auto player = RE::PlayerCharacter::GetSingleton();
		const auto singleton = CreatedObjectMonitor::GetSingleton();
		assert(player && singleton);

		const auto qualityIngredientsPerk = Data::ModObject<RE::BGSPerk>("ARM_Alchemy_PRK_060_QualityIngredients");
		assert(qualityIngredientsPerk);

		const auto lastingTreatmentPerk = Data::ModObject<RE::BGSPerk>("ARM_Alchemy_PRK_050_LastingTreatment");
		assert(lastingTreatmentPerk);

		const auto stimulantPerk = Data::ModObject<RE::BGSPerk>("ARM_Alchemy_PRK_080_Stimulants");
		const auto stimulantHealth = Data::ModObject<RE::EffectSetting>("ARM_Alchemy_MGF_StimulantsHealth");
		const auto stimulantStamina = Data::ModObject<RE::EffectSetting>("ARM_Alchemy_MGF_StimulantsStamina");
		const auto stimulantMagicka = Data::ModObject<RE::EffectSetting>("ARM_Alchemy_MGF_StimulantsMagicka");

		const auto slowDeathPerk = Data::ModObject<RE::BGSPerk>("ARM_Alchemy_PRK_080_SlowDeath");
		const auto slowDeathEffect = Data::ModObject<RE::EffectSetting>("ARM_Alchemy_MGF_SlowDeathEffect");
		assert(qualityIngredientsPerk);
		assert(stimulantPerk && stimulantHealth && stimulantStamina && stimulantMagicka && slowDeathPerk && slowDeathEffect);
		assert(slowDeathPerk && slowDeathEffect);

		if (!a_alchemyItem->IsPoison() && stimulantPerk && player->HasPerk(stimulantPerk)) {
			if (qualityIngredientsPerk && player->HasPerk(qualityIngredientsPerk)) {
				for (auto* effect : a_alchemyItem->effects) {
					if (effect->baseEffect && !effect->baseEffect->IsHostile()) {
						effect->effectItem.duration *= static_cast<uint32_t>(singleton->currentBoostPct);
						effect->effectItem.magnitude *= singleton->currentBoostPct;
					}
				}
			}

			if (lastingTreatmentPerk && player->HasPerk(lastingTreatmentPerk)) {
				for (auto* effect : a_alchemyItem->effects) {
					if (effect->baseEffect && !effect->baseEffect->IsHostile()) {
						effect->effectItem.duration *= 2;
					}
				}
			}

			if (stimulantHealth && stimulantStamina && stimulantMagicka) {
				auto healthEffect = new RE::Effect();
				auto staminaEffect = new RE::Effect();
				auto magickaEffect = new RE::Effect();

				if (healthEffect) {
					healthEffect->effectItem.magnitude = 50.0f;
					healthEffect->effectItem.area = 0;
					healthEffect->effectItem.duration = 60;
					healthEffect->baseEffect = stimulantHealth;
					healthEffect->cost = 0.0f;

					a_alchemyItem->effects.push_back(healthEffect);
				}

				if (staminaEffect) {
					staminaEffect->effectItem.magnitude = 50.0f;
					staminaEffect->effectItem.area = 0;
					staminaEffect->effectItem.duration = 60;
					staminaEffect->baseEffect = stimulantStamina;
					staminaEffect->cost = 0.0f;

					a_alchemyItem->effects.push_back(staminaEffect);
				}

				if (magickaEffect) {
					magickaEffect->effectItem.magnitude = 50.0f;
					magickaEffect->effectItem.area = 0;
					magickaEffect->effectItem.duration = 60;
					magickaEffect->baseEffect = stimulantMagicka;
					magickaEffect->cost = 0.0f;

					a_alchemyItem->effects.push_back(magickaEffect);
				}
			}
		}
		else if (a_alchemyItem->IsPoison() && slowDeathPerk && player->HasPerk(slowDeathPerk)) {
			if (qualityIngredientsPerk && player->HasPerk(qualityIngredientsPerk)) {
				for (auto* effect : a_alchemyItem->effects) {
					if (effect->baseEffect && effect->baseEffect->IsHostile()) {
						effect->effectItem.duration *= static_cast<uint32_t>(singleton->currentBoostPct);
						effect->effectItem.magnitude *= singleton->currentBoostPct;
					}
				}
			}

			if (stimulantHealth && slowDeathEffect) {
				auto damageHealthEffect = new RE::Effect();
				if (damageHealthEffect) {
					damageHealthEffect->effectItem.magnitude = 10.0f;
					damageHealthEffect->effectItem.area = 0;
					damageHealthEffect->effectItem.duration = 5;
					damageHealthEffect->baseEffect = slowDeathEffect;
					damageHealthEffect->cost = 0.0f;

					a_alchemyItem->effects.push_back(damageHealthEffect);
				}
			}
		}
		_createItem(a_dataHandler, a_alchemyItem);
	}

	bool CreatedObjectMonitor::SelectItem(const RE::CraftingSubMenus::AlchemyMenu& a_menu, const uint64_t& a_param2)
	{
		const auto response = _selectItem(a_menu, a_param2);
		
		const auto singleton = CreatedObjectMonitor::GetSingleton();
		const auto player = RE::PlayerCharacter::GetSingleton();
		assert(player && singleton);

		singleton->currentBoostPct = 1.0f;

		const auto qualityIngredientsPerk = Data::ModObject<RE::BGSPerk>("ARM_Alchemy_PRK_060_QualityIngredients");
		const auto uncommonIngredients = Data::ModObject<RE::BGSListForm>("ARM_Alchemy_FRL_UncommonIngredientsList");
		const auto rareIngredientsList = Data::ModObject<RE::BGSListForm>("ARM_Alchemy_FRL_RareIngredientsList");
		const auto exquisiteIngredientsList = Data::ModObject<RE::BGSListForm>("ARM_Alchemy_FRL_ExquisiteIngredientsList");
		assert(qualityIngredientsPerk && uncommonIngredients && rareIngredientsList && exquisiteIngredientsList);

		if (!qualityIngredientsPerk || !player->HasPerk(qualityIngredientsPerk)) {
			return response;
		}

		if (!uncommonIngredients || !rareIngredientsList || !exquisiteIngredientsList) {
			return response;
		}

		bool hasUncommon = false;
		bool hasRare = false;
		bool hasExquisite = false;

		for (const auto& ingredient : *a_menu.potionCreationData.ingredientEntries) {
			if (!ingredient.isSelected) {
				continue;
			}

			const auto baseObj = ingredient.ingredient->GetObject();
			const auto alciObj = baseObj ? baseObj->As<RE::AlchemyItem>() : nullptr;
			if (!alciObj) {
				continue;
			}

			if (!hasUncommon && uncommonIngredients->HasForm(alciObj->formID)) {
				singleton->currentBoostPct += 0.15f;
				hasUncommon = true;
			}
			else if (!hasRare && rareIngredientsList->HasForm(alciObj->formID)) {
				singleton->currentBoostPct += 0.25f;
				hasRare = true;
			}
			else if (!hasExquisite && exquisiteIngredientsList->HasForm(alciObj->formID)) {
				singleton->currentBoostPct += 0.50f;
				hasExquisite = true;
			}
		}
		return response;
	}
}