#include "papyrus.h"

#include "Settings/INI/INISettings.h"
#include "Settings/JSON/JSONSettings.h"

namespace Papyrus
{
	static std::vector<int> GetVersion(STATIC_ARGS) {
		return { Plugin::VERSION[0], Plugin::VERSION[1], Plugin::VERSION[2] };
	}

	static bool CheckRequiredMods(STATIC_ARGS) {
		// TODO: Check Required Mods (none yet)
		return true;
	}

	static void LogMessage(STATIC_ARGS, RE::BSFixedString a_message, int a_severity) {
		if (a_message.empty()) {
			a_message = "[EMPTY]";
		}
		switch (a_severity) {
		case 0:
			LOG_DEBUG("Papyrus: {}", a_message.c_str());
			break;
		case 1:
			logger::info("Papyrus: {}", a_message.c_str());
			break;
		default:

			break;
		}
	}

	static void ReloadINISettings(STATIC_ARGS) {
		auto* manager = Settings::INI::Holder::GetSingleton();
		if (!manager) {
			logger::warn("Papyrus: Failed to reload INI settings due to being unable to fetch the INI mananger."sv);
			return;
		}
		manager->Reload();
	}

	static void ReloadJSONSettings(STATIC_ARGS) {
		auto* manager = Settings::JSON::Reader::GetSingleton();
		if (!manager) {
			logger::warn("Papyrus: Failed to reload INI settings due to being unable to fetch the INI mananger."sv);
			return;
		}
		manager->Reload();
	}

	static void Bind(VM& a_vm) {
		logger::info("  >Binding GetVersion..."sv);
		BIND(GetVersion);
		logger::info("  >Binding CheckRequiredMods..."sv);
		BIND(CheckRequiredMods);
		logger::info("  >Binding LogMessage..."sv);
		BIND(LogMessage);
		logger::info("  >Binding ReloadINISettings..."sv);
		BIND(ReloadINISettings);
		logger::info("  >Binding ReloadJSONSettings..."sv);
		BIND(ReloadJSONSettings);
	}

	bool RegisterFunctions(VM* a_vm) {
		logger::info("Binding papyrus functions in utility script {}..."sv, script);
		Bind(*a_vm);
		logger::info("Finished binding functions."sv);
		return true;
	}
}
