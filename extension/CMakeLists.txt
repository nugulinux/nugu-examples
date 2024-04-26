SET(target_app nugu_extension_sample)

ADD_DEFINITIONS(-DNUGU_CONFIG_FILENAME="nugu-config.json")

FILE(GLOB_RECURSE sources *.cc)

ADD_EXECUTABLE(${target_app} ${sources})
TARGET_LINK_LIBRARIES(${target_app} ${pkgs_LDFLAGS} -lnugu-extension -lstdc++)

IF (TARGET nugu-extension)
    ADD_DEPENDENCIES(${target_app} nugu-extension)
ENDIF()

INSTALL(TARGETS ${target_app} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
