Scriptname ARM_ModObjects extends Quest  
{Holder script for relevant perk mod objects. Read by the DLL at startup.}

;/
Common objects, used by the combat overhaul.
/;
Perk Property ARM_Illusion_PRK_000_Illusionist00 Auto
Perk Property ARM_Conjuration_PRK_000_Conjurer00 Auto
Perk Property ARM_Destruction_PRK_000_Elementalist00 Auto
Perk Property ARM_Restoration_PRK_000_Healer00 Auto
Perk Property ARM_Alteration_PRK_000_Thaumaturge00 Auto

Spell Property ARM_Common_SPL_StaggerCooldownSpell Auto
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