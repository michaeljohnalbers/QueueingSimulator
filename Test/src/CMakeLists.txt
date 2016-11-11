# Author: Michael Albers
# Description: Builds the test utilities library.

# Test helper library
file(GLOB sources *.cpp)
add_library(qs-test-utils SHARED ${sources})
target_include_directories(qs-test-utils PUBLIC ../inc)
target_include_directories(qs-test-utils PUBLIC ../../Plugins/BasicPlugin/inc)
target_link_libraries(qs-test-utils qs-basic-plugin)

