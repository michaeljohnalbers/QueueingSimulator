# Author: Michael Albers
# Description: Builds the basic plugin library.

file(GLOB sources *.cpp)

include_directories(../inc)
add_library(qs-basic-plugin SHARED ${sources})

include(../../PluginHelper.cmake)
QS_PLUGIN_INSTALL(qs-basic-plugin BasicPlugin.xml)
