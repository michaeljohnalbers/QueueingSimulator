# Author: Michael Albers
# Description: Builds the engine library.

file(GLOB sources *.cpp)

include_directories(../inc)
add_library(qs-engine SHARED ${sources})

install(TARGETS qs-engine
        LIBRARY DESTINATION ${QS_INSTALL_LIB_DIR})
