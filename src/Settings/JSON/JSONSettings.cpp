#include "JSONSettings.h"

namespace Settings::JSON
{
	bool Read() {
		logger::info("Reading JSON settings..."sv);
		auto* reader = Reader::GetSingleton();
		if (!reader) {
			logger::critical("  >Failed to fetch the reader singleton."sv);
			return false;
		}
		return reader->Read();
	}

	bool Reader::Read()
	{
		dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			logger::critical("  >Failed to cache DataHandler singleton."sv);
			return false;
		}
		std::string jsonFolder = fmt::format(R"(.\Data\SKSE\Plugins\{})"sv, Plugin::NAME);
		logger::info("  >Settings folder: {}."sv, jsonFolder);
		if (!std::filesystem::exists(jsonFolder)) {
			logger::info("    >No settings folder found."sv);
			return true;
		}

		std::vector<std::string> paths{};
		try {
			for (const auto& entry : std::filesystem::directory_iterator(jsonFolder)) {
				if (entry.is_regular_file() && entry.path().extension() == ".json") {
					paths.push_back(entry.path().string());
				}
			}

			std::sort(paths.begin(), paths.end());
			logger::info("    >Found {} configuration files."sv, std::to_string(paths.size()));
		}
		catch (const std::exception& e) {
			logger::warn("Caught {} while reading files."sv, e.what());
			return false;
		}
		if (paths.empty()) {
			logger::info("    >No settings found"sv);
			return true;
		}

		for (const auto& path : paths) {
			auto configName = path.substr(jsonFolder.size() + 1, path.size() - 1);
			logger::info("    >Reading config {}..."sv, configName);
			Json::Reader JSONReader;
			Json::Value JSONFile;
			try {
				std::ifstream rawJSON(path);
				JSONReader.parse(rawJSON, JSONFile);

				if (!ReadConfig(JSONFile)) {
					logger::warn("      >Config treated as invalid, skipping."sv);
					continue;
				}
			}
			catch (const Json::Exception& e) {
				logger::warn("Caught {} while reading files.", e.what());
				continue;
			}
			catch (const std::exception& e) {
				logger::error("Caught unhandled exception {} while reading files.", e.what());
				continue;
			}
		}

