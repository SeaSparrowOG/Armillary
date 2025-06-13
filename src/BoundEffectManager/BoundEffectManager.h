#pragma once

#include "Serialization/SerializationManager/SerializationManager.h"

namespace BoundEffectManager
{
	bool Initialize();

	class BoundEffectManager :
		public REX::Singleton<BoundEffectManager>,
		public Serialization::SerializationManager::Serializable
	{
	public:
		bool Initialize();

		bool CanCastSpell(RE::MagicItem* a_spell, bool a_dualCast);

		void ProcessEffectAdded(RE::ActiveEffect* a_effect);
		void ProcessEffectRemoved(RE::ActiveEffect* a_effect);

		bool IsBoundEffect(RE::ActiveEffect* a_effect); 

		bool Save(SKSE::SerializationInterface* a_intfc) override;
		bool Load(SKSE::SerializationInterface* a_intfc) override;
		void Revert(SKSE::SerializationInterface* a_intfc) override;
	private:
		RE::EffectSetting* GetCostliestValidEffect(RE::MagicItem* a_spell);

		bool HasEnoughOfAttributeToBind(RE::ActorValue a_av, float a_demand);
		bool IsBindingEffectApplicable(const RE::EffectSetting* a_effect, bool a_dualCast);

		using BindableAttribute = std::pair<RE::ActorValue, RE::ActorValue>;

		struct BoundAttribute
		{
			float ammount{ 0.0f };
			BindableAttribute attribute{ RE::ActorValue::kMagicka, RE::ActorValue::kVariable04 };

			void Restore(RE::PlayerCharacter* a_player) {
				a_player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, attribute.first, ammount);
				a_player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, attribute.second, -ammount);
				attribute = { RE::ActorValue::kNone, RE::ActorValue::kNone };
				ammount = 0.0f;
			}

			void Bind(RE::PlayerCharacter* a_player) const {
				a_player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, attribute.first, ammount);
				a_player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, attribute.first, -ammount);
				a_player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, attribute.second, +ammount);
			}
		};

		float totalHealthBound{ 0.0f };
		float totalStaminaBound{ 0.0f };
		float totalMagickaBound{ 0.0f };
		std::unordered_map<RE::ActiveEffect*, std::vector<BoundAttribute>> costliestBindings{};
		std::unordered_set<RE::ActiveEffect*> boundEffects{};
		
		RE::BGSKeyword* bindHealthKeyword{ nullptr };
		RE::BGSKeyword* bindStaminaKeyword{ nullptr };
		RE::BGSKeyword* bindMagickaKeyword{ nullptr };

		RE::PlayerCharacter* player{ nullptr };
		inline static constexpr uint32_t RecordType{ 'BEFM' };

		inline static constexpr const BindableAttribute magickaBinding
			= { RE::ActorValue::kMagicka, RE::ActorValue::kVariable04 };
		inline static constexpr const BindableAttribute staminaBinding
			= { RE::ActorValue::kStamina, RE::ActorValue::kVariable04 };
		inline static constexpr const BindableAttribute healthBinding
			= { RE::ActorValue::kHealth, RE::ActorValue::kVariable04 };
	};

	inline static constexpr const char* BindHealthKeywordID = "ARM_KEYW_Framework_BindHealthKeyword";
	inline static constexpr const char* BindStaminaKeywordID = "ARM_KEYW_Framework_BindStaminaKeyword";
	inline static constexpr const char* BindMagickaKeywordID = "ARM_KEYW_Framework_BindMagickaKeyword";
}