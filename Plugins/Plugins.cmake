# Author: Michael Albers
# Description: Delegation CMake file to handle all plugins

add_subdirectory(Plugins/BasicPlugin/src)

install(FILES Plugins/PluginConfig.xsd
        DESTINATION ${QS_INSTALL_PLUGIN_BASE_DIR})
