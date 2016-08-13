# Author: Michael Albers
# Description: Builds the basic plugin library.

file(GLOB sources *.cpp)

include_directories(../inc)
add_library(qs-basic-plugin SHARED ${sources})
