# Author: Michael Albers
# Description: Builds the GUI control library.

file(GLOB sources *.cpp)

include_directories(../inc)
add_library(qs-control SHARED ${sources})

install(TARGETS qs-control
        LIBRARY DESTINATION ${QS_INSTALL_LIB_DIR})
