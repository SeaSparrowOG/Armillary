install(
	FILES
		"${DATA_DIR}/Armillary.esp"
	DESTINATION "."
	COMPONENT components
	EXCLUDE_FROM_ALL
)

install(
	FILES
		"${DATA_DIR}/Armillary.bsa"
	DESTINATION "."
	COMPONENT components
	EXCLUDE_FROM_ALL
)

install(
	FILES
		"${DATA_DIR}/SKSE/Plugins/ActorValueData/Armillary_AVG.toml"
	DESTINATION "SKSE/Plugins/ActorValueData"
	COMPONENT components
	EXCLUDE_FROM_ALL
)

install(
	FILES
		"${DATA_DIR}/SKSE/Plugins/CustomSkills/SKILLS.json"
	DESTINATION "SKSE/Plugins/CustomSkills"
	COMPONENT components
	EXCLUDE_FROM_ALL
)

install(
	FILES
		"${DATA_DIR}/SKSE/Plugins/CustomSkills/Armillary/Adventuring.json"
	DESTINATION "SKSE/Plugins/CustomSkills/Armillary"
	COMPONENT components
	EXCLUDE_FROM_ALL
)

install(
	FILES
		"${DATA_DIR}/Interface/Translations/Armillary_ENGLISH.txt"
	DESTINATION "Interface/Translations"
	COMPONENT components
	EXCLUDE_FROM_ALL
)

install(
	FILES "${DATA_DIR}/Source/Scripts/SEA_Armillary.psc"
	DESTINATION "Source/Scripts"
	COMPONENT components
	EXCLUDE_FROM_ALL
)

install(
	FILES "${DATA_DIR}/Source/Scripts/ARM_ModObjects.psc"
	DESTINATION "Source/Scripts"
	COMPONENT components
	EXCLUDE_FROM_ALL
)

install(
	FILES "${DATA_DIR}/Source/Scripts/ARM_ElementsAriseHazardsShock.psc"
	DESTINATION "Source/Scripts"
	COMPONENT components
	EXCLUDE_FROM_ALL
)

install(
	FILES "${DATA_DIR}/Source/Scripts/ARM_ElementsAriseHazardsFrost.psc"
	DESTINATION "Source/Scripts"
	COMPONENT components
	EXCLUDE_FROM_ALL
)

install(
	FILES "${DATA_DIR}/Source/Scripts/ARM_ElementsAriseHazards.psc"
	DESTINATION "Source/Scripts"
	COMPONENT components
	EXCLUDE_FROM_ALL
)