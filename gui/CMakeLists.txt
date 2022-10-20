# set CMP0100 policy from CMake version 3.24.2 for using AUTOMOC
IF(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.24.2")
    CMAKE_POLICY(SET CMP0100 NEW)
ENDIF()

FIND_PACKAGES(Qt5 "Core;Quick;WebEngine;WebChannel")
pkg_check_modules(pkgs REQUIRED nugu-extension)

FOREACH(flag ${pkgs_CFLAGS})
	ADD_COMPILE_OPTIONS(${flag})
ENDFOREACH(flag)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

SET(target_app nugu_gui_sample)

ADD_DEFINITIONS(-DNUGU_GUI_CONFIG_FILENAME="nugu-gui-config.json")
ADD_DEFINITIONS(-DNUGU_TEMPLATE_CONFIG_FILENAME="nugu-template-config.json")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
FILE(GLOB_RECURSE sources src/*.cpp src/*.hpp *.hpp)

ADD_EXECUTABLE(${target_app} ${sources} assets/resource.qrc)
TARGET_LINK_LIBRARIES(${target_app} PRIVATE "${pkgs_LDFLAGS}" ${Qt5_LIBS})

INSTALL(TARGETS ${target_app} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
INSTALL(DIRECTORY config/ DESTINATION ${storagedir})
