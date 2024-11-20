Scriptname ARM_ElementsAriseHazards extends activemagiceffect  
{Places hazards while the user is concentrating every few parts of a second.}

Hazard Property ARM_Destruction_HZD_FireBarrierHazard Auto

Actor caster
Int iMaxUpdates = 20

Event OnEffectStart(Actor a_kTarget, Actor a_kCaster) 

    caster = a_kTarget
    iMaxUpdates = 20
    caster.PlaceAtMe(ARM_Destruction_HZD_FireBarrierHazard).SetActorCause(caster)
    RegisterForSingleUpdate(0.3)
EndEvent

Event OnUpdate()

    caster.PlaceAtMe(ARM_Destruction_HZD_FireBarrierHazard).SetActorCause(caster)
    iMaxUpdates -= 1

    If (iMaxUpdates > 0)

        RegisterForSingleUpdate(0.3)
    EndIf
EndEvent

Event OnEffectFinish(Actor a_kTarget, Actor a_kCaster)

    UnregisterForUpdate()
    iMaxUpdates = 0
EndEvent    