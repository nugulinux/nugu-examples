SET(target_app nugu_extension_sample)

FILE(GLOB_RECURSE sources *.cc)
ADD_EXECUTABLE(${target_app} ${sources})
