Scriptname ARM_ModObjects extends Quest  
{Holder script for relevant perk mod objects. Read by the DLL at startup.}

;/
Common objects, used by the combat overhaul.
/;
Perk Property ARM_Alteration_PRK_020_AlterationDualCasting Auto
Perk Property ARM_Conjuration_PRK_020_ConjurationDualCasting Auto
Perk Property ARM_Destruction_PRK_020_DestructionDualCasting Auto
Perk Property ARM_Illusion_PRK_020_IllusionDualCasting Auto
Perk Property ARM_Restoration_PRK_020_DualCasting Auto
Perk Property ARM_Common_avManager Auto 

Spell Property ARM_Common_SPL_StaggerCooldownSpell Auto
Spell Property ARM_Common_SPL_VulnerableTargetMarker Auto 

MagicEffect Property ARM_Common_MGF_StaggerCooldownFFTActor Auto
{Cooldown spell effect. The duration of it defines how long actors won't be stunned again for.}

;/
Alchemy related objects.
/;
Formlist Property ARM_Alchemy_FRL_UncommonIngredientsList Auto 
{Ingredients to give a +10% boost if the relevant perk is acquired.}
Formlist Property ARM_Alchemy_FRL_RareIngredientsList Auto 
{Ingredients to give a +25% boost if the relevant perk is acquired.}
Formlist Property ARM_Alchemy_FRL_ExquisiteIngredientsList Auto 
{Ingredients to give a +50% boost if the relevant perk is acquired.}

;Effects used by stimulants, DO NOT ALTER.
MagicEffect Property ARM_Alchemy_MGF_StimulantsHealth Auto
MagicEffect Property ARM_Alchemy_MGF_StimulantsMagicka Auto
MagicEffect Property ARM_Alchemy_MGF_StimulantsStamina Auto

MagicEffect Property ARM_Alchemy_MGF_SlowDeathEffect Auto
{Effect used by slow death.}

Perk Property ARM_Alchemy_PRK_060_QualityIngredients Auto
Perk Property ARM_Alchemy_PRK_050_LastingTreatment Auto 
Perk Property ARM_Alchemy_PRK_080_SlowDeath Auto 
Perk Property ARM_Alchemy_PRK_080_Stimulants Auto 

;/
Armor related objects
/;
Perk Property ARM_Armor_PRK_100_Unbreakable Auto

Keyword Property ARM_Armor_KWD_HeavyCuirass Auto 
Keyword Property ARM_Armor_KWD_LightCuirass Auto 
Keyword Property ARM_Armor_KWD_ClothingCuirass Auto 
;/
Destruction related objects
/;
Keyword Property ARM_Destruction_KWD_MagicCloakFire Auto
{Automatically distributed to flame cloak spells at startup.}
Keyword Property ARM_Destruction_KWD_MagicCloakFrost Auto
{Automatically distributed to frost cloak spells at startup.}
Keyword Property ARM_Destruction_KWD_MagicCloakShock Auto
{Automatically distributed to shock cloak spells at startup.}

Perk Property ARM_Destruction_PRK_040_Inspired Auto 
Perk Property ARM_Destruction_PRK_080_ElementsArise Auto
Perk Property ARM_Destruction_PRK_050_ReIgnite Auto
Perk Property ARM_Destruction_PRK_050_Wither Auto 
Perk Property ARM_Destruction_PRK_050_Incapacitate Auto 
Perk Property ARM_Destruction_PRK_070_ArcBurn Auto

Spell Property ARM_Destruction_SPL_InspireProc Auto
{Applied by the DLL when an actor dual casts a destruction spell while they have the Inspired perk.}

Spell Property ARM_Destruction_SPL_WitherProc Auto
Spell Property ARM_Destruction_SPL_ReIgniteProc Auto 
Spell Property ARM_Destruction_SPL_IncapacitateProc Auto
{Applied by the DLL when their respective perks would normally apply.}


Spell Property ARM_Destruction_SPL_DummyFireCastDetector Auto
Spell Property ARM_Destruction_SPL_DummyFrostCastDetector Auto
Spell Property ARM_Destruction_SPL_DummyShockCastDetector Auto
{Dummy spells, never applied to anyone. The DLL hijacks IsCasting, and if IsCasting checks for these spells in the left hand, it checks to see if you are casting an elemental spell instead.}