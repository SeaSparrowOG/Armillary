Scriptname ARM_RighteousRetaliationProc extends activemagiceffect  

Perk Property ARM_PERK_HeavyArmor_095_RightToRule_WAS_TowerOfStrength Auto
Spell Property ARM_SPEL_HeavyArmor_RighteousRetaliationProc Auto 
Spell Property ARM_SPEL_HeavyArmor_RightToRuleProc Auto 

Actor kPaladin

Event OnEffectStart(Actor a_kTarget, Actor a_kCaster)

    kPaladin = a_kTarget
EndEvent

Event OnDying(Actor a_kKiller)

    If (kPaladin)

        kPaladin = NONE ;There is a weird edge case where OnDying can fire TWICE. Do not remove this.

        ARM_SPEL_HeavyArmor_RighteousRetaliationProc.Cast(kPaladin)
        If (kPaladin.HasPerk(ARM_PERK_HeavyArmor_095_RightToRule_WAS_TowerOfStrength) && kPaladin == a_kKiller) 

            ARM_SPEL_HeavyArmor_RightToRuleProc.Cast(kPaladin)
        EndIf
    EndIf
EndEvent