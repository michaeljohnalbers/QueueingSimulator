# Author: Michael Albers
# Description: Builds the queueing plugin gtest

file(GLOB sources *cpp)
link_directories(${GTEST_LIBRARY_DIR})
add_executable (QueueingPluginTest ${sources})
target_include_directories(QueueingPluginTest PUBLIC
  ../inc ../../BasicPlugin/inc ${GTEST_INCLUDE_DIR} ../../../Engine/inc
  ../../../Test/inc)
target_link_libraries(QueueingPluginTest ${GTEST_LIBRARY} qs-queueing-plugin
  qs-basic-plugin qs-test-utils)
