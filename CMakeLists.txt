CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
PROJECT(nugu-examples)

INCLUDE(FindPkgConfig)
INCLUDE(GNUInstallDirs)
INCLUDE(cmake/Feature.cmake)

SET(target_app nugu_extension_sample)
SET(storagedir "/var/lib/nugu/storage")

pkg_check_modules(pkgs REQUIRED
    nugu-extension
)
FOREACH(flag ${pkgs_CFLAGS})
	ADD_COMPILE_OPTIONS(${flag})
ENDFOREACH(flag)

ADD_DEFINITIONS(-DNUGU_STORAGE_PATH="${storagedir}/")

IF (ENABLE_NUGU_EXTENSION_SAMPLE)
    ADD_SUBDIRECTORY(extension)
ENDIF()

IF (ENABLE_NUGU_GUI_SAMPLE)
    ADD_SUBDIRECTORY(gui)
ENDIF()
