# Author: Michael Albers
# Description: Builds the engine library.

file(GLOB sources *.cpp)

include_directories(../inc)
add_library(qs-engine SHARED ${sources})
