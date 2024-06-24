CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
PROJECT(nugu-examples)

INCLUDE(FindPkgConfig)
INCLUDE(GNUInstallDirs)
INCLUDE(cmake/Feature.cmake)
INCLUDE(cmake/Utils.cmake)

# C++ standard: c++11 (not gnu++11)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_EXTENSIONS OFF)

# C standard: gnu99 (not c99)
SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_EXTENSIONS ON)

# setup storage path
SET(storagedir "/var/lib/nugu/storage")
IF(ENABLE_HOME_STORAGE_DIR)
	SET(storagedir $ENV{HOME}/.nugu)
ENDIF()

pkg_check_modules(pkgs REQUIRED nugu-extension)
FOREACH(flag ${pkgs_CFLAGS})
	ADD_COMPILE_OPTIONS(${flag})
ENDFOREACH(flag)

IF (ENABLE_NUGU_EXTENSION_SAMPLE)
	ADD_SUBDIRECTORY(extension)
ENDIF()

IF (ENABLE_NUGU_GUI_SAMPLE)
	ADD_SUBDIRECTORY(gui)
ENDIF()

IF (ENABLE_NUGU_AUTH_SAMPLE)
	ADD_SUBDIRECTORY(auth)
ENDIF()
