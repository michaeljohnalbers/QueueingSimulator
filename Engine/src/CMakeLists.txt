# Author: Michael Albers
# Description: Builds the engine library.

file(GLOB sources *.cpp)

include_directories(../inc)
add_library(qs-engine SHARED ${sources})
target_compile_options(qs-engine PUBLIC --std=c++11 -Wall)
