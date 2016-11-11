# Author: Michael Albers
# Description: Builds the common library.

file(GLOB sources *.cpp)

add_library(qs-common SHARED ${sources})
target_include_directories(qs-common PUBLIC ../inc)

install(TARGETS qs-common
        LIBRARY DESTINATION ${QS_INSTALL_LIB_DIR})
