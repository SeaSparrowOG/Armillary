#include "events.h"

#include "events/Spells/spellCast.h"

namespace Events 
{
	void Install() {
		SpellCast::Listener::GetSingleton()->RegisterListener();
	}
}