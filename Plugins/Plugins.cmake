# Author: Michael Albers
# Description: Delegation CMake file to handle all plugins

add_subdirectory(Plugins/BasicPlugin/src)
add_subdirectory(Plugins/QueueingPlugin/src)

install(FILES Plugins/PluginConfig.xsd
        DESTINATION ${QS_INSTALL_PLUGIN_BASE_DIR})
