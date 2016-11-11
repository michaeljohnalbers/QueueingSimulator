# Author: Michael Albers
# Description: Builds the basic plugin library.

file(GLOB sources *.cpp)

include_directories(../inc)
add_library(qs-basic-plugin SHARED ${sources})
target_link_libraries(qs-basic-plugin qs-common)

include(../../PluginHelper.cmake)
QS_PLUGIN_INSTALL(qs-basic-plugin BasicPlugin.xml BasicPlugin)
