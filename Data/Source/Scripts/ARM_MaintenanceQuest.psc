Scriptname ARM_MaintenanceQuest Extends Quest

Message Property ARM_MESG_Framework_IncompatibleUpdate Auto 
Message Property ARM_MESG_Framework_Updating Auto 
Message Property ARM_MESG_Framework_Installed Auto
Message Property ARM_MESG_Framework_MissingSKSE Auto 
Message Property ARM_MESG_Framework_MissingRequirement Auto 
Message Property ARM_MESG_Framework_MissingArmillaryDLL Auto 
Message Property ARM_MESG_Framework_MissingMagicFixesDLL Auto 
Message Property ARM_MESG_Framework_OutdatedMagicFixesDLL Auto 

Int[] iVersion 

Bool Function AllRequirementsPresent()

    Int iResponse = SKSE.GetVersion()
    If (!iResponse)

        ARM_MESG_Framework_MissingSKSE.Show()
        Return False
    EndIf

    Int[] iArmillaryDLLVersion = SEA_Armillary.GetVersion()
    If (!iArmillaryDLLVersion)

        ARM_MESG_Framework_MissingArmillaryDLL.Show()
        Return False
    EndIf

    Int[] iMagicFixesVersion = SEA_MagicTweaks.GetVersion()
    If (!iMagicFixesVersion)

        ARM_MESG_Framework_MissingMagicFixesDLL.Show()
        Return False
    EndIf

    ;Required Magic Tweaks version: 1.2
    If (iMagicFixesVersion[0] < 1 || iMagicFixesVersion[1] < 2)

        ARM_MESG_Framework_OutdatedMagicFixesDLL.Show()
        Return False
    EndIf

    Return SEA_Armillary.CheckRequiredMods()
EndFunction

Int Function CheckVersion()

    If (!iVersion)

        Return -1
    EndIf

    If (iVersion[0] < 1)

        iVersion[0] = -1
        Return -1
    EndIf

    If (iVersion[1] < 0)

        Return 0
    EndIf

    Return 1
EndFunction

Function Maintenance()

    Int iResponse = CheckVersion()
    If (iResponse == -1)
        
        ARM_MESG_Framework_IncompatibleUpdate.Show()
        Stop()
        Return
    ElseIf (iResponse == 1)

        ARM_MESG_Framework_Updating.Show()
        Stop()
        Return
    EndIf

    If (!AllRequirementsPresent())

        ARM_MESG_Framework_MissingRequirement.Show()
        Stop()
        Return
    EndIf
EndFunction

Event OnInit()

    If (!AllRequirementsPresent()) 

        ARM_MESG_Framework_MissingRequirement.Show()
        Return
    EndIf

    Int[] iDLLVersion = SEA_Armillary.GetVersion()
    iVersion = New Int[2]
    iVersion[0] = iDLLVersion[0]
    iVersion[1] = iDLLVersion[1]

    String sInitializedVer = "Initialized Armillary to: [" + iVersion[0] + "." + iVersion[1] + "]"
    SEA_Armillary.LogMessage(sInitializedVer, 1)
EndEvent