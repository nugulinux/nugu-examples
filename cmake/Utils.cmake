# Macro for expanding prebuilt FIND_PACKAGE function.
# It produce ${package_LIBS} variable which is used to link libraries.
MACRO(FIND_PACKAGES pacakge components)
    FOREACH(component ${components})
        SET(${pacakge}_LIBS ${${pacakge}_LIBS} "${pacakge}::${component}")
    ENDFOREACH()

    FIND_PACKAGE(${pacakge} COMPONENTS "${components}" REQUIRED)
ENDMACRO()
