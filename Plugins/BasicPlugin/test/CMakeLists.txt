# Author: Michael Albers
# Description: Builds the basic plugin gtest

file(GLOB sources *cpp)
link_directories(${GTEST_LIBRARY_DIR})
add_executable (BasicPluginTest ${sources})
target_include_directories(BasicPluginTest PUBLIC ../inc ${GTEST_INCLUDE_DIR})
target_include_directories(BasicPluginTest PUBLIC ../../../Engine/inc)
target_link_libraries(BasicPluginTest ${GTEST_LIBRARY} qs-basic-plugin)
target_link_libraries(BasicPluginTest ${GTEST_LIBRARY} qs-test-utils)