		logger::info("Finished reading all settings."sv);
		return true;
	}

	bool Reader::ReadConfig(const Json::Value& a_json) {
		const auto& minVersionField = a_json[MINIMUM_VERSION_FIELD];
		if (minVersionField) {
			if (!minVersionField.isInt()) {
				logger::warn("      >Config has {} specified, but it is not an integer."sv, MINIMUM_VERSION_FIELD);
				return false;
			}
			auto requiredVer = minVersionField.asInt();
			if (requiredVer < 1 || requiredVer > std::numeric_limits<uint8_t>::max()) {
				logger::warn("      >Config has {} specified, but its required version is either too large or too small."sv, MINIMUM_VERSION_FIELD);
				return false;
			}

			auto sanitizedVer = static_cast<uint8_t>(requiredVer);
			if (PARSER_VERSION < sanitizedVer) {
				logger::warn("      >Config requires parser version {}, but the current parser is version {}."sv, sanitizedVer, PARSER_VERSION);
				return false;
			}
		}
		auto& patchSpellsEntry = a_json[PATCH_SPELL_FIELD];
		if (patchSpellsEntry && !ReadPatchSpells(patchSpellsEntry)) {
			return false;
		}
		return true;
	}

	bool Reader::ReadPatchSpells(const Json::Value& a_entry) {
		if (!a_entry.isObject()) {
			logger::error("      >Config has {} specified, but it is not an object."sv, PATCH_SPELL_FIELD);
			return false;
		}
		auto& addEffectField = a_entry[PATCH_SPELL_FIELD_ADD_EFFECT];
		if (addEffectField && !ReadPatchSpellsAddEffect(addEffectField)) {
			return false;
		}
		return true;
	}

	bool Reader::ReadPatchSpellsAddEffect(const Json::Value& a_entry) {
		if (!a_entry.isArray()) {
			logger::error("      >Config has {} specified, but it is not an array."sv, PATCH_SPELL_FIELD_ADD_EFFECT);
			return false;
		}
		for (const auto& spellPatch : a_entry) {
			if (!spellPatch.isObject()) {
				logger::error("      >Config has {} specified, but at least one of its contents is not an object."sv, PATCH_SPELL_FIELD_ADD_EFFECT);
				return false;
			}

			auto& base = spellPatch[PATCH_SPELL_FIELD_ADD_EFFECT_BASE];
			if (!base) {
				logger::error("      >Config has {} specified, but is missing a base effect definition."sv, PATCH_SPELL_FIELD_ADD_EFFECT_BASE);
				return false;
			}
			auto& spellField = spellPatch[PATCH_SPELL_FIELD_ADD_EFFECT_SPELL];
			if (!spellField) {
				logger::error("      >Config has {} specified, but is missing a base spell definition."sv, PATCH_SPELL_FIELD_ADD_EFFECT_BASE);
				return false;
			}

			auto& magField = spellPatch[PATCH_SPELL_FIELD_ADD_EFFECT_MAG];
			float mag = 0.0f;
			if (magField) {
				if (!magField.isDouble()) {
					logger::error("      >Config has {}/{} specified, but it is not a float."sv, PATCH_SPELL_FIELD_ADD_EFFECT_BASE, PATCH_SPELL_FIELD_ADD_EFFECT_MAG);
					return false;
				}
				mag = std::max(0.0f, magField.asFloat());
			}

			auto& areaField = spellPatch[PATCH_SPELL_FIELD_ADD_EFFECT_AREA];
			uint32_t area = 0;
			if (areaField) {
				if (!areaField.isInt()) {
					logger::error("      >Config has {}/{} specified, but it is not an integer."sv, PATCH_SPELL_FIELD_ADD_EFFECT_BASE, PATCH_SPELL_FIELD_ADD_EFFECT_AREA);
					return false;
				}

				auto rawInt = areaField.asInt();
				area = std::numeric_limits<uint32_t>::max() > rawInt ?
					0 < rawInt ?
					static_cast<uint32_t>(rawInt) :
					std::numeric_limits<uint32_t>::min() :
					std::numeric_limits<uint32_t>::max();
			}

			auto& durField = spellPatch[PATCH_SPELL_FIELD_ADD_EFFECT_DUR];
			uint32_t dur = 0;
			if (durField) {
				if (!durField.isInt()) {
					logger::error("      >Config has {}/{} specified, but it is not an integer."sv, PATCH_SPELL_FIELD_ADD_EFFECT_BASE, PATCH_SPELL_FIELD_ADD_EFFECT_DUR);
					return false;
				}

				auto rawInt = durField.asInt();
				dur = std::numeric_limits<uint32_t>::max() > rawInt ?
					0 < rawInt ?
					static_cast<uint32_t>(rawInt) :
					std::numeric_limits<uint32_t>::min() :
					std::numeric_limits<uint32_t>::max();
			}

			auto spellsToModify = std::vector<RE::SpellItem*>();
			auto effectsToAdd = std::vector<RE::EffectSetting*>();
			if (!AddMultipleFormsToVector(base, effectsToAdd)) {
				return false;
			}
			if (!AddMultipleFormsToVector(spellField, spellsToModify)) {
				return false;
			}
			if (spellsToModify.empty() || effectsToAdd.empty()) {
				continue;
			}

			auto* effectFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::EffectSetting>();
			if (!effectFactory) {
				logger::critical("      >Internal error: Failed to get Effect Setting Form Factory from the game."sv);
				return false;
			}

			for (auto* spell : spellsToModify) {
				auto delivery = spell->GetDelivery();
				auto casting = spell->GetCastingType();
				for (auto* effect : effectsToAdd) {
					using AMEFlag = RE::EffectSetting::EffectSettingData::Flag;

					auto* newEffect = effectFactory->Create();
					if (!newEffect) {
						logger::critical("      >Internal error: Form factory failed to create new effect."sv);
						return false;
					}

					newEffect->data = effect->data;
					newEffect->data.delivery = delivery;
					newEffect->data.castingType = casting;

					newEffect->conditions = effect->conditions;
					newEffect->counterEffects = effect->counterEffects;
					newEffect->effectSounds = effect->effectSounds;
					newEffect->fullName = effect->fullName;
					newEffect->keywords = effect->keywords;
					newEffect->magicItemDescription = effect->magicItemDescription;
					newEffect->menuDispObject = effect->menuDispObject;
					newEffect->numKeywords = effect->numKeywords;
					newEffect->filterValidationFunction = effect->filterValidationFunction;
					newEffect->filterValidationItem = effect->filterValidationItem;

					auto* toAppend = new RE::Effect();
					toAppend->baseEffect = newEffect;
					toAppend->conditions = RE::TESCondition();
					toAppend->cost = 0.0f;
					if (effect->data.flags.any(AMEFlag::kNoArea)) {
						toAppend->effectItem.area = 0u;
					}
					else {
						toAppend->effectItem.area = area;
					}
					if (effect->data.flags.any(AMEFlag::kNoDuration)) {
						toAppend->effectItem.duration = 0u;
					}
					else {
						toAppend->effectItem.duration = dur;
					}
					if (effect->data.flags.any(AMEFlag::kNoMagnitude)) {
						toAppend->effectItem.magnitude = 0.0f;
					}
					else {
						toAppend->effectItem.magnitude = mag;
					}
					spell->effects.push_back(toAppend);
				}
			}
		}
		return true;
	}
}

