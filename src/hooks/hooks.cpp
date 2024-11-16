#include "Hooks/hooks.h"

#include "Alchemy/createdObjectMonitor.h"

namespace Hooks {
	void Install()
	{
		logger::info("Installing Alchemy hooks...");
		Hooks::Alchemy::Install();
	}
}