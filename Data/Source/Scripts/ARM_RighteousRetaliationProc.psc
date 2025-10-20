Scriptname ARM_RighteousRetaliationProc extends activemagiceffect  

Perk Property ARM_PERK_HeavyArmor_095_RightToRule_WAS_TowerOfStrength Auto
Spell Property ARM_SPEL_HeavyArmor_RighteousRetaliationProc Auto 
Spell Property ARM_SPEL_HeavyArmor_RightToRuleProc Auto 

Actor kPaladin
Actor kSquire

Event OnEffectStart(Actor a_kTarget, Actor a_kCaster)

    kPaladin = a_kCaster
    kSquire = a_kTarget
EndEvent

Event OnDying(Actor a_kKiller)

    If (kPaladin)

        ARM_SPEL_HeavyArmor_RighteousRetaliationProc.Cast(kPaladin)
        If (kPaladin.HasPerk(ARM_PERK_HeavyArmor_095_RightToRule_WAS_TowerOfStrength) && kPaladin == a_kKiller) 

            ARM_SPEL_HeavyArmor_RightToRuleProc.Cast(kSquire)
        EndIf

        kPaladin = NONE
        kSquire = NONE
    EndIf
EndEvent