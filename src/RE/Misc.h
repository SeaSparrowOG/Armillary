#pragma once

namespace RE
{
    static RE::ActorValue LookupActorValueByName(const char* av_name)
    {
        using func_t = decltype(&LookupActorValueByName);
        REL::Relocation<func_t> func{ REL::ID(27203) };
        return func(av_name);
    }
}