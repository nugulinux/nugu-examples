CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
PROJECT(nugu-examples)

INCLUDE(FindPkgConfig)

SET(target_app nugu_extension_sample)
SET(storagedir "/var/lib/nugu/storage")

pkg_check_modules(pkgs REQUIRED
    nugu-extension
)
FOREACH(flag ${pkgs_CFLAGS})
	ADD_COMPILE_OPTIONS(${flag})
ENDFOREACH(flag)

ADD_DEFINITIONS(-DNUGU_STORAGE_PATH="${storagedir}/")
ADD_DEFINITIONS(-DNUGU_CONFIG_FILENAME="nugu-config.json")

ADD_SUBDIRECTORY(extension)
ADD_SUBDIRECTORY(daemon)
ADD_SUBDIRECTORY(gui)
