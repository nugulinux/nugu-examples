FIND_PACKAGE(Qt5Core REQUIRED)
pkg_check_modules(pkgs REQUIRED
    nugu-extension
    Qt5Quick
    Qt5WebEngine
    Qt5WebChannel)

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
FILE(GLOB_RECURSE sources src/*.cpp src/*.hpp)

ADD_EXECUTABLE(${target_app} ${sources} assets/resource.qrc)
TARGET_LINK_LIBRARIES(${target_app} PRIVATE ${pkgs_LDFLAGS})

INSTALL(TARGETS ${target_app} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
