SET(target_app nugu_gui_sample)

FILE(GLOB_RECURSE sources *.cc)
ADD_EXECUTABLE(${target_app} ${sources})