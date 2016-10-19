# Author: Michael Albers
# Description: Builds the GUI control library.

file(GLOB sources *.cpp)

add_library(qs-control SHARED ${sources})

target_include_directories(qs-control PRIVATE ../inc)
target_include_directories(qs-control PRIVATE ../data) # For XPM file
target_include_directories(qs-control PRIVATE ../../Engine/inc)
target_include_directories(qs-control PRIVATE ../../Visualization/inc)
target_include_directories(qs-control PRIVATE ${GLFW_INCLUDE_DIR})
target_include_directories(qs-control PRIVATE ${GLM_INCLUDE_DIR})
target_link_libraries(qs-control qs-engine)

install(TARGETS qs-control
        LIBRARY DESTINATION ${QS_INSTALL_LIB_DIR})
