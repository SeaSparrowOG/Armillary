#pragma once

namespace ConditionManager
{
	bool Initialize();

	class ConditionManager : public REX::Singleton<ConditionManager>
	{
	public:
		bool Initialize();

		bool SubstituteItemCount(RE::TESBoundObject* a_obj, RE::TESObjectREFR* a_ref, uint32_t& a_out);
	
	private:
		RE::TESBoundObject* totalCount{ nullptr };
		RE::TESBoundObject* fireCount{ nullptr };
		RE::TESBoundObject* frostCount{ nullptr };
		RE::TESBoundObject* shockCount{ nullptr };
		RE::TESBoundObject* untypedCount{ nullptr };
		RE::TESBoundObject* conjuredUndeadCount{ nullptr };
		RE::TESBoundObject* reanimatedCount{ nullptr };

		RE::BGSKeyword* actorTypeUndead{ nullptr };
	};

	inline static constexpr const char* totalCountID = "ARM_AMMO_Framework_ConjuredReanimatedCount";
	inline static constexpr const char* fireCountID = "ARM_AMMO_Framework_FireConjuredCount";
	inline static constexpr const char* frostCountID = "ARM_AMMO_Framework_FrostConjuredCount";
	inline static constexpr const char* shockCountID = "ARM_AMMO_Framework_ShockConjuredCount";
	inline static constexpr const char* untypedCountID = "ARM_AMMO_Framework_UntypedConjuredCount";
	inline static constexpr const char* conjuredUndeadCountID = "ARM_AMMO_Framework_UndeadConjuredCount";
	inline static constexpr const char* reanimatedCountID = "ARM_AMMO_Framework_ReanimatedCount";
}