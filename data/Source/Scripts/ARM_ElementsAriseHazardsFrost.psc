Scriptname ARM_ElementsAriseHazardsFrost extends ActiveMagicEffect  

Hazard Property ARM_Destruction_HZD_FrostBarrierHazard Auto

Actor caster
Int iMaxUpdates = 20

Event OnEffectStart(Actor a_kTarget, Actor a_kCaster) 

    caster = a_kTarget
    iMaxUpdates = 20
    caster.PlaceAtMe(ARM_Destruction_HZD_FrostBarrierHazard).SetActorCause(caster)
    RegisterForSingleUpdate(0.3)
EndEvent

Event OnUpdate()

    caster.PlaceAtMe(ARM_Destruction_HZD_FrostBarrierHazard).SetActorCause(caster)
    iMaxUpdates -= 1

    If (iMaxUpdates > 0)

        RegisterForSingleUpdate(0.3)
    EndIf
EndEvent

Event OnEffectFinish(Actor a_kTarget, Actor a_kCaster)

    UnregisterForUpdate()
    iMaxUpdates = 0
EndEvent    