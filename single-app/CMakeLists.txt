PROJECT(nugu-single-sample)

SET(target_app nugu_single_sample)

FILE(GLOB_RECURSE sources *.cc)
ADD_EXECUTABLE(${target_app} ${sources})
