# Author: Michael Albers
# Description: Builds the Common gtest

file(GLOB sources *cpp)
link_directories(${GTEST_LIBRARY_DIR})
add_executable(CommonTest ${sources})
target_include_directories(CommonTest PUBLIC ../inc ${GTEST_INCLUDE_DIR}
  ../../Test/inc)
target_link_libraries(CommonTest ${GTEST_LIBRARY} qs-common)
