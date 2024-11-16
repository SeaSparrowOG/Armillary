#pragma once

namespace Data
{
	class ModObjectManager final : public RE::BSTEventSink<RE::TESQuestInitEvent>
	{
	public:
		static constexpr auto QuestName = "ARM_ModObjectManager"sv;
		static constexpr auto ScriptName = "ARM_ModObjects"sv;

		ModObjectManager(const ModObjectManager&) = delete;
		ModObjectManager(ModObjectManager&&) = delete;

		~ModObjectManager() = default;

		ModObjectManager& operator=(const ModObjectManager&) = delete;
		ModObjectManager& operator=(ModObjectManager&&) = delete;

		static ModObjectManager& Instance();

		RE::BSEventNotifyControl ProcessEvent(
			const RE::TESQuestInitEvent* a_event,
			RE::BSTEventSource<RE::TESQuestInitEvent>* a_eventSource) override;

		void Reload();

		[[nodiscard]] RE::TESForm* Get(std::string_view a_key) const;

	private:
		ModObjectManager() = default;

		void Initialize(RE::TESQuest* a_quest);

		util::istring_map<RE::TESForm*> objects;
	};

	template <typename T>
	[[nodiscard]] inline T* ModObject(std::string_view a_key)
	{
		if (const auto object = ModObjectManager::Instance().Get(a_key))
			return object->As<T>();
		return nullptr;
	}
}