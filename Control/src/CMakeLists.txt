# Author: Michael Albers
# Description: Builds the GUI control library.

file(GLOB sources *.cpp)

add_library(qs-control SHARED ${sources})

target_include_directories(qs-control PRIVATE ../inc)
target_include_directories(qs-control PRIVATE ../../Engine/inc)
target_link_libraries(qs-control qs-engine)

install(TARGETS qs-control
        LIBRARY DESTINATION ${QS_INSTALL_LIB_DIR})
