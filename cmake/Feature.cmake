INCLUDE(FeatureSummary)

MACRO(DEFINE_FEATURE id description default)
	OPTION(ENABLE_${id} "Enable - ${description} (Default: ${default})" ${default})
	add_feature_info(FEATURE_${id} ENABLE_${id} "${description} (Default: ${default})")
ENDMACRO(DEFINE_FEATURE)

DEFINE_FEATURE(NUGU_EXTENSION_SAMPLE "Enable NUGU extension sample" ON)
DEFINE_FEATURE(NUGU_GUI_SAMPLE "Enable NUGU gui sample" ON)
DEFINE_FEATURE(HOME_STORAGE_DIR "Enable to use user home directory to storage" OFF)

feature_summary(WHAT ALL)
