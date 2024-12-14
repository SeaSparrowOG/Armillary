#pragma once

#include "RE/Offset.h"

namespace RE
{
    static RE::ActorValue LookupActorValueByName(const char* av_name)
    {
        using func_t = decltype(&LookupActorValueByName);
        REL::Relocation<func_t> func{ REL::ID(27203) };
        return func(av_name);
    }

    inline void SetPlayerTeammate(RE::Actor* a_this, bool a_teammate, bool a_canDoFavor) {
        using func_t = decltype(&SetPlayerTeammate);
        static REL::Relocation<func_t> func{ Offset::Script::SetPlayerTeammate };
        func(a_this, a_teammate, a_canDoFavor);
    }
}