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