# Author: Michael Albers
# Description: Builds the queueing plugin library.

file(GLOB sources *.cpp)

include_directories(../inc)
include_directories(../../BasicPlugin/inc)
add_library(qs-queueing-plugin SHARED ${sources})

include(../../PluginHelper.cmake)
QS_PLUGIN_INSTALL(qs-queueing-plugin QueueingPlugin.xml QueueingPlugin)
