SET(target_app nugu_extension_sample)

FILE(GLOB_RECURSE sources *.cc)

ADD_EXECUTABLE(${target_app} ${sources})
TARGET_LINK_LIBRARIES(${target_app} ${pkgs_LDFLAGS} -lnugu-extension)
ADD_DEPENDENCIES(${target_app} nugu-extension)

INSTALL(TARGETS ${target_app} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
INSTALL(FILES config/nugu-config.json DESTINATION ${storagedir})
