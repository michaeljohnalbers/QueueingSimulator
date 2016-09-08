# Author: Michael Albers
# Description: Builds the Engine gtest

file(GLOB sources *cpp)
link_directories(${GTEST_LIBRARY_DIR})
add_executable(EngineTest ${sources})
add_definitions(-DQS_PLUGIN_SCHEMA_DIR="${CMAKE_SOURCE_DIR}/Plugins")
target_include_directories(EngineTest PUBLIC ../inc ${GTEST_INCLUDE_DIR})
target_include_directories(EngineTest PUBLIC ../../Plugins/BasicPlugin/inc)
target_link_libraries(EngineTest ${GTEST_LIBRARY} qs-engine qs-basic-plugin
                      ${XERCESC_LIBRARY} dl)
