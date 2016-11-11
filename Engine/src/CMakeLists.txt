# Author: Michael Albers
# Description: Builds the engine library.

file(GLOB sources *.cpp)

add_library(qs-engine SHARED ${sources})
target_include_directories(qs-engine PUBLIC ../inc)
target_include_directories(qs-engine PUBLIC ../../Plugins/BasicPlugin/inc)
target_link_libraries(qs-engine qs-common)

install(TARGETS qs-engine
        LIBRARY DESTINATION ${QS_INSTALL_LIB_DIR})
