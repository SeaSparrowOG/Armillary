Scriptname ARM_ModObjects extends Quest  
{Holder script for relevant perk mod objects. Read by the DLL at startup.}

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