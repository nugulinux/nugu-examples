PROJECT(nugu-multi-sample)

SET(target_app nugu_multi_sample)

FILE(GLOB_RECURSE sources *.cc)
ADD_EXECUTABLE(${target_app} ${sources})
