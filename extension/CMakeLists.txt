SET(target_app nugu_extension_sample)

ADD_DEFINITIONS(-DNUGU_CONFIG_FILENAME="nugu-config.json")

FILE(GLOB_RECURSE sources *.cc)

ADD_EXECUTABLE(${target_app} ${sources})

IF (TARGET nugu-extension)
	TARGET_LINK_LIBRARIES(${target_app} ${COMMON_LDFLAGS} nugu-extension)
	ADD_DEPENDENCIES(${target_app} nugu-extension)
ELSE()
	TARGET_LINK_LIBRARIES(${target_app} ${pkgs_LDFLAGS})
ENDIF()

INSTALL(TARGETS ${target_app} DESTINATION ${CMAKE_INSTALL_BINDIR})