namespace Settings::JSON
{
	std::string Reader::GetEditorID(const RE::TESForm* a_form)
	{
		switch (a_form->GetFormType()) {
		case RE::FormType::Keyword:
		case RE::FormType::LocationRefType:
		case RE::FormType::Action:
		case RE::FormType::MenuIcon:
		case RE::FormType::Global:
		case RE::FormType::HeadPart:
		case RE::FormType::Race:
		case RE::FormType::Sound:
		case RE::FormType::Script:
		case RE::FormType::Navigation:
		case RE::FormType::Cell:
		case RE::FormType::WorldSpace:
		case RE::FormType::Land:
		case RE::FormType::NavMesh:
		case RE::FormType::Dialogue:
		case RE::FormType::Quest:
		case RE::FormType::Idle:
		case RE::FormType::AnimatedObject:
		case RE::FormType::ImageAdapter:
		case RE::FormType::VoiceType:
		case RE::FormType::Ragdoll:
		case RE::FormType::DefaultObject:
		case RE::FormType::MusicType:
		case RE::FormType::StoryManagerBranchNode:
		case RE::FormType::StoryManagerQuestNode:
		case RE::FormType::StoryManagerEventNode:
		case RE::FormType::SoundRecord:
			return a_form->GetFormEditorID();
		default:
		{
			static auto tweaks = REX::W32::GetModuleHandleW(L"po3_Tweaks");
			static auto func = reinterpret_cast<_GetFormEditorID>(REX::W32::GetProcAddress(tweaks, "GetFormEditorID"));
			if (func) {
				return func(a_form->formID);
			}
			return {};
		}
		}
	}

	inline std::vector<std::string> Reader::split(const std::string& a_str, const std::string& a_delimiter)
	{
		std::vector<std::string> result;
		size_t start = 0;
		size_t end = a_str.find(a_delimiter);

		while (end != std::string::npos) {
			result.push_back(a_str.substr(start, end - start));
			start = end + a_delimiter.length();
			end = a_str.find(a_delimiter, start);
		}

		result.push_back(a_str.substr(start));
		return result;
	}

	inline bool Reader::is_only_hex(std::string_view a_str, bool a_requirePrefix)
	{
		if (!a_requirePrefix) {
			return std::ranges::all_of(a_str, [](unsigned char ch) {
				return std::isxdigit(ch);
				});
		}
		else if (a_str.compare(0, 2, "0x") == 0 || a_str.compare(0, 2, "0X") == 0) {
			return a_str.size() > 2 && std::all_of(a_str.begin() + 2, a_str.end(), [](unsigned char ch) {
				return std::isxdigit(ch);
				});
		}
		return false;
	}

	inline std::string Reader::tolower(std::string_view a_str)
	{
		std::string result(a_str);
		std::ranges::transform(result, result.begin(), [](unsigned char ch) { return static_cast<unsigned char>(std::tolower(ch)); });
		return result;
	}
}