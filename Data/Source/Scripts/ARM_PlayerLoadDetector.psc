Scriptname ARM_PlayerLoadDetector Extends ReferenceAlias

Event OnPlayerLoadGame()

    (Self.GetOwningQuest() As ARM_MaintenanceQuest).Maintenance()
EndEvent